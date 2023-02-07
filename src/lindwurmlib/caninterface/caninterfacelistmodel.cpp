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

#include "caninterfacelistmodel.h"
#include "caninterface/caninterfacemanager.h"

namespace Lindwurm::Lib
{
    CanInterfaceListModel::CanInterfaceListModel(CanInterfaceManager *interfaceManager, QObject *parent)
        : QAbstractListModel(parent)
    {
        QList<CanInterfaceInfo> interfaces = interfaceManager->availableInterfaces();

        for (const CanInterfaceInfo &interfaceInfo : interfaces)
        {
            m_interfaceList.append( std::make_tuple( interfaceInfo.name(), interfaceInfo.id() ) );
        }

        connect(interfaceManager, &ICanInterfaceManager::interfaceAdded, this, &CanInterfaceListModel::interfaceAdded);
        connect(interfaceManager, &ICanInterfaceManager::interfaceRemoved, this, &CanInterfaceListModel::interfaceRemoved);
        connect(interfaceManager, &ICanInterfaceManager::interfaceModified, this, &CanInterfaceListModel::interfaceModified);
    }

    int CanInterfaceListModel::rowCount(const QModelIndex &parent) const
    {
        if ( ! parent.isValid() )
        {
            return m_interfaceList.size();
        }

        return 0;
    }

    QVariant CanInterfaceListModel::data(const QModelIndex &index, int role) const
    {
        if ( ! index.isValid() )
        {
            return QVariant();
        }

        if ( role == Qt::DisplayRole )
        {
            // first entry of the tuple is the interface name
            return std::get<0>( m_interfaceList.at( index.row() ) );
        }

        // UserRole is used to return the interface id
        if ( role == Qt::UserRole )
        {
            // second entry of the tuple is the interface id
            return std::get<1>( m_interfaceList.at( index.row() ) );
        }

        return QVariant();
    }

    void CanInterfaceListModel::interfaceAdded(const CanInterfaceInfo &interfaceInfo)
    {
        beginInsertRows( QModelIndex(), m_interfaceList.size(), m_interfaceList.size());

        m_interfaceList.append( std::make_tuple( interfaceInfo.name(), interfaceInfo.id() ) );

        endInsertRows();
    }

    void CanInterfaceListModel::interfaceModified(const CanInterfaceInfo &interfaceInfo)
    {
        int row = 0;
        bool interfaceFound = false;

        for (row = 0; row < m_interfaceList.size(); row++)
        {
            if ( std::get<1>( m_interfaceList.at(row) ) == interfaceInfo.id() )
            {
                interfaceFound = true;
                break;
            }
        }

        if ( interfaceFound )
        {
            std::get<0>( m_interfaceList[row] ) = interfaceInfo.name();
            emit dataChanged( createIndex(row, 0), createIndex(row, 0) );
        }
    }

    void CanInterfaceListModel::interfaceRemoved(const QString &interfaceId)
    {
        int row = 0;
        bool interfaceFound = false;

        for (row = 0; row < m_interfaceList.size(); row++)
        {
            if ( std::get<1>( m_interfaceList.at(row) ) == interfaceId )
            {
                interfaceFound = true;
                break;
            }
        }

        if ( interfaceFound )
        {
            beginRemoveRows( QModelIndex(), row, row);

            m_interfaceList.removeAt(row);

            endRemoveRows();
        }
    }
}
