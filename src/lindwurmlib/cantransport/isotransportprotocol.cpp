/*  www.lindwurm-can.org
 *  Copyright (C) 2023 Sascha Muenzberg <sascha@lindwurm-can.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cantransport/isotransportprotocol.h"

#include "caninterface/icaninterfacehandle.h"

#include <QDebug>
#include <QLoggingCategory>

namespace
{
    Q_LOGGING_CATEGORY(LOG_TAG, "lindwurm.lib.transport")
    const int DEFAULT_BLOCK_SIZE = 0;
    const int DEFAULT_SEPARATION_TIME = 0;
    const int DEFAULT_TIMEOUT = 1000;
    const int MAX_WAIT_CYCLES = 5;
}

namespace Lindwurm::Lib
{
    IsoTransportProtocol::IsoTransportProtocol(quint32 sourceAddress, quint32 targetAddress, QObject *parent)
        : QObject(parent)
        , m_sourceAddress(sourceAddress)
        , m_targetAddress(targetAddress)
    {
        m_sendConnection.timeoutTimer.setSingleShot(true);
        m_sendConnection.timeoutTimer.setInterval(DEFAULT_TIMEOUT);

        m_receiveConnection.timeoutTimer.setSingleShot(true);
        m_receiveConnection.timeoutTimer.setInterval(DEFAULT_TIMEOUT);

        m_separationTimer.setSingleShot(true);

        // since separation time is specified as _minimum_ we use precise timers as they will never time out earlier than expected
        m_separationTimer.setTimerType(Qt::PreciseTimer);

        resetSendConnection();
        resetReceiveConnection();

        connect(&m_sendConnection.timeoutTimer,     &QTimer::timeout, this, &IsoTransportProtocol::sendTimeout);
        connect(&m_receiveConnection.timeoutTimer,  &QTimer::timeout, this, &IsoTransportProtocol::receiveTimeout);

        connect(&m_separationTimer,                 &QTimer::timeout, this, &IsoTransportProtocol::continueSending);
    }

    void IsoTransportProtocol::mountCANInterface(ICanInterfaceHandleSharedPtr interface)
    {
        unmountCANInterface();

        m_canInterface = interface;
        connect(m_canInterface.get(), &ICanInterfaceHandle::frameReceived, this, &IsoTransportProtocol::canFrameReceived);
    }

    void IsoTransportProtocol::unmountCANInterface()
    {
        if (m_canInterface)
        {
            disconnect(m_canInterface.get(), &ICanInterfaceHandle::frameReceived, this, &IsoTransportProtocol::canFrameReceived);
            m_canInterface->unmount();
            m_canInterface.reset();
        }
    }

    void IsoTransportProtocol::setPaddingEnabled(bool enablePadding)
    {
        m_usePadding = enablePadding;
    }

    quint32 IsoTransportProtocol::sourceAddress() const
    {
        return m_sourceAddress;
    }

    quint32 IsoTransportProtocol::targetAddress() const
    {
        return m_targetAddress;
    }

    void IsoTransportProtocol::sendTimeout()
    {
        qWarning(LOG_TAG) << "Timeout while sending data. Reset send connection!";
        resetSendConnection();

        emit errorOccurred(IsoTpError::Timeout);
    }

    void IsoTransportProtocol::receiveTimeout()
    {
        qWarning(LOG_TAG) << "Timeout while receiving data. Reset receive connection!";
        resetReceiveConnection();

        emit errorOccurred(IsoTpError::Timeout);
    }


    void IsoTransportProtocol::canFrameReceived(const QCanBusFrame &frame, const QString &sourceInterface)
    {
        Q_UNUSED(sourceInterface)

        if ( frame.frameId() != m_targetAddress )
        {
            return;
        }

        IsoTransportProtocolFrame tpFrame = IsoTransportProtocolFrame::fromRawCanFrame(frame);

        switch ( tpFrame.frameType() )
        {
            case IsoTransportProtocolFrame::Type::SingleFrame:

                if ( m_receiveConnection.state != IsoTpConnectionState::Idle )
                {
                    qWarning(LOG_TAG) << "Ignoring unexpected single frame while receiving a segmented message.";
                    emit errorOccurred(IsoTpError::UnexpectedFrame);

                    return;
                }

                singleFrameReceived(tpFrame);

                return;

            case IsoTransportProtocolFrame::Type::FirstFrame:

                if ( m_receiveConnection.state != IsoTpConnectionState::Idle )
                {
                    qWarning(LOG_TAG) << "Ignoring unexpected first frame while already receiving a segmented message.";
                    emit errorOccurred(IsoTpError::UnexpectedFrame);

                    return;
                }

                firstFrameReceived(tpFrame);

                return;

            case IsoTransportProtocolFrame::Type::ConsecutiveFrame:

                if ( m_receiveConnection.state != IsoTpConnectionState::Transmission )
                {
                    qWarning(LOG_TAG) << "Ignoring unexpected consecutive frame while not in transmission state.";
                    emit errorOccurred(IsoTpError::UnexpectedFrame);

                    return;
                }

                consecutiveFrameReceived(tpFrame);

                return;

            case IsoTransportProtocolFrame::Type::FlowControlFrame:

                if ( m_sendConnection.state != IsoTpConnectionState::Transmission )
                {
                    qWarning(LOG_TAG) << "Ignoring unexpected flow control frame while not in transmission state.";
                    emit errorOccurred(IsoTpError::UnexpectedFrame);

                    return;
                }

                flowControlFrameReceived(tpFrame);

                return;

            case IsoTransportProtocolFrame::Type::Invalid:

                qWarning(LOG_TAG) << "Received invalid ISO TP frame.";

                return;
        }
    }

    int IsoTransportProtocol::paddingSize() const
    {
        if ( ! m_usePadding )
        {
            return 0;
        }

        if ( m_canInterface->flexibleDataRateEnabled() )
        {
            return 64;
        }
        else
        {
            return 8;
        }
    }

    void IsoTransportProtocol::resetSendConnection()
    {
        m_sendConnection.timeoutTimer.stop();
        m_sendConnection.data.clear();

        m_sendConnection.state                  = IsoTpConnectionState::Idle;
        m_sendConnection.dataLength             = 0;
        m_sendConnection.blockSize              = 0;
        m_sendConnection.minSeparationTime      = 0;
        m_sendConnection.currentBlockNumber     = 0;
        m_sendConnection.currentSequenceNumber  = 1;
        m_sendConnection.currentWaitCycle       = 0;

        m_dataToSentCopy.clear();
    }

    void IsoTransportProtocol::resetReceiveConnection()
    {
        m_receiveConnection.timeoutTimer.stop();
        m_receiveConnection.data.clear();

        m_receiveConnection.state                  = IsoTpConnectionState::Idle;
        m_receiveConnection.dataLength             = 0;
        m_receiveConnection.blockSize              = DEFAULT_BLOCK_SIZE;
        m_receiveConnection.minSeparationTime      = DEFAULT_SEPARATION_TIME;
        m_receiveConnection.currentBlockNumber     = 0;
        m_receiveConnection.currentSequenceNumber  = 1;
        m_receiveConnection.currentWaitCycle       = 0;
    }

    void IsoTransportProtocol::singleFrameReceived(IsoTransportProtocolFrame &tpFrame)
    {
        QByteArray tpData = tpFrame.data();

        if ( tpData.size() != tpFrame.dataLength() )
        {
            qWarning(LOG_TAG) << "Received malformed ISO TP frame: expected" << tpFrame.dataLength() << "bytes, but received"  << tpData.size() << "bytes.";

            emit errorOccurred(IsoTpError::MalformedFrame);
        }
        else
        {
            emit dataReceived( tpFrame.data() );
        }
    }

    void IsoTransportProtocol::firstFrameReceived(IsoTransportProtocolFrame &tpFrame)
    {
        qDebug(LOG_TAG) << "First frame received. Entering receiveing state.";

        m_receiveConnection.state       = IsoTpConnectionState::Transmission;
        m_receiveConnection.dataLength  = tpFrame.dataLength();
        m_receiveConnection.data        = tpFrame.data();

        sendFlowControlFrame();

        m_receiveConnection.timeoutTimer.start();
    }

    void IsoTransportProtocol::consecutiveFrameReceived(IsoTransportProtocolFrame &tpFrame)
    {
        m_receiveConnection.timeoutTimer.stop();

        if ( tpFrame.sequenceNumber() != m_receiveConnection.currentSequenceNumber )
        {
            qCritical(LOG_TAG) << "Received unexpected sequence number " << tpFrame.sequenceNumber();

            resetReceiveConnection();

            emit errorOccurred(IsoTpError::OutOfOrderData);

            return;
        }

        // current received bytes + maximum bytes of one consecutive frame
        bool isLastFrame = (m_receiveConnection.data.size() + 7) >= m_receiveConnection.dataLength;

        if ( isLastFrame )
        {
            int remainingBytes = m_receiveConnection.dataLength - m_receiveConnection.data.size();

            QByteArray remainingData = tpFrame.data();

            if ( remainingData.size() < remainingBytes )
            {
                // last frame does not contain all expected remaining data
                // this assumes each consecutive frame has to be completely filled
                qCritical(LOG_TAG) << "Unexpected end of data stream.";

                resetReceiveConnection();

                emit errorOccurred(IsoTpError::DataError);

                return;
            }

            // remove possible appended padding
            m_receiveConnection.data.append( remainingData.mid(0, remainingBytes) );

            QByteArray receivedData = m_receiveConnection.data;

            // calling reset before sending signal to allow recursive calls to IsoTransportProtocol in slots
            resetReceiveConnection();

            emit dataReceived( receivedData );

            return;
        }
        else
        {
            m_receiveConnection.currentSequenceNumber = (m_receiveConnection.currentSequenceNumber + 1) % 16;
            m_receiveConnection.data.append( tpFrame.data() );
        }

        if ( m_receiveConnection.blockSize != 0 )
        {
            m_receiveConnection.currentBlockNumber++;

            if ( m_receiveConnection.currentBlockNumber == m_receiveConnection.blockSize )
            {
                // we have reached the maximum number of blocks, so we must send the next flow control frame before the sender continues
                sendFlowControlFrame();
                m_receiveConnection.currentBlockNumber = 0;
            }
        }

        m_receiveConnection.timeoutTimer.start();
    }

    void IsoTransportProtocol::sendFlowControlFrame()
    {
        IsoTransportProtocolFrame flowControlFrame = IsoTransportProtocolFrame::flowControlFrame(m_sourceAddress, IsoTpFlowStatus::ClearToSend, DEFAULT_BLOCK_SIZE, DEFAULT_SEPARATION_TIME, paddingSize() );

        if ( flowControlFrame.isValid() )
        {
            m_canInterface->sendFrame( flowControlFrame.canFrame() );
        }
    }

    bool IsoTransportProtocol::sendSingleFrame(const QByteArray &data)
    {
        IsoTransportProtocolFrame singleFrame = IsoTransportProtocolFrame::singleFrame(m_sourceAddress, data, paddingSize() );

        return m_canInterface->sendFrame( singleFrame.canFrame() );
    }

    bool IsoTransportProtocol::sendData(const QByteArray &data)
    {
        if ( ! m_canInterface )
        {
            // we don't have an interface mounted
            return false;
        }

        if ( m_sendConnection.state == IsoTpConnectionState::Transmission )
        {
            // TODO: Monitor if this may a legitimate and possible situation. If this may happen we could implement some kind
            // of sending buffer
            qWarning(LOG_TAG) << "Could not send data because a transmission is already in progress.";

            return false;
        }

        if ( data.size() <= 7 )
        {
            if ( sendSingleFrame(data) == true )
            {
                emit dataSent(data);
                return true;
            }
        }
        else
        {
            m_sendConnection.data = data;
            m_dataToSentCopy = data;

            return sendFirstFrame();
        }

        return false;
    }

    bool IsoTransportProtocol::sendFirstFrame()
    {
        if ( m_sendConnection.data.size() > 4096 )
        {
            qCritical(LOG_TAG) << "Sending data with size > 4096 is not supported with ISO-TP!";

            resetSendConnection();

            return false;
        }

        m_sendConnection.state = IsoTpConnectionState::Transmission;

        IsoTransportProtocolFrame firstFrame = IsoTransportProtocolFrame::firstFrame(m_sourceAddress, m_sendConnection.data.size(), m_sendConnection.data.left(6) );
        m_sendConnection.data.remove(0, 6);

        if ( m_canInterface->sendFrame( firstFrame.canFrame() ) == false )
        {
            // if we could not send the first frame, we cancel the transmission
            resetSendConnection();
            return false;
        }

        m_sendConnection.timeoutTimer.start();

        return true;
    }

    void IsoTransportProtocol::flowControlFrameReceived(IsoTransportProtocolFrame &tpFrame)
    {
        m_sendConnection.timeoutTimer.stop();

        qDebug(LOG_TAG) << "flowControlFrame received! blockSize: " << tpFrame.blockSize() << " Separation time: " << tpFrame.separationTime();

        m_sendConnection.blockSize = tpFrame.blockSize();
        m_sendConnection.minSeparationTime = tpFrame.separationTime();

        switch ( tpFrame.flowStatus() )
        {
            case IsoTpFlowStatus::ClearToSend:

                m_sendConnection.currentWaitCycle = 0; // only count successive wait cycles => reset every non blocking frame
                continueSending();

                return;

            case IsoTpFlowStatus::Wait:

                m_sendConnection.currentWaitCycle++;

                if ( m_sendConnection.currentWaitCycle >= MAX_WAIT_CYCLES)
                {
                    qDebug() << "Received to many wait cycles. Reset send connection!";
                    resetSendConnection();
                    return;
                }

                qDebug(LOG_TAG) << "Waiting for next flow control frame";
                m_sendConnection.timeoutTimer.start();
                return;

            case IsoTpFlowStatus::Overflow:

                qDebug(LOG_TAG) << "Overflow";
                resetSendConnection();

                emit errorOccurred(IsoTpError::OverFlow);

                return;
        }
    }

    bool IsoTransportProtocol::sendNextConsecutiveFrame()
    {
        IsoTransportProtocolFrame consecutiveFrame = IsoTransportProtocolFrame::consecutiveFrame(m_sourceAddress, m_sendConnection.currentSequenceNumber, m_sendConnection.data.left(7), paddingSize() );
        m_sendConnection.data.remove(0, 7);

        m_sendConnection.currentSequenceNumber = (m_sendConnection.currentSequenceNumber + 1) % 16;

        return m_canInterface->sendFrame( consecutiveFrame.canFrame() );
    }

    void IsoTransportProtocol::continueSending()
    {
        do
        {
            sendNextConsecutiveFrame();

            if ( m_sendConnection.data.size() == 0 )
            {
                QByteArray sentDataLocalCopy = m_dataToSentCopy;

                resetSendConnection();

                emit dataSent(sentDataLocalCopy);

                return;
            }

            if ( m_sendConnection.blockSize != 0 )
            {
                m_sendConnection.currentBlockNumber++;

                if ( m_sendConnection.currentBlockNumber == m_sendConnection.blockSize )
                {
                    // we reached the maximum number of blocks we're allowed to send before we receive the next consecutive frame
                    // so we reset the current block number and wait for the next consecutive frame
                    m_sendConnection.currentBlockNumber = 0;
                    m_sendConnection.timeoutTimer.start();

                    return;
                }
            }

            if ( m_sendConnection.minSeparationTime != 0 )
            {
                if ( m_sendConnection.minSeparationTime <= 127 )
                {
                    // STmin 0x00 - 0x7F (0 - 127) => units of STmin are absolute milliseconds
                    m_separationTimer.setInterval(m_sendConnection.minSeparationTime);
                }
                else
                {
                    if ( (m_sendConnection.minSeparationTime >= 0xF1) && (m_sendConnection.minSeparationTime <= 0xF9) )
                    {
                        // STmin 0x00 - 0xF1 => units of STmin are multiples of 100us
                        // 0xF1 => 100us
                        // 0xF9 => 900us

                        // since QTimer only supports ms precision, we always set 1 ms
                        m_separationTimer.setInterval(1);
                    }
                    else
                    {
                        // Reserved values
                        // 0x80 - 0xF0
                        // 0xFA - 0xFF

                        // should not happen, but for robust error handling
                        // we shall use the longest STmin value specified by the standard => 127 ms
                        m_separationTimer.setInterval(127);
                    }
                }

                m_separationTimer.start();

                return;
            }

        }
        while (true);

    }

}
