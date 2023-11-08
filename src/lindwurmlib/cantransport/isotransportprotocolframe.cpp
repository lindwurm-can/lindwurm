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

#include "cantransport/isotransportprotocolframe.h"

namespace Lindwurm::Lib
{
    IsoTransportProtocolFrame::IsoTransportProtocolFrame()
    {
        // constructs an invalid protocol frame
    }

    IsoTransportProtocolFrame::IsoTransportProtocolFrame(const QCanBusFrame &canFrame)
        : m_canFrame(canFrame)
    {
        if ( m_canFrame.payload().size() < 1)
        {
            return;
        }

        unsigned char type = (m_canFrame.payload().at(0) & 0xF0) >> 4;

        switch (type)
        {
            case 0:
                        if ( m_canFrame.payload().size() < 2)   return;
                        m_frameType = Type::SingleFrame;
                        break;

            case 1:
                        if ( m_canFrame.payload().size() < 8)   return;
                        m_frameType = Type::FirstFrame;
                        break;

            case 2:
                        if ( m_canFrame.payload().size() < 2)   return;
                        m_frameType = Type::ConsecutiveFrame;
                        break;

            case 3:
                        if ( m_canFrame.payload().size() < 3)   return;
                        m_frameType = Type::FlowControlFrame;
                        break;

            default:    m_frameType = Type::Invalid;            return;
        }

    }

    IsoTransportProtocolFrame IsoTransportProtocolFrame::fromRawCanFrame(const QCanBusFrame &canFrame)
    {
        return IsoTransportProtocolFrame(canFrame);
    }

    IsoTransportProtocolFrame IsoTransportProtocolFrame::singleFrame(quint32 frameId, const QByteArray &data, int paddingSize)
    {
        if ( (data.size() > 7) || (data.size() < 1) )
        {
            return IsoTransportProtocolFrame();
        }

        QByteArray payload;

        payload.push_front( static_cast<char>( data.size() ) );
        payload.push_back( data );

        if (paddingSize > 0)
        {
            appendPadding(payload, paddingSize);
        }

        QCanBusFrame canFrame;

        canFrame.setFrameId(frameId);
        canFrame.setPayload(payload);

        return IsoTransportProtocolFrame(canFrame);
    }

    IsoTransportProtocolFrame IsoTransportProtocolFrame::firstFrame(quint32 frameId, int totalDataLength, const QByteArray &data)
    {
        if ( (totalDataLength > 4096) || (data.size() > 6) )
        {
            return IsoTransportProtocolFrame();
        }

        QByteArray payload;

        payload.append( 0x10 + ( (totalDataLength >> 8) & 0x0F) );
        payload.append( totalDataLength & 0xFF );
        payload.append( data );

        QCanBusFrame canFrame;

        canFrame.setFrameId(frameId);
        canFrame.setPayload(payload);

        return IsoTransportProtocolFrame(canFrame);
    }

    IsoTransportProtocolFrame IsoTransportProtocolFrame::consecutiveFrame(quint32 frameId, quint8 sequenceNumber, const QByteArray &data, int paddingSize)
    {
        if ( (data.size() < 1) || (data.size() > 7) || (sequenceNumber > 15) )
        {
            return IsoTransportProtocolFrame();
        }

        QByteArray payload;

        payload.append( 0x20 + ( sequenceNumber & 0x0F) );
        payload.append( data );

        appendPadding(payload, paddingSize);

        QCanBusFrame canFrame;

        canFrame.setFrameId(frameId);
        canFrame.setPayload(payload);

        return IsoTransportProtocolFrame(canFrame);
    }

    IsoTransportProtocolFrame IsoTransportProtocolFrame::flowControlFrame(quint32 frameId, IsoTpFlowStatus flowStatus, int blockSize, int separationTime, int paddingSize)
    {
        if ( (blockSize > 255) || (separationTime > 255) )
        {
            return IsoTransportProtocolFrame();
        }

        QByteArray payload;

        payload.append( 0x30 + flowStatusToNumber(flowStatus) );
        payload.append( static_cast<unsigned char>(blockSize) );
        payload.append( static_cast<unsigned char>(separationTime) );

        if (paddingSize > 0)
        {
            appendPadding(payload, paddingSize);
        }

        QCanBusFrame canFrame;

        canFrame.setFrameId(frameId);
        canFrame.setPayload(payload);

        return IsoTransportProtocolFrame(canFrame);
    }

