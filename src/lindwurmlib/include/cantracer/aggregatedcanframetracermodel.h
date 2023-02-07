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

#ifndef AGGREGATEDCANFRAMETRACERMODEL_H
#define AGGREGATEDCANFRAMETRACERMODEL_H

#include "lindwurmlib_global.h"

#include "cantracer/abstractcanframetracermodel.h"
#include <QVector>
#include <QTimer>

namespace Lindwurm::Lib
{
    class CanFrameTracer;

    class LINDWURMLIB_EXPORT AggregatedCanFrameTracerModel : public AbstractCanFrameTracerModel
    {
        Q_OBJECT
        public:

            AggregatedCanFrameTracerModel(CanFrameTracer *tracer, QObject *parent = nullptr);

            virtual int         rowCount(const QModelIndex &parent = QModelIndex() ) const;
            virtual int         columnCount(const QModelIndex &parent = QModelIndex() ) const;
            virtual QVariant    data(const QModelIndex &index, int role = Qt::DisplayRole) const ;
            virtual QVariant    headerData(int section, Qt::Orientation orientation, int role) const;

        private slots:

            void                aggregateRecordInserted();
            void                aggregateRecordUpdated(int aggregateRecordIndex);
            void                updateModel(void);

        private:

            int                 m_rowCount;
            QVector<int>        m_updatedAggregateIndices;
            QTimer              m_updateTimer;
    };
}

#endif // AGGREGATEDCANFRAMETRACERMODEL_H
