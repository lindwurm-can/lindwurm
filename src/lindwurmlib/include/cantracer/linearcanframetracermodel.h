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

#ifndef LINEARCANFRAMETRACERMODEL_H
#define LINEARCANFRAMETRACERMODEL_H

#include "lindwurmlib_global.h"

#include "cantracer/abstractcanframetracermodel.h"
#include <QTimer>

namespace Lindwurm::Lib
{
    class CanFrameTracer;

    class LINDWURMLIB_EXPORT LinearCanFrameTracerModel : public AbstractCanFrameTracerModel
    {
        Q_OBJECT
        public:

            explicit LinearCanFrameTracerModel(CanFrameTracer* tracer, QObject *parent = nullptr);

            virtual int         rowCount(const QModelIndex &parent = QModelIndex() ) const;
            virtual int         columnCount(const QModelIndex &parent = QModelIndex() ) const;
            virtual QVariant    data(const QModelIndex &index, int role = Qt::DisplayRole) const ;
            virtual QVariant    headerData(int section, Qt::Orientation orientation, int role) const;

        private slots:

            void                frameRecordInserted();
            void                updateModel(void);

        private:

            QString             getFrameTime(const QCanBusFrame::TimeStamp &frameTimestamp) const;
            QString             getFrameTimeDiff(qint64 timeDiffMicroSeconds) const;
            QString             toASCIIString(const QByteArray &data) const;


        private:

            int                 m_rowCount;
            int                 m_insertedRowsCount;
            QTimer              m_updateTimer;

    };
}

#endif // LINEARCANFRAMETRACERMODEL_H
