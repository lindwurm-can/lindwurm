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

#ifndef CANINTERFACEMANAGERMODEL_H
#define CANINTERFACEMANAGERMODEL_H

#include "lindwurmlib_global.h"
#include <QAbstractTableModel>
#include <QList>

#include "caninterface/caninterfaceinfo.h"

namespace Lindwurm::Lib
{
    class CanInterfaceManager;

    /**
     * @brief The CanInterfaceManagerModel class provides a table model to view and modify the currently available CAN interfaces.
     *
     * This model is mainly used by the CanInterfaceManagerWidget to provide an overview of the available CAN interfaces and
     * their configuration. If you want to provide a list of available CAN interfaces for selection you must use CanInterfaceListModel instead.
     */
    class LINDWURMLIB_EXPORT CanInterfaceManagerModel : public QAbstractTableModel
    {
        Q_OBJECT
        public:

            CanInterfaceManagerModel(CanInterfaceManager* manager, QObject* parent = nullptr);

            virtual int             rowCount(const QModelIndex &parent = QModelIndex() ) const;
            virtual int             columnCount(const QModelIndex &parent = QModelIndex() ) const;
            virtual QVariant        data(const QModelIndex &index, int role = Qt::DisplayRole) const ;
            virtual bool            setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
            virtual QVariant        headerData(int section, Qt::Orientation orientation, int role) const;
            virtual Qt::ItemFlags   flags(const QModelIndex &index) const;
            virtual QModelIndex     index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

            enum
            {
                InterfaceIdRole = Qt::UserRole + 1
            };

        private slots:

            void                    interfaceAdded(const Lindwurm::Lib::CanInterfaceInfo &interface);
            void                    interfaceModified(const Lindwurm::Lib::CanInterfaceInfo &info);
            void                    interfaceRemoved(const QString &interfaceId);

        private:

            int                     findRowForInterfaceId(const QString &interfaceId) const;
            bool                    setInterfaceConnected(QString interfaceId, bool connect);
            bool                    setInterfaceLocalEchoEnabled(QString interfaceId, bool enabled);

        private:

            CanInterfaceManager*    m_interfaceManager;
            QList<CanInterfaceInfo> m_interfaceList = {};
    };
}

#endif // CANINTERFACEMANAGERMODEL_H
