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

#ifndef DATABYIDTABLEMODEL_H
#define DATABYIDTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QByteArray>
#include <QVector>

namespace Lindwurm::Core
{
    class DataByIdTableModel : public QAbstractTableModel
    {
        Q_OBJECT
        public:

            explicit DataByIdTableModel(QObject *parent = nullptr);

            virtual int         rowCount(const QModelIndex &parent = QModelIndex() ) const;
            virtual int         columnCount(const QModelIndex &parent = QModelIndex() ) const;
            virtual QVariant    data(const QModelIndex &index, int role = Qt::DisplayRole) const;
            virtual QVariant    headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

            void                addData(quint16 id, const QByteArray &data);
            void                reset();

        private:

            QMap<quint16, QByteArray>   m_data;
            QVector<quint16>            m_indexToIdList;
    };
}

#endif // DATABYIDTABLEMODEL_H
