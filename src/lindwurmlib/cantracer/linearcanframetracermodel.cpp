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

#include "cantracer/linearcanframetracermodel.h"

#include "cantracer/canframetracer.h"

#include <QSize>

namespace
{
    const int ViewUpdateInterval = 100;
    const int BASE_10 = 10;
}

namespace Lindwurm::Lib
{
    LinearCanFrameTracerModel::LinearCanFrameTracerModel(CanFrameTracer *tracer, QObject *parent)
        : AbstractCanFrameTracerModel(tracer, parent)
        , m_rowCount( m_tracer->frameRecordCount() )
        , m_insertedRowsCount(0)
        , m_updateTimer()
    {
        m_updateTimer.setSingleShot(true);
        connect(&m_updateTimer, &QTimer::timeout, this, &LinearCanFrameTracerModel::updateModel);

        connect(m_tracer, &CanFrameTracer::frameRecordInserted, this, &LinearCanFrameTracerModel::frameRecordInserted);
    }

    int LinearCanFrameTracerModel::rowCount(const QModelIndex &parent) const
    {
        if ( ! parent.isValid() )
        {
            return m_rowCount;
        }

        return 0;
    }

    int LinearCanFrameTracerModel::columnCount(const QModelIndex &parent) const
    {
        if ( ! parent.isValid() )
        {
            return 9;
        }

        return 0;
    }

    QVariant LinearCanFrameTracerModel::data(const QModelIndex &index, int role) const
    {
        if (index.isValid() && role == Qt::DisplayRole)
        {
            CanFrameTracerRecord record = m_tracer->frameRecordAt( index.row() );

            switch ( index.column() )
            {
                case 0:     return index.row() + 1;
                case 1:     return getFrameTime( record.canFrame().timeStamp() );
                case 2:     return QString("%1").arg( record.canFrame().frameId(), 3, 16, QLatin1Char(' ') ).toUpper();
                case 3:     return getFrameTimeDiff( record.timeDifferenceUSecs() );
                case 4:     return record.canFrame().hasLocalEcho() ? "TX" : "RX";
                case 5:     return record.sourceInterface();
                case 6:     return record.canFrame().payload().size();
                case 7:     return record.canFrame().payload().toHex(' ').toUpper();
                case 8:     return toASCIIString( record.canFrame().payload() );
                default:    return QVariant();
            }
        }

        if ( index.isValid() && role == CopyTextRole )
        {
            CanFrameTracerRecord record = m_tracer->frameRecordAt( index.row() );

            return QString("%1").arg( record.canFrame().frameId(), 3, 16, QLatin1Char(' ') ).toUpper() + "\t" + record.canFrame().payload().toHex(' ').toUpper() + "\t\t# " + toASCIIString( record.canFrame().payload() );
        }

        return QVariant();
    }

    QVariant LinearCanFrameTracerModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if ( role == Qt::DisplayRole && orientation == Qt::Orientation::Horizontal )
        {
            switch (section)
            {
                case 0:     return "No.";
                case 1:     return "Time";
                case 2:     return "ID";
                case 3:     return "Time Diff";
                case 4:     return "Dir";
                case 5:     return "Interface";
                case 6:     return "Length";
                case 7:     return "Data";
                case 8:     return "ASCII";
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
                case 4:     return QSize(50, 24);
                case 5:     return QSize(90, 24);
                case 6:     return QSize(70, 24);
                case 7:     return QSize(230, 24);
                default:    return QVariant();
            }
        }

        return QVariant();
    }

    void LinearCanFrameTracerModel::frameRecordInserted()
    {
        m_insertedRowsCount++;

        if ( ! m_updateTimer.isActive() )
        {
            m_updateTimer.start(ViewUpdateInterval);
        }
    }

    void LinearCanFrameTracerModel::updateModel()
    {
        // existing rows are numbered from 0 (!) to _rowCount-1
        // first and last are the row numbers that the new rows will have after they have been inserted.
        // e.g. if 2 rows are inserted they will have the numbers _rowCount and _rowCount + 1 // 2-1
        // if only 1 row is inserted it will have the number _rowCount, hence first/last are equal
        beginInsertRows( QModelIndex(), m_rowCount, m_rowCount + (m_insertedRowsCount-1) );

        m_rowCount += m_insertedRowsCount;
        m_insertedRowsCount = 0;

        endInsertRows();
    }

    QString LinearCanFrameTracerModel::getFrameTime(const QCanBusFrame::TimeStamp &frameTimestamp) const
    {
        qint64 timestampMicroSeconds = frameTimestamp.seconds() * 1000000 + frameTimestamp.microSeconds();
        qint64 timeSinceTracerStartMicroSeconds = timestampMicroSeconds - m_tracer->startTime();

        qint64 timeSinceTracerStartMicroSecondsDecimal = timeSinceTracerStartMicroSeconds % 1000000;

        // should actually never happen as timestamps from frames are increasing in general
        // but handle it, in case of a corrupt can tracer log file with an unplausible timestamp
        if ( timeSinceTracerStartMicroSecondsDecimal < 0 )
        {
            // as we output the decimal plac separately, avoid a leading minus sign
            timeSinceTracerStartMicroSecondsDecimal = timeSinceTracerStartMicroSecondsDecimal * -1;
        }

        return QString("%1.%2 s").arg( timeSinceTracerStartMicroSeconds / 1000000, 4, BASE_10, QLatin1Char(' ') ).arg(timeSinceTracerStartMicroSecondsDecimal , 6, BASE_10, QLatin1Char('0') );
    }

    QString LinearCanFrameTracerModel::getFrameTimeDiff(qint64 timeDiffMicroSeconds) const
    {
        qint64 decimal = timeDiffMicroSeconds % 1000;

        // should actually never happen as timestamps from frames are increasing in general
        // but handle it, in case of a corrupt can tracer log file with an unplausible timestamp
        if ( decimal < 0 )
        {
            // as we output the decimal plac separately, avoid a leading minus sign
            decimal = decimal * -1;
        }

        return QString("%1.%2 ms").arg( timeDiffMicroSeconds / 1000, 7, BASE_10, QLatin1Char(' ') ).arg(decimal , 3, BASE_10, QLatin1Char('0') );
    }

    QString LinearCanFrameTracerModel::toASCIIString(const QByteArray &data) const
    {
        QString asciiString;
        int length = data.length();
        QChar dataChar;

        for (int i = 0; i < length; i++)
        {
            dataChar = QChar(data.at(i));
            if ( dataChar.isPrint() )
                asciiString.append( QString( dataChar ) );
            else
                asciiString.append( "." );
        }

        return asciiString;
    }
}
