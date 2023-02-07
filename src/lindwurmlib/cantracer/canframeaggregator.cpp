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

#include "canframeaggregator.h"

namespace Lindwurm::Lib
{
    CanFrameAggregator::CanFrameAggregator(quint32 frameId)
        : m_frameId(frameId)
        , m_frameRecordIndices()
        , m_latestFrameTimestampUSecs(0)
        , m_averageTimeIntervalUSecs(0)
    {

    }

    CanFrameAggregator::~CanFrameAggregator()
    {
    }

    quint32 CanFrameAggregator::frameId() const
    {
        return m_frameId;
    }

    void CanFrameAggregator::appendFrameRecord(int frameRecordIndex, qint64 timestampUSecs, qint64 timeDifferenceUSecs)
    {
        m_frameRecordIndices.append(frameRecordIndex);
        m_latestFrameTimestampUSecs = timestampUSecs;

        // update the average difference with the new frame
        m_averageTimeIntervalUSecs = m_averageTimeIntervalUSecs + ( ( float(timeDifferenceUSecs) - m_averageTimeIntervalUSecs) / m_frameRecordIndices.size() );
    }

    int CanFrameAggregator::frameRecordCount() const
    {
        return m_frameRecordIndices.size();
    }

    int CanFrameAggregator::latestFrameRecordIndex() const
    {
        return m_frameRecordIndices.last();
    }

    qint64 CanFrameAggregator::latestTimestampUSecs() const
    {
        return m_latestFrameTimestampUSecs;
    }

    double CanFrameAggregator::averageTimeIntervalUSecs() const
    {
        return m_averageTimeIntervalUSecs;
    }
}
