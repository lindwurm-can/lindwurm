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

#include "cantracer/canframetracer.h"
#include "caninterface/icaninterfacehandle.h"

#include <QDateTime>
#include <QMutexLocker>

namespace Lindwurm::Lib
{
    CanFrameTracer::CanFrameTracer(QObject *parent) : QObject(parent)
    {

    }

    void CanFrameTracer::mountCANInterface(ICanInterfaceHandleSharedPtr interface)
    {
        stop();

        unmountCANInterface();

        m_canInterface = interface;
    }

    void CanFrameTracer::unmountCANInterface()
    {
        if ( m_canInterface )
        {
            m_canInterface->unmount();
            m_canInterface.reset();
        }
    }

    void CanFrameTracer::start()
    {
        if ( (m_canInterface) && (m_isRunning == false) )
        {
            m_isRunning = true;
            m_traceStartTimeMicroSeconds = QDateTime::currentMSecsSinceEpoch() * 1000;

            // adding a time buffer from 1 ms to compensate a deviation (first received frames may have a timestamp little earlier than current timestamp)
            // which could result in negative frame times
            m_traceStartTimeMicroSeconds = m_traceStartTimeMicroSeconds - 1000;

            connect(m_canInterface.get(), &ICanInterfaceHandle::frameReceived, this, &CanFrameTracer::canFrameReceived);
        }
    }

    void CanFrameTracer::stop()
    {
        if ( (m_canInterface) && (m_isRunning == true) )
        {
            disconnect(m_canInterface.get(), &ICanInterfaceHandle::frameReceived, this, &CanFrameTracer::canFrameReceived);
        }

        m_isRunning = false;
    }

    qint64 CanFrameTracer::startTime() const
    {
        return m_traceStartTimeMicroSeconds;
    }

    int CanFrameTracer::frameRecordCount() const
    {
        QMutexLocker locker( &m_frameRecordsMutex );

        return m_frameRecords.size();
    }

    CanFrameTracerRecord CanFrameTracer::frameRecordAt(int index) const
    {
        QMutexLocker locker( &m_frameRecordsMutex );

        return m_frameRecords.at(index);
    }

    int CanFrameTracer::aggregateRecordCount() const
    {
        QMutexLocker locker( &m_aggregatorsMutex );

        return m_aggregators.size();
    }

    CanFrameAggregator CanFrameTracer::aggregateRecordAt(int index) const
    {
        QMutexLocker locker( &m_aggregatorsMutex );

        return m_aggregators.at(index);
    }

    void CanFrameTracer::canFrameReceived(const QCanBusFrame &frame, const QString &sourceInterface)
    {
        // TODO: BugFix (negative trace times)
        // Check if received frame timestamp is earlier than _traceStartTimeMicroSeconds
        // and correct _traceStartTimeMicroSeconds accordingly

        QMutexLocker aggregatorsLocker( &m_aggregatorsMutex );
        QMutexLocker frameLocker( &m_frameRecordsMutex );

            bool newAggregateInserted = false;

            if ( ! m_frameIdToAggregatorIndex.contains( frame.frameId() ) )
            {
                // we have identified a new distinct frame id and create an aggregator for it

                CanFrameAggregator aggregator( frame.frameId() );

                m_aggregators.append(aggregator);

                // map the frame id to the aggregate record's index
                m_frameIdToAggregatorIndex.insert( frame.frameId(), m_aggregators.size() - 1 );

                // remind to emit aggregateInserted signal after all data is updated
                newAggregateInserted = true;
            }

            int aggregatorIndex = m_frameIdToAggregatorIndex.value(frame.frameId(), -1);

            Q_ASSERT(aggregatorIndex != -1);

            qint64 timeDiffToLastCorrespondingFrameUSecs = 0;

            qint64 timeOfLastCorrespondingFrameUSecs = m_aggregators.at(aggregatorIndex).latestTimestampUSecs();
            qint64 timestampOfCurrentFrameUSecs = frame.timeStamp().seconds() * 1000000 + frame.timeStamp().microSeconds();

            if ( timeOfLastCorrespondingFrameUSecs != 0 )
            {
                timeDiffToLastCorrespondingFrameUSecs = timestampOfCurrentFrameUSecs - timeOfLastCorrespondingFrameUSecs;
            }

            // insert current frame to frame records
            m_frameRecords.append( CanFrameTracerRecord(frame, timeDiffToLastCorrespondingFrameUSecs, 0, sourceInterface) );

            // append current frame to aggregate record
            m_aggregators[aggregatorIndex].appendFrameRecord( m_frameRecords.size() - 1, timestampOfCurrentFrameUSecs, timeDiffToLastCorrespondingFrameUSecs );

        frameLocker.unlock();
        aggregatorsLocker.unlock();

        if ( newAggregateInserted )
        {
            emit aggregateRecordInserted();
        }
        else
        {
            emit aggregateRecordUpdated(aggregatorIndex);
        }

        emit frameRecordInserted();
    }

    void CanFrameTracer::initializeStartTimeFromFirstFrame()
    {
        if ( m_frameRecords.size() > 0 )
        {
            CanFrameTracerRecord record = m_frameRecords.at(0);

            m_traceStartTimeMicroSeconds = (record.canFrame().timeStamp().seconds() * 1000000) + record.canFrame().timeStamp().microSeconds();
        }
    }
}
