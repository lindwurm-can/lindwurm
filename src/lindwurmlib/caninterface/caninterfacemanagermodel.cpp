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

#include "caninterface/caninterfacemanagermodel.h"
#include "caninterface/caninterfacemanager.h"
#include "caninterface/icaninterface.h"
#include "caninterface/icaninterfacesharedptr.h"
#include <QSize>

namespace Lindwurm::Lib
{
    CanInterfaceManagerModel::CanInterfaceManagerModel(CanInterfaceManager *manager, QObject *parent)
        : QAbstractTableModel( parent )
        , m_interfaceManager( manager )
        , m_interfaceList( manager->availableInterfaces() )
    {
        connect(m_interfaceManager, &CanInterfaceManager::interfaceAdded,       this, &CanInterfaceManagerModel::interfaceAdded);
        connect(m_interfaceManager, &CanInterfaceManager::interfaceModified,    this, &CanInterfaceManagerModel::interfaceModified);
        connect(m_interfaceManager, &CanInterfaceManager::interfaceRemoved,     this, &CanInterfaceManagerModel::interfaceRemoved);
    }

    int CanInterfaceManagerModel::rowCount(const QModelIndex &parent) const
    {
        if ( ! parent.isValid() )
        {
            return m_interfaceList.size();
        }

        return 0;
    }

    int CanInterfaceManagerModel::columnCount(const QModelIndex &parent) const
    {
        if ( ! parent.isValid() )
        {
            return 8;
        }

        return 0;
    }

    QVariant CanInterfaceManagerModel::data(const QModelIndex &index, int role) const
    {
        if ( ! index.isValid() )
        {
            return QVariant();
        }

        CanInterfaceInfo info = m_interfaceList.at( index.row() );

        Q_ASSERT_X( info.isValid(), "CanInterfaceManagerModel", "Interface for index not found!");

        if ( role == InterfaceIdRole )
        {
            return info.id();
        }

        if ( role == Qt::DisplayRole )
        {
            switch ( index.column() )
            {
                case 0:     return QVariant();
                case 1:     return info.name();
                case 2:     return info.interfaceType();
                case 3:     return info.device();
                case 4:

                    if ( info.bitRate() > 0)
                    {
                        return info.bitRate();
                    }
                    else
                    {
                        return "";
                    }

                case 5:     return QVariant();
                case 6:

                    if ( info.dataBitRate() > 0)
                    {
                        return info.dataBitRate();
                    }
                    else
                    {
                        return "";
                    }

                case 7:     return QVariant();

                default:    return QVariant();
            }
        }

        if ( role == Qt::CheckStateRole )
        {
            switch ( index.column() )
            {
                case 0:

                    if ( info.connected() )
                    {
                        return Qt::Checked;
                    }
                    else
                    {
                        return Qt::Unchecked;
                    }

                case 5:

                    if ( info.interfaceType() == "bridge" )
                    {
                        return QVariant();
                    }

                    if ( info.flexibleDataRateEnabled() )
                    {
                        return Qt::Checked;
                    }
                    else
                    {
                        return Qt::Unchecked;
                    }

                case 7:

                    if ( info.interfaceType() == "bridge" )
                    {
                        return QVariant();
                    }

                    if ( info.localEchoEnabled() )
                    {
                        return Qt::Checked;
                    }
                    else
                    {
                        return Qt::Unchecked;
                    }

                default:

                    return QVariant();
            }
        }

        return QVariant();
    }

    bool CanInterfaceManagerModel::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if ( ! index.isValid() )
        {
            return false;
        }

        if (role == Qt::CheckStateRole)
        {
            QString interfaceId = m_interfaceList.at( index.row() ).id();

            bool isChecked = value.toInt() == Qt::Checked;

            switch ( index.column() )
            {
                case 0:

                    if ( ! setInterfaceConnected(interfaceId, isChecked) )
                    {
                        return false;
                    }
                    break;

                case 7:

                    if ( ! setInterfaceLocalEchoEnabled(interfaceId, isChecked) )
                    {
                        return false;
                    }

                    break;
            }

            emit dataChanged( index, index );
            return true;
        }

        return false;
    }

    QVariant CanInterfaceManagerModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if ( role == Qt::DisplayRole && orientation == Qt::Orientation::Horizontal )
        {
            switch (section)
            {
                case 0:     return "Connected";
                case 1:     return "Name";
                case 2:     return "Type";
                case 3:     return "Device";
                case 4:     return "Bitrate";
                case 5:     return "FD";
                case 6:     return "Data Bitrate";
                case 7:     return "Local Echo";
                default:    return QVariant();
            }
        }

        if ( role == Qt::SizeHintRole && orientation == Qt::Orientation::Horizontal )
        {
            switch (section)
            {
                case 0:     return QSize(90, 24);
                case 1:     return QSize(150, 24);
                case 2:     return QSize(120, 24);
                case 3:     return QSize(150, 24);
                case 4:     return QSize(100, 24);
                case 5:     return QSize(30, 24);
                case 6:     return QSize(130, 24);
                default:    return QVariant();
            }
        }

        return QVariant();
    }

    Qt::ItemFlags CanInterfaceManagerModel::flags(const QModelIndex &index) const
    {
        Qt::ItemFlags baseFlags = QAbstractTableModel::flags(index);

        if ( ! index.isValid() )
        {
            return baseFlags;
        }

        switch ( index.column() )
        {
            case 0:
            case 7:
                return baseFlags | Qt::ItemIsUserCheckable;

            default:
                break;
        }

        return baseFlags;
    }

    QModelIndex CanInterfaceManagerModel::index(int row, int column, const QModelIndex &parent) const
    {
        if ( !hasIndex(row, column, parent) )
        {
            return QModelIndex();
        }

        return createIndex(row, column);
    }

    void CanInterfaceManagerModel::interfaceAdded(const CanInterfaceInfo &interface)
    {
        beginInsertRows( QModelIndex(), m_interfaceList.size(), m_interfaceList.size());

        m_interfaceList.append(interface);

        endInsertRows();
    }

    void CanInterfaceManagerModel::interfaceModified(const CanInterfaceInfo &info)
    {
        int row = findRowForInterfaceId( info.id() );

        if ( row >= 0 )
        {
            emit dataChanged( createIndex(row, 0), createIndex(row, 7) );
        }
    }

    void CanInterfaceManagerModel::interfaceRemoved(const QString &interfaceId)
    {
        int row = findRowForInterfaceId(interfaceId);

        if ( row >= 0 )
        {
            beginRemoveRows( QModelIndex(), row, row);

            m_interfaceList.removeAt(row);

            endRemoveRows();
        }
    }

    int CanInterfaceManagerModel::findRowForInterfaceId(const QString &interfaceId) const
    {
        for (int row = 0; row < m_interfaceList.size(); row++)
        {
            if ( m_interfaceList.at(row).id() == interfaceId)
            {
                return row;
            }
        }

        return -1;
    }

    bool CanInterfaceManagerModel::setInterfaceConnected(QString interfaceId, bool connect)
    {
        bool result = false;

        if ( connect )
        {
            result = m_interfaceManager->connectInterface( interfaceId );
        }
        else
        {
            result = m_interfaceManager->disconnectInterface( interfaceId );
        }

        return result;
    }

    bool CanInterfaceManagerModel::setInterfaceLocalEchoEnabled(QString interfaceId, bool enabled)
    {
        ICanInterfaceSharedPtr canInterface = m_interfaceManager->interfaceById(interfaceId);

        if ( ! canInterface )
        {
            return false;
        }

        canInterface->setLocalEchoEnabled(enabled);

        return true;
    }
}
