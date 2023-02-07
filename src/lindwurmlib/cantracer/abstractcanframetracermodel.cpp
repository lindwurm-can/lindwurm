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

#include "cantracer/abstractcanframetracermodel.h"
#include "cantracer/canframetracer.h"

namespace
{
    const int BASE_10 = 10;
}

namespace Lindwurm::Lib
{
    AbstractCanFrameTracerModel::AbstractCanFrameTracerModel(CanFrameTracer *tracer, QObject *parent)
        : QAbstractTableModel(parent)
        , m_tracer(tracer)
    {

    }

    QString AbstractCanFrameTracerModel::getFrameTime(const QCanBusFrame::TimeStamp &frameTimestamp) const
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

    QString AbstractCanFrameTracerModel::getFrameTimeDiff(qint64 timeDiffMicroSeconds) const
    {
        qint64 decimal = timeDiffMicroSeconds % 1000;

        // should actually never happen as timestamps from frames are increasing in general
        // but handle it, in case of a corrupt can tracer log file with an unplausible timestamp
        if ( decimal < 0 )
        {
            // as we output the decimal plac separately, avoid a leading minus sign
            decimal = decimal * -1;
        }

        return QString("%1.%2 ms").arg( timeDiffMicroSeconds / 1000, 7, BASE_10, QLatin1Char(' ') ).arg( decimal, 3, BASE_10, QLatin1Char('0') );
    }

    QString AbstractCanFrameTracerModel::toASCIIString(const QByteArray &data) const
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
