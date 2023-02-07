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

#include "databyidtablemodel.h"
#include "diagnostic/udsdataid.h"
#include <QSize>

using namespace Lindwurm::Lib;

namespace Lindwurm::Core
{
    DataByIdTableModel::DataByIdTableModel(QObject *parent)
        : QAbstractTableModel{parent}
    {

    }

    int DataByIdTableModel::rowCount(const QModelIndex &parent) const
    {
        if ( parent.isValid() )
        {
            return 0;
        }

        return m_indexToIdList.size();
    }

    int DataByIdTableModel::columnCount(const QModelIndex &parent) const
    {
        if ( parent.isValid() )
        {
            return 0;
        }

        return 4;
    }

    QVariant DataByIdTableModel::data(const QModelIndex &index, int role) const
    {
        if ( ! index.isValid() )
        {
            return QVariant();
        }

        if ( role == Qt::DisplayRole )
        {
            quint16 id = m_indexToIdList.at( index.row() );
            QByteArray rawData = m_data[id];

            switch ( index.column() )
            {
                case 0: return UdsDataId::get(id).id();
                case 1: return UdsDataId::get(id).description();
                case 2: return QString::fromLatin1( rawData.data(), rawData.size() );
                case 3: return rawData.toHex(' ').toUpper();
            }
        }

        return QVariant();
    }

    QVariant DataByIdTableModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if ( role == Qt::DisplayRole && orientation == Qt::Orientation::Horizontal )
        {
            switch (section)
            {
                case 0:     return "Data ID";
                case 1:     return "Description";
                case 2:     return "Value";
                case 3:     return "RAW";
                default:    return QVariant();
            }
        }

        if ( role == Qt::SizeHintRole && orientation == Qt::Orientation::Horizontal )
        {
            switch (section)
            {
                case 0:     return QSize(70, 24);
                case 1:     return QSize(335, 24);
                case 2:     return QSize(450, 24);
                case 3:     return QSize(565, 24);
                default:    return QVariant();
            }
        }

        return QVariant();
    }

    void DataByIdTableModel::addData(quint16 id, const QByteArray &data)
    {
        beginInsertRows( QModelIndex(), m_indexToIdList.size(), m_indexToIdList.size() );

            m_indexToIdList.append(id);
            m_data.insert(id, data);

            endInsertRows();
    }

    void DataByIdTableModel::reset()
    {
        beginResetModel();
            m_data.clear();
            m_indexToIdList.clear();
        endResetModel();
    }
}
