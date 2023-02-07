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
#include <QObject>
#include <QCanBusFrame>

namespace Lindwurm::Lib
{
    class LINDWURMLIB_EXPORT IsoTransportProtocolFrame : public QObject
    {
        Q_OBJECT
        public:

            enum class IsoTpFrameType
            {
                Invalid,
                SingleFrame,
                FirstFrame,
                ConsecutiveFrame,
                FlowControlFrame
            };

            explicit IsoTransportProtocolFrame(const QCanBusFrame& canFrame, QObject *parent = nullptr);
            explicit IsoTransportProtocolFrame(quint32 frameId, const QByteArray &data, QObject *parent = nullptr);
            explicit IsoTransportProtocolFrame(quint32 frameId, int flowStatus, int blockSize, int separationTime, int paddingSize = 0, QObject *parent = 0);

            IsoTpFrameType  frameType() const;
            QCanBusFrame    canFrame() const;
            bool            isValid() const;
            int             dataLength() const;
            int             sequenceNumber() const;
            int             flowStatus() const;
            int             blockSize() const;
            int             separationTime() const;
            QByteArray      data() const;

        private:

            IsoTpFrameType      m_frameType = { IsoTpFrameType::Invalid };
            QCanBusFrame        m_canFrame;
    };
}

#endif // ISOTRANSPORTPROTOCOLFRAME_H
