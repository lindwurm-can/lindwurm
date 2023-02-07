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

#ifndef CANFRAMETRACER_H
#define CANFRAMETRACER_H

#include "lindwurmlib_global.h"

#include <QObject>
#include <QString>
#include <QVector>
#include <QRecursiveMutex>
#include <QMap>

#include "cantracer/canframetracerrecord.h"
#include "cantracer/canframeaggregator.h"
#include "caninterface/icaninterfacehandlesharedptr.h"

namespace Lindwurm::Lib
{
    /**
     * @brief The CanFrameTracer allows to create a trace log of a CAN interface.
     */
    class LINDWURMLIB_EXPORT CanFrameTracer : public QObject
    {
        Q_OBJECT
        public:

            CanFrameTracer(QObject *parent = nullptr);

            void                    mountCANInterface(ICanInterfaceHandleSharedPtr interface);
            void                    unmountCANInterface();

            /**
             * @brief Start capturing.
             */
            void                    start(void);

            /**
             * @brief Stop capturing.
             */
            void                    stop(void);

            /**
             * @brief Returns the start time of the trace.
             * @return the start time of the trace in microseconds.
             */
            qint64                  startTime() const;

            int                     frameRecordCount() const;
            CanFrameTracerRecord    frameRecordAt(int index) const;

            int                     aggregateRecordCount() const;
            CanFrameAggregator      aggregateRecordAt(int index) const;

        signals:

            void                    frameRecordInserted();
            void                    aggregateRecordInserted();
            void                    aggregateRecordUpdated(int aggregateRecordIndex);

        private slots:

            void                    canFrameReceived(const QCanBusFrame &frame, const QString &sourceInterface);
            void                    initializeStartTimeFromFirstFrame();

        private:

            ICanInterfaceHandleSharedPtr    m_canInterface = {};
            bool                            m_isRunning = { false };
            qint64                          m_traceStartTimeMicroSeconds = { 0 };
            QVector<CanFrameTracerRecord>   m_frameRecords = {};
            mutable QRecursiveMutex         m_frameRecordsMutex = {};

            QVector<CanFrameAggregator>     m_aggregators = {};
            QMap<quint32, int>              m_frameIdToAggregatorIndex = {};
            mutable QRecursiveMutex         m_aggregatorsMutex = {};
    };
}

#endif // CANFRAMETRACER_H
