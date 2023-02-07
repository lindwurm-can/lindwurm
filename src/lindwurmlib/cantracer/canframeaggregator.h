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

#ifndef CANFRAMEAGGREGATOR_H
#define CANFRAMEAGGREGATOR_H

#include <qglobal.h>
#include <QVector>
#include <QCanBusFrame>

namespace Lindwurm::Lib
{
    class CanFrameAggregator
    {
        public:

            CanFrameAggregator(quint32 frameId);
            ~CanFrameAggregator();

            quint32     frameId(void) const;

            void        appendFrameRecord(int frameRecordIndex, qint64 timestampUSecs, qint64 timeDifferenceUSecs);
            int         frameRecordCount(void) const;
            int         latestFrameRecordIndex(void) const;

            qint64      latestTimestampUSecs(void) const;
            double      averageTimeIntervalUSecs(void) const;

        private:

            quint32         m_frameId;
            QVector<int>    m_frameRecordIndices;
            qint64          m_latestFrameTimestampUSecs;
            double          m_averageTimeIntervalUSecs;
    };
}

#endif // CANFRAMEAGGREGATOR_H
