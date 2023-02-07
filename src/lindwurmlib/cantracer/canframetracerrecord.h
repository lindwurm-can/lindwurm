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

#ifndef CANFRAMETRACERRECORD_H
#define CANFRAMETRACERRECORD_H

#include "lindwurmlib_global.h"
#include <QCanBusFrame>

namespace Lindwurm::Lib
{
    /**
     * @brief The CanFrameTracerRecord class represents a record for a single CAN frame in a trace log.
     *
     * It stores additional information about the captured CAN frame in the trace log, for example
     * the time difference to the last CAN frame with this ID or the source interface of the frame.
     *
     * WIP: The hamming distance shows the payload changes over time.
     */
    class LINDWURMLIB_EXPORT CanFrameTracerRecord
    {
        public:

            /**
             * @brief Constructs a CanFrameTracerRecord with the provided data.
             * @param frame                 the traced frame.
             * @param timeDifferenceUSecs   the time difference since last corresponding frame in µs.
             * @param hammingDistance       the hamming distance of the payload bytes.
             */
            CanFrameTracerRecord(const QCanBusFrame &frame, qint64 timeDifferenceUSecs, int hammingDistance, const QString &sourceInterface);

            /**
             * @brief Returns the captured CAN frame.
             * @return the captured CAN frame.
             */
            const QCanBusFrame&     canFrame() const;

            /**
             * @brief Returns the time difference to the last frame with this frame ID.
             * @return the time difference in µs.
             */
            qint64                  timeDifferenceUSecs() const;

            /**
             * @brief Returns the hamming distance to the last frame with this frame ID.
             * @return the hamming distance.
             */
            int                     hammingDistance() const;

            /**
             * @brief Returns the name of the interface from which the frame was captured.
             * @return the name of the interface from which the frame was captured.
             */
            QString                 sourceInterface() const;


        private:

            QCanBusFrame    m_frame;
            qint64          m_timeDifferenceUSecs;
            int             m_hammingDistance;
            QString         m_sourceInterface;
    };
}

#endif // CANFRAMETRACERRECORD_H
