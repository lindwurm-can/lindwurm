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

#ifndef CANFRAMEENUMERATOR_H
#define CANFRAMEENUMERATOR_H

#include "lindwurmlib_global.h"

#include "utils/bytearrayenumerator.h"

#include <qglobal.h>
#include <QByteArray>
#include <QCanBusFrame>

namespace Lindwurm::Lib
{
    /**
     * @brief The CanFrameEnumerator class allows enumerating over a range of CAN frames and payloads.
     *
     * This class allows to enumerate over a set of CAN frames defined by a textual representation. The CAN frames
     * can either be static where each line of text defines a CAN frame:
     *
     * `
     * 7F0 00 11 22
     * 7F1 00 11 22
     * `
     */
    class LINDWURMLIB_EXPORT CanFrameEnumerator
    {
        public:

            CanFrameEnumerator();

            /**
             * @brief Returns the total frame count in the sequence.
             * @return total frame count in the sequence.
             */
            quint64                 frameCount() const;

            /**
             * @brief Returns the current frame counter.
             * @return the current frame counter.
             */
            quint64                 currentCount() const;

            /**
             * @brief Returns true if there is at least one frame remaining.
             * @return `true` if there is at least one frame remaining; otherwise `false`
             */
            bool                    hasNext() const;

            /**
             * @brief Returns the next available CAN frame in the sequence.
             * @return the next available CAN frame in the sequence; if no frame is left an invalid frame.
             */
            QCanBusFrame            next();

            /**
             * @brief Parses the textual definition and initializes the CanFrameEnumerator.
             * @param textFrame the frame definition to be enumerated.
             * @return `true` if the definition was parsed successfully and the enumerator is valid; otherwise `false`.
             */
            bool                    parseFrame(QString textFrame);

            /**
             * @brief Resets the enumerator back to the first CAN frame.
             */
            void                    reset();


        private:

            bool                    parseFrameID(const QString& idElement);
            bool                    parseStaticPayload(const QStringList& payloadElements);

            bool                    m_isValid = {false};

            quint32                 m_startFrameID = {0};
            quint32                 m_endFrameID = {0};
            quint32                 m_currentFrameID = {0};
            bool                    m_hasStaticPayload = {true};
            QByteArray              m_staticPayload = {};
            ByteArrayEnumerator     m_dynamicPayload = {};
    };
}

#endif // CANFRAMEENUMERATOR_H
