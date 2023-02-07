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

#ifndef ABSTRACTCANFRAMETRACERMODEL_H
#define ABSTRACTCANFRAMETRACERMODEL_H

#include <QAbstractTableModel>
#include <QCanBusFrame>

namespace Lindwurm::Lib
{
    class CanFrameTracer;

    class AbstractCanFrameTracerModel : public QAbstractTableModel
    {
        Q_OBJECT
        public:

            explicit            AbstractCanFrameTracerModel(CanFrameTracer *tracer, QObject *parent = nullptr);

            enum
            {
                CopyTextRole = Qt::UserRole + 1
            };

        protected:

            QString             getFrameTime(const QCanBusFrame::TimeStamp &frameTimestamp) const;
            QString             getFrameTimeDiff(qint64 timeDiffMicroSeconds) const;
            QString             toASCIIString(const QByteArray &data) const;

        protected:

            CanFrameTracer*     m_tracer = { nullptr };
    };
}

#endif // ABSTRACTCANFRAMETRACERMODEL_H
