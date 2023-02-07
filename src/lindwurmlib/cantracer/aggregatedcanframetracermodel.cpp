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

#include "cantracer/aggregatedcanframetracermodel.h"

#include "cantracer/canframetracer.h"

#include <QSize>

namespace
{
    const int ViewUpdateInterval = 100;
}

namespace Lindwurm::Lib
{
    AggregatedCanFrameTracerModel::AggregatedCanFrameTracerModel(CanFrameTracer *tracer, QObject *parent)
        : AbstractCanFrameTracerModel(tracer, parent)
        , m_rowCount( m_tracer->aggregateRecordCount() )
        , m_updatedAggregateIndices()
        , m_updateTimer()
    {
        m_updateTimer.setSingleShot(true);
        connect(&m_updateTimer, &QTimer::timeout, this, &AggregatedCanFrameTracerModel::updateModel);

        connect(m_tracer, &CanFrameTracer::aggregateRecordInserted, this, &AggregatedCanFrameTracerModel::aggregateRecordInserted);
        connect(m_tracer, &CanFrameTracer::aggregateRecordUpdated,  this, &AggregatedCanFrameTracerModel::aggregateRecordUpdated);
    }

    int AggregatedCanFrameTracerModel::rowCount(const QModelIndex &parent) const
    {
        if ( ! parent.isValid() )
        {
            return m_rowCount;
        }

        return 0;
    }

    int AggregatedCanFrameTracerModel::columnCount(const QModelIndex &parent) const
    {
        if ( ! parent.isValid() )
        {
            return 11;
        }

        return 0;
    }

    QVariant AggregatedCanFrameTracerModel::data(const QModelIndex &index, int role) const
    {
        if (index.isValid() && role == Qt::DisplayRole)
        {
            CanFrameAggregator aggregate = m_tracer->aggregateRecordAt( index.row() );
            CanFrameTracerRecord record = m_tracer->frameRecordAt( aggregate.latestFrameRecordIndex() );

            switch ( index.column() )
            {
                case 0:     return index.row() + 1;
                case 1:     return getFrameTime( record.canFrame().timeStamp() );
                case 2:     return QString("%1").arg( record.canFrame().frameId(), 3, 16, QLatin1Char(' ') ).toUpper();
                case 3:     return getFrameTimeDiff( record.timeDifferenceUSecs() );
                case 4:     return getFrameTimeDiff( qint64( aggregate.averageTimeIntervalUSecs() ) );
                case 5:     return aggregate.frameRecordCount();
                case 6:     return record.canFrame().hasLocalEcho() ? "TX" : "RX";
                case 7:     return record.sourceInterface();
                case 8:     return record.canFrame().payload().size();
                case 9:     return record.canFrame().payload().toHex(' ').toUpper();
                case 10:    return toASCIIString( record.canFrame().payload() );
                default:    return QVariant();
            }
        }

        if ( index.isValid() && role == CopyTextRole )
        {
            CanFrameAggregator aggregate = m_tracer->aggregateRecordAt( index.row() );
            CanFrameTracerRecord record = m_tracer->frameRecordAt( aggregate.latestFrameRecordIndex() );

            return QString("%1").arg( record.canFrame().frameId(), 3, 16, QLatin1Char(' ') ).toUpper() + "\t" + record.canFrame().payload().toHex(' ').toUpper() + "\t\t# " + toASCIIString( record.canFrame().payload() );
        }

        return QVariant();
    }

    QVariant AggregatedCanFrameTracerModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if ( role == Qt::DisplayRole && orientation == Qt::Orientation::Horizontal )
        {
            switch (section)
            {
                case 0:     return "No.";
                case 1:     return "Time";
                case 2:     return "ID";
                case 3:     return "Time Diff";
                case 4:     return "Interval (avg)";
                case 5:     return "Count";
                case 6:     return "Dir";
                case 7:     return "Interface";
                case 8:     return "Length";
                case 9:     return "Data";
                case 10:     return "ASCII";
                default:    return QVariant();
            }
        }

        if ( role == Qt::SizeHintRole && orientation == Qt::Orientation::Horizontal )
        {
            switch (section)
            {
                case 0:     return QSize(100, 24);
                case 1:     return QSize(150, 24);
                case 2:     return QSize(50, 24);
                case 3:     return QSize(160, 24);
                case 4:     return QSize(180, 24);
                case 5:     return QSize(90, 24);
                case 6:     return QSize(50, 24);
                case 7:     return QSize(90, 24);
                case 8:     return QSize(70, 24);
                case 9:     return QSize(230, 24);
                default:    return QVariant();
            }
        }

        return QVariant();
    }

    void AggregatedCanFrameTracerModel::aggregateRecordInserted()
    {
        // existing rows are numbered from 0 (!) to _rowCount-1
        // first and last are the row numbers that the new rows will have after they have been inserted.
        // e.g. if 2 rows are inserted they will have the numbers _rowCount and _rowCount + 1 // 2-1
        // if only 1 row is inserted it will have the number _rowCount, hence first/last are equal
        beginInsertRows( QModelIndex(), m_rowCount, m_rowCount);

        m_rowCount += 1;

        endInsertRows();
    }

    void AggregatedCanFrameTracerModel::aggregateRecordUpdated(int aggregateRecordIndex)
    {
        m_updatedAggregateIndices.append(aggregateRecordIndex);

        if ( ! m_updateTimer.isActive() )
        {
            m_updateTimer.start(ViewUpdateInterval);
        }
    }

    void AggregatedCanFrameTracerModel::updateModel()
    {
        for (const int index : qAsConst(m_updatedAggregateIndices))
        {
            emit dataChanged( createIndex(index, 0),  createIndex(index, 8) );
        }

        m_updatedAggregateIndices.clear();
    }
}
