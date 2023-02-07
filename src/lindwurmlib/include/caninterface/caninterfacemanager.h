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

#ifndef CANINTERFACEMANAGER_H
#define CANINTERFACEMANAGER_H

#include "lindwurmlib_global.h"
#include "icaninterfacemanager.h"
#include "icaninterfacesharedptr.h"

#include <QMap>
#include <QStringList>

namespace Lindwurm::Lib
{
    class CanInterfaceListModel;

    class LINDWURMLIB_EXPORT CanInterfaceManager : public ICanInterfaceManager
    {
        Q_OBJECT
        public:

                                                    CanInterfaceManager(QObject *parent);

            virtual int                             numberOfInterfaces(void) const override;
            virtual QStringList                     interfaceTypes() const override;
            virtual QStringList                     availableDevicesOf(const QString &interfaceType) const override;
            virtual QList<CanInterfaceInfo>         availableInterfaces() const override;
            virtual CanInterfaceInfo                interfaceInfoFor(const QString &interfaceId) override;

            ICanInterfaceSharedPtr                  interfaceById(const QString &interfaceId);

            bool                                    connectInterface(const QString &interfaceId);
            bool                                    disconnectInterface(const QString &interfaceId);

            InterfaceError                          addInterface(const CanInterfaceConfig &config, bool autoConnect = false);
            InterfaceError                          editInterface(const CanInterfaceConfig &config);
            InterfaceError                          removeInterface(const QString& interfaceID);

            virtual QAbstractItemModel*             interfaceListModel() const override;
            virtual ICanInterfaceHandleSharedPtr    mountInterface(const QString &interfaceId, const QString &component) override;

            QStringList                             mountedComponents(const QString& interfaceId) const;

        private:

            ICanInterfaceSharedPtr                  createInterface(const CanInterfaceConfig &config);
            ICanInterfaceSharedPtr                  createBridge(const CanInterfaceConfig &config);

        private:

            QMap<QString, QString>                  m_interfacesIdByName;
            QMap<QString, ICanInterfaceSharedPtr>   m_interfacesById;
            CanInterfaceListModel*                  m_listModel = { nullptr };
    };
}



#endif // CANINTERFACEMANAGER_H
