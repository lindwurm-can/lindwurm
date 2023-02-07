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
    IsoTransportProtocolFrame::IsoTransportProtocolFrame(const QCanBusFrame &canFrame, QObject *parent)
        : QObject(parent)
        , m_canFrame(canFrame)
    {
        if ( m_canFrame.payload().size() < 1)
        {
            return;
        }

        unsigned char type = (m_canFrame.payload().at(0) & 0xF0) >> 4;

        switch (type)
        {
            case 0:     m_frameType = IsoTpFrameType::SingleFrame;       break;
            case 1:     m_frameType = IsoTpFrameType::FirstFrame;        break;
            case 2:     m_frameType = IsoTpFrameType::ConsecutiveFrame;  break;
            case 3:     m_frameType = IsoTpFrameType::FlowControlFrame;  break;
            default:    m_frameType = IsoTpFrameType::Invalid;           return;
        }

    }

    IsoTransportProtocolFrame::IsoTransportProtocolFrame(quint32 frameId, const QByteArray &data, QObject *parent)
        : QObject(parent)
        , m_canFrame()
    {
        if ( data.size() > 7 )
        {
            m_frameType = IsoTpFrameType::Invalid;
        }

        m_frameType = IsoTpFrameType::SingleFrame;

        QByteArray payload;

        payload.push_front( static_cast<char>( data.size() ) );
        payload.push_back( data );

        m_canFrame.setPayload(payload);
        m_canFrame.setFrameId(frameId);
    }

    IsoTransportProtocolFrame::IsoTransportProtocolFrame(quint32 frameId, int flowStatus, int blockSize, int separationTime, int paddingSize, QObject *parent)
        : QObject(parent)
        , m_canFrame()
    {
        if ( (flowStatus > 2) || (blockSize > 255) || (separationTime > 255) )
        {
            m_frameType = IsoTpFrameType::Invalid;
            return;
        }

        m_frameType = IsoTpFrameType::FlowControlFrame;

        QByteArray payload;

        payload.append( 0x30 + flowStatus );
        payload.append( static_cast<unsigned char>(blockSize) );
        payload.append( static_cast<unsigned char>(separationTime) );

        if (paddingSize > 0)
        {
            int neededPadding = paddingSize - 3;

            for (int i = 0; i < neededPadding; i++)
            {
                payload.append('\x00');
            }
        }

        m_canFrame.setFrameId(frameId);
        m_canFrame.setPayload(payload);
    }

    IsoTransportProtocolFrame::IsoTpFrameType IsoTransportProtocolFrame::frameType() const
    {
        return m_frameType;
    }

    QCanBusFrame IsoTransportProtocolFrame::canFrame() const
    {
        return m_canFrame;
    }

    bool IsoTransportProtocolFrame::isValid() const
    {
        return m_frameType != IsoTpFrameType::Invalid;
    }

    int IsoTransportProtocolFrame::dataLength() const
    {
        switch ( m_frameType )
        {
            case IsoTpFrameType::SingleFrame:

                return (m_canFrame.payload().at(0) & 0x0F);

            case IsoTpFrameType::FirstFrame:
            {
                char msb = m_canFrame.payload().at(0) & 0x0F;
                char lsb = m_canFrame.payload().at(1);

                return (static_cast<quint16>(msb) << 8) | (static_cast<quint16>(lsb) );
            }

            case IsoTpFrameType::ConsecutiveFrame:

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
        if (m_frameType == IsoTpFrameType::FirstFrame || m_frameType == IsoTpFrameType::ConsecutiveFrame)
        {
            if ( m_canFrame.payload().size() > 0 )
            {
                return m_canFrame.payload().at(0) & 0x0F;
            }
        }

        return 0;
    }

    int IsoTransportProtocolFrame::flowStatus() const
    {
        if ( m_frameType == IsoTpFrameType::FlowControlFrame)
        {
            if ( m_canFrame.payload().size() > 0 )
            {
                return m_canFrame.payload().at(0) & 0x0F;
            }
        }

        return 0;
    }

    int IsoTransportProtocolFrame::blockSize() const
    {
        return 0;
    }

    int IsoTransportProtocolFrame::separationTime() const
    {
        return 0;
    }

    QByteArray IsoTransportProtocolFrame::data() const
    {
        switch ( m_frameType )
        {
            case IsoTpFrameType::SingleFrame:

                return m_canFrame.payload().mid(1, dataLength() );

            case IsoTpFrameType::FirstFrame:

                return m_canFrame.payload().mid(2, -1);

            case IsoTpFrameType::ConsecutiveFrame:

                return m_canFrame.payload().mid(1, -1);

            default:
                break;
        }

        return QByteArray();
    }
}
