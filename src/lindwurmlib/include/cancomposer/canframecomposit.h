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

#ifndef CANFRAMECOMPOSIT_H
#define CANFRAMECOMPOSIT_H

#include "lindwurmlib_global.h"
#include "canframeenumerator.h"

#include <QVector>

namespace Lindwurm::Lib
{
    class LINDWURMLIB_EXPORT CanFrameComposit
    {
        public:

            CanFrameComposit();

            bool                            parseFrameComposit(QString textFrames);

            quint64                         frameCount() const;
            quint64                         currentCount() const;

            bool                            hasNext() const;
            QCanBusFrame                    next();
            void                            reset();

        private:

            bool                            m_isValid = {false};
            quint64                         m_frameCount =  {0};
            quint64                         m_currentCount = {0};
            int                             m_currentFrameIndex = {0};

            QVector<CanFrameEnumerator>     m_frames;
    };
}

#endif // CANFRAMECOMPOSIT_H