    IsoTransportProtocolFrame::Type IsoTransportProtocolFrame::frameType() const
    {
        return m_frameType;
    }

    QCanBusFrame IsoTransportProtocolFrame::canFrame() const
    {
        return m_canFrame;
    }

    bool IsoTransportProtocolFrame::isValid() const
    {
        return m_frameType != Type::Invalid;
    }

    int IsoTransportProtocolFrame::dataLength() const
    {
        switch ( m_frameType )
        {
            case Type::SingleFrame:

                return (m_canFrame.payload().at(0) & 0x0F);

            case Type::FirstFrame:
            {
                quint8 msb = m_canFrame.payload().at(0) & 0x0F;
                quint8 lsb = m_canFrame.payload().at(1);

                return (static_cast<quint16>(msb) << 8) | (static_cast<quint16>(lsb) );
            }

            case Type::ConsecutiveFrame:

                // consecutive frames usually have 7 bytes data (8 byte CAN frame -1 byte for PCI)
                // last consecutive frame may have fewer bytes (if no padding is used): payload length -1 byte for PCI
                // if padding is used dataLength will include this padding bytes - must be cut according to dataLength from first frame
                return m_canFrame.payload().size() - 1;

            default:

                break;
        }

        return 0;
    }

    int IsoTransportProtocolFrame::sequenceNumber() const
    {
        if (m_frameType == Type::FirstFrame || m_frameType == Type::ConsecutiveFrame)
        {
            if ( m_canFrame.payload().size() > 0 )
            {
                return m_canFrame.payload().at(0) & 0x0F;
            }
        }

        return 0;
    }

    IsoTpFlowStatus IsoTransportProtocolFrame::flowStatus() const
    {
        if ( m_frameType == Type::FlowControlFrame)
        {
            if ( m_canFrame.payload().size() > 0 )
            {
                return numberToFlowStatus( m_canFrame.payload().at(0) & 0x0F );
            }
        }

        return IsoTpFlowStatus::Overflow;
    }

    quint8 IsoTransportProtocolFrame::blockSize() const
    {
        if ( m_frameType == Type::FlowControlFrame)
        {
            if ( m_canFrame.payload().size() > 1 )
            {
                return m_canFrame.payload().at(1);
            }
        }

        return 0;
    }

    quint8 IsoTransportProtocolFrame::separationTime() const
    {
        if ( m_frameType == Type::FlowControlFrame)
        {
            if ( m_canFrame.payload().size() > 2 )
            {
                return m_canFrame.payload().at(2);
            }
        }

        return 0;
    }

    QByteArray IsoTransportProtocolFrame::data() const
    {
        switch ( m_frameType )
        {
            case Type::SingleFrame:

                return m_canFrame.payload().mid(1, dataLength() );

            case Type::FirstFrame:

                return m_canFrame.payload().mid(2, -1);

            case Type::ConsecutiveFrame:

                return m_canFrame.payload().mid(1, -1);

            default:
                break;
        }

        return QByteArray();
    }

    quint8 IsoTransportProtocolFrame::flowStatusToNumber(IsoTpFlowStatus status)
    {
        switch (status)
        {
            case IsoTpFlowStatus::ClearToSend:  return 0;
            case IsoTpFlowStatus::Wait:         return 1;
            case IsoTpFlowStatus::Overflow:     return 2;
        }

        return 0;
    }

    IsoTpFlowStatus IsoTransportProtocolFrame::numberToFlowStatus(quint8 status)
    {
        switch (status)
        {
            case 0: return IsoTpFlowStatus::ClearToSend;
            case 1: return IsoTpFlowStatus::Wait;
            case 2: return IsoTpFlowStatus::Overflow;
        }

        // > 2 =>   undefined behavior
        return IsoTpFlowStatus::Overflow;
    }

    void IsoTransportProtocolFrame::appendPadding(QByteArray &payload, int paddingSize)
    {
        int neededPadding = paddingSize - payload.size();

        for (int i = 0; i < neededPadding; i++)
        {
            payload.append('\x00');
        }
    }
}
