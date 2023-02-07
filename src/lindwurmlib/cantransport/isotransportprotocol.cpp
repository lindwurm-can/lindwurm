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
}

namespace Lindwurm::Lib
{
    IsoTransportProtocol::IsoTransportProtocol(quint32 sourceAddress, quint32 targetAddress, QObject *parent)
        : QObject(parent)
        , m_sourceAddress(sourceAddress)
        , m_targetAddress(targetAddress)
    {

    }

    void IsoTransportProtocol::setPaddingEnabled(bool enablePadding)
    {
        m_usePadding = enablePadding;
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

    quint32 IsoTransportProtocol::sourceAddress() const
    {
        return m_sourceAddress;
    }

    quint32 IsoTransportProtocol::targetAddress() const
    {
        return m_targetAddress;
    }

    bool IsoTransportProtocol::sendData(const QByteArray &data)
    {
        if ( ! m_canInterface )
        {
            // we don't have an interface mounted
            return false;
        }

        if ( m_state != IsoTpState::Idle )
        {
            return false;
        }

        if ( data.size() <= 7 )
        {
            // send as single frame
            if ( sendSingleFrame(data) == true )
            {
                //qDebug(LOG_TAG) << "Send data: " << data.toHex();
                emit dataSent(data);
            }
            else
            {
                return false;
            }
        }

        return false;
    }

    void IsoTransportProtocol::canFrameReceived(const QCanBusFrame &frame, const QString &sourceInterface)
    {
        Q_UNUSED(sourceInterface)

        if ( frame.frameId() != m_targetAddress )
        {
            return;
        }

        IsoTransportProtocolFrame tpFrame(frame);

        switch ( tpFrame.frameType() )
        {
            case IsoTransportProtocolFrame::IsoTpFrameType::SingleFrame:

                singleFrameReceived(tpFrame);
                return;

            case IsoTransportProtocolFrame::IsoTpFrameType::FirstFrame:

                firstFrameReceived(tpFrame);
                break;

            case IsoTransportProtocolFrame::IsoTpFrameType::ConsecutiveFrame:

                if ( m_state == IsoTpState::Receiving )
                {
                    consecutiveFrameReceived(tpFrame);
                }
                else
                {
                    emit errorOccurred(IsoTpError::UnexpectedFrame);
                }

                break;

            case IsoTransportProtocolFrame::IsoTpFrameType::FlowControlFrame:

                break;

            case IsoTransportProtocolFrame::IsoTpFrameType::Invalid:

                qCritical(LOG_TAG) << "Received invalid ISO TP frame";
                return;
        }
    }

    bool IsoTransportProtocol::sendSingleFrame(const QByteArray &data)
    {
        int payloadSize = data.size();

        QByteArray frameData;

        frameData.push_front( static_cast<char>(payloadSize) );
        frameData.append(data);

        int paddingLength = 0;

        if ( m_usePadding )
        {
            paddingLength = paddingSize() - frameData.size();

            for (int i = 0; i < paddingLength; i++)
            {
                frameData.append('\x00');
            }
        }

        QCanBusFrame singleFrame(m_sourceAddress, frameData);

        return m_canInterface->sendFrame(singleFrame);
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
        // TODO: Implement timeout: In case of error (no consecutive frames follows) reset to Idle mode to prevent denial of service
        m_state = IsoTpState::Receiving;

        IsoTransportProtocolFrame flowControlFrame(m_sourceAddress, 0, 0, 0, paddingSize() );

        if ( flowControlFrame.isValid() )
        {
            m_canInterface->sendFrame( flowControlFrame.canFrame() );
        }

        m_nextExpectedSequenceNumber = 1;
        m_expectedDataLength = tpFrame.dataLength();
        m_receiveBuffer = tpFrame.data();
    }

    void IsoTransportProtocol::consecutiveFrameReceived(IsoTransportProtocolFrame &tpFrame)
    {
        if ( tpFrame.sequenceNumber() != m_nextExpectedSequenceNumber )
        {
            qCritical(LOG_TAG) << "Received unexpected sequence number";

            emit errorOccurred(IsoTpError::OutOfOrderData);

            resetState();

            return;
        }

        // current received bytes + maximum bytes of one consecutive frame
        if ( (m_receiveBuffer.size() + 7) < m_expectedDataLength )
        {
            m_nextExpectedSequenceNumber = (m_nextExpectedSequenceNumber + 1) % 16;
            m_receiveBuffer.append( tpFrame.data() );
        }
        else
        {
            // this is the last frame

            int remainingBytes = m_expectedDataLength - m_receiveBuffer.size();

            QByteArray remainingData = tpFrame.data();

            if ( remainingData.size() < remainingBytes )
            {
                // last frame does not contain all expected remaining data

                qCritical(LOG_TAG) << "Unexpected end of data stream";
                emit errorOccurred(IsoTpError::DataError);

                resetState();

                return;
            }

            // remove possible appended padding
            m_receiveBuffer.append( remainingData.mid(0, remainingBytes) );

            QByteArray receivedData = m_receiveBuffer;

            // calling reset before sending signal to allow recursive calls to IsoTransportProtocol in slots
            resetState();

            emit dataReceived( receivedData );
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

    void IsoTransportProtocol::resetState()
    {
        m_nextExpectedSequenceNumber = 0;
        m_expectedDataLength = 0;
        m_receiveBuffer.clear();

        m_state = IsoTpState::Idle;
    }
}
