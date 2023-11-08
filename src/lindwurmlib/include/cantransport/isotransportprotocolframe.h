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

#ifndef ISOTRANSPORTPROTOCOLFRAME_H
#define ISOTRANSPORTPROTOCOLFRAME_H

#include "lindwurmlib_global.h"
#include <QCanBusFrame>

namespace Lindwurm::Lib
{
    enum class IsoTpFlowStatus
    {
        ClearToSend = 0,
        Wait        = 1,
        Overflow    = 2
    };

    class LINDWURMLIB_EXPORT IsoTransportProtocolFrame
    {
        public:

            enum class Type
            {
                Invalid,
                SingleFrame,
                FirstFrame,
                ConsecutiveFrame,
                FlowControlFrame
            };

            static      IsoTransportProtocolFrame fromRawCanFrame(const QCanBusFrame& canFrame);

            static      IsoTransportProtocolFrame singleFrame(quint32 frameId, const QByteArray &data, int paddingSize = 0);
            static      IsoTransportProtocolFrame firstFrame(quint32 frameId, int totalDataLength, const QByteArray &data);
            static      IsoTransportProtocolFrame consecutiveFrame(quint32 frameId, quint8 sequenceNumber, const QByteArray &data, int paddingSize = 0);
            static      IsoTransportProtocolFrame flowControlFrame(quint32 frameId, IsoTpFlowStatus flowStatus, int blockSize, int separationTime, int paddingSize = 0);

            Type            frameType() const;
            QCanBusFrame    canFrame() const;
            bool            isValid() const;
            int             dataLength() const;
            int             sequenceNumber() const;
            IsoTpFlowStatus flowStatus() const;
            quint8          blockSize() const;
            quint8          separationTime() const;
            QByteArray      data() const;

        private:

            explicit                IsoTransportProtocolFrame();
            explicit                IsoTransportProtocolFrame(const QCanBusFrame& canFrame);

            static quint8           flowStatusToNumber(IsoTpFlowStatus status);
            static IsoTpFlowStatus  numberToFlowStatus(quint8 status);
            static void             appendPadding(QByteArray &payload, int paddingSize);

        private:

            Type                m_frameType = { Type::Invalid };
            QCanBusFrame        m_canFrame;
    };
}

#endif // ISOTRANSPORTPROTOCOLFRAME_H
