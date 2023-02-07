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

#include "caninterface/caninterfacemanager.h"
#include "caninterface/icaninterface.h"
#include "caninterface/caninterfacehandle.h"
#include "caninterface/candevice.h"
#include "caninterface/canbridge.h"
#include "caninterface/caninterfacelistmodel.h"

#include <QCanBus>
#include <QUuid>

#include <QDebug>
#include <QLoggingCategory>

namespace
{
    Q_LOGGING_CATEGORY(LOG_TAG, "lindwurm.lib.interface")
}

namespace Lindwurm::Lib
{
    CanInterfaceManager::CanInterfaceManager(QObject *parent)
        : ICanInterfaceManager(parent)
        , m_listModel( new CanInterfaceListModel(this, this) )
    {
        ICanInterfaceManager::s_instance = this;
    }

    int CanInterfaceManager::numberOfInterfaces() const
    {
        return m_interfacesById.size();
    }

    QStringList CanInterfaceManager::interfaceTypes() const
    {
        QStringList types = QCanBus::instance()->plugins();
        types.append("bridge");

        return types;
    }

    QStringList CanInterfaceManager::availableDevicesOf(const QString &interfaceType) const
    {
        QList<QCanBusDeviceInfo> availableDevices = QCanBus::instance()->availableDevices(interfaceType);
        QStringList devices;

        for ( const auto &info : qAsConst(availableDevices) )
        {
            devices.append( info.name() );
        }

        return devices;
    }

    QList<CanInterfaceInfo> CanInterfaceManager::availableInterfaces() const
    {
        QList<CanInterfaceInfo> interfaces;

        // Problem: interfaces are sorted by random id
        /*
        for (const auto &interface : qAsConst(m_interfacesById) )
        {
            interfaces.append( CanInterfaceInfo( interface ) );
        }
        */

        // get interfaces sorted by name
        for (const auto &interfaceId : qAsConst(m_interfacesIdByName) )
        {
            interfaces.append( CanInterfaceInfo( m_interfacesById[interfaceId] ) );
        }

        return interfaces;
    }

    CanInterfaceInfo CanInterfaceManager::interfaceInfoFor(const QString &interfaceId)
    {
        ICanInterfaceSharedPtr interface = m_interfacesById[interfaceId];

        if ( interface )
        {
            return CanInterfaceInfo(interface);
        }

        // if we did not find the interfaceID, we return a default (invalid) InterfaceInfo
        return CanInterfaceInfo();
    }

    ICanInterfaceSharedPtr CanInterfaceManager::interfaceById(const QString &interfaceId)
    {
        return m_interfacesById[interfaceId];
    }

    bool CanInterfaceManager::connectInterface(const QString &interfaceId)
    {
        ICanInterfaceSharedPtr interface = m_interfacesById[interfaceId];

        if ( ! interface )
        {
            return false;
        }

        qInfo(LOG_TAG) << "Connect interface " << interface->name();
        return interface->connectInterface();
    }

    bool CanInterfaceManager::disconnectInterface(const QString &interfaceId)
    {
        ICanInterfaceSharedPtr interface = m_interfacesById[interfaceId];

        if ( ! interface )
        {
            return false;
        }

        qInfo(LOG_TAG) << "Disconnect interface " << interface->name();
        return interface->disconnectInterface();
    }

    ICanInterfaceManager::InterfaceError CanInterfaceManager::addInterface(const CanInterfaceConfig &config, bool autoConnect)
    {
        if ( m_interfacesIdByName.contains(config.name) || config.name.isEmpty() )
        {
            return InterfaceError::DuplicateInterfaceName;
        }

        ICanInterfaceSharedPtr newCANInterface;

        if ( config.interfaceType == "bridge" )
        {
            newCANInterface = createBridge(config);
        }
        else
        {
            newCANInterface = createInterface(config);
        }

        if ( ! newCANInterface )
        {
            return InterfaceError::UnknownError;
        }

        newCANInterface->setName(config.name);

        m_interfacesById.insert( newCANInterface->id(), newCANInterface );
        m_interfacesIdByName.insert( newCANInterface->name(), newCANInterface->id() );

        if (autoConnect)
        {
            newCANInterface->connectInterface();
        }

        emit interfaceAdded( CanInterfaceInfo(newCANInterface) );

        return InterfaceError::NoError;
    }

    ICanInterfaceManager::InterfaceError CanInterfaceManager::editInterface(const CanInterfaceConfig &config)
    {
        ICanInterfaceSharedPtr  interface = m_interfacesById[config.id];

        if ( !interface )
        {
            return InterfaceError::InterfaceNotFound;
        }

        if ( interface->name() != config.name )
        {
            // interface name change requested

            if ( m_interfacesIdByName.contains( config.name ) )
            {
                if ( m_interfacesIdByName[config.name] != interface->id() )
                {
                    // we found a different interface with this name
                    return InterfaceError::DuplicateInterfaceName;
                }
            }

            interface->setName(config.name);
        }

        if ( interface->interfaceType() == "bridge" )
        {
            std::shared_ptr<CanBridge> bridge = std::dynamic_pointer_cast<CanBridge>(interface);

            if ( ! bridge )
            {
                // this should not happen
                return InterfaceError::UnknownError;
            }

            QStringList currentlyBridgedInterfaceIDs;
            currentlyBridgedInterfaceIDs = bridge->bridgedInterfaceIds();

            bool bridgeWasConnected = bridge->connected();

            if ( bridgeWasConnected )
            {
                // for precaution we disconnect the bridge while changing it's mount configuration
                bridge->disconnectInterface();
            }

                // TODO: Check for possible (dangerous) misconfigurations (like loops, etc.) !?

                // iterate the requested bridge interfaces and mount them if they are not already mounted on the bridge
                for (const QString &requestedBridgeInterfaceId : config.bridgedInterfaces )
                {
                    if ( currentlyBridgedInterfaceIDs.contains( requestedBridgeInterfaceId ) )
                    {
                        // interface already mounted on bridge
                        continue;
                    }

                    if ( m_interfacesById.contains( requestedBridgeInterfaceId ) )
                    {
                        bridge->mountInterface( m_interfacesById[ requestedBridgeInterfaceId ]  );
                    }
                }

                // iterate the current mounted bridge interfaces and unmount all interfaces not requested in the new configuration
                for (const QString &currentlyBridgetInterfaceId : qAsConst(currentlyBridgedInterfaceIDs))
                {
                    if ( ! config.bridgedInterfaces.contains(currentlyBridgetInterfaceId) )
                    {
                        // currently bridged interface is not anymore in configured list, so unmount it
                        bridge->unmountInterface( m_interfacesById[ currentlyBridgetInterfaceId ] );
                    }
                }

            if ( bridgeWasConnected )
            {
                // if the bridge was connected before reconnect it
                bridge->connectInterface();
            }
        }
        else
        {
            // these settings are not supported by bridge interfaces

            if ( interface->bitRate() != config.bitRate )
            {
                interface->setBitRate( config.bitRate );
            }

            if ( interface->flexibleDataRateEnabled() != config.enableFlexibleDataRate )
            {
                interface->setFlexibleDataRateEnabled( config.enableFlexibleDataRate );
            }

            if ( interface->flexibleDataRateEnabled() && (interface->dataBitRate() != config.dataBitRate ) )
            {
                interface->setDataBitRate( config.dataBitRate );
            }

            if ( interface->localEchoEnabled() != config.enableLocalEcho )
            {
                interface->setLocalEchoEnabled( config.enableLocalEcho );
            }
        }

        emit interfaceModified( CanInterfaceInfo(interface) );

        return InterfaceError::NoError;
    }

    ICanInterfaceManager::InterfaceError CanInterfaceManager::removeInterface(const QString &interfaceID)
    {
        ICanInterfaceSharedPtr  interface = m_interfacesById[interfaceID];

        if ( ! interface )
        {
            return InterfaceError::InterfaceNotFound;
        }

        if ( interface->isMounted() )
        {
            return InterfaceError::InterfaceMounted;
        }

        m_interfacesById.remove(interfaceID);
        m_interfacesIdByName.remove(interface->name());

        emit interfaceRemoved(interfaceID);

        qInfo(LOG_TAG) << "Removed interface" << interface->name();

        return InterfaceError::NoError;
    }

    QAbstractItemModel *CanInterfaceManager::interfaceListModel() const
    {
        return m_listModel;
    }

    ICanInterfaceHandleSharedPtr CanInterfaceManager::mountInterface(const QString &interfaceId, const QString &component)
    {
        ICanInterfaceSharedPtr interface = m_interfacesById[interfaceId];

        if (interface)
        {
            return std::make_shared<CanInterfaceHandle>(interface, component);
        }

        return std::shared_ptr<ICanInterfaceHandle>();
    }

    QStringList CanInterfaceManager::mountedComponents(const QString &interfaceId) const
    {
        ICanInterfaceSharedPtr  interface = m_interfacesById[interfaceId];

        if ( ! interface )
        {
            return QStringList();
        }

        return interface->mountedComponents();
    }

    ICanInterfaceSharedPtr CanInterfaceManager::createInterface(const CanInterfaceConfig &config)
    {
         ICanInterfaceSharedPtr newCANInterface;

        QString errorString;
        QCanBusDevice   *canBusDevice = QCanBus::instance()->createDevice(config.interfaceType, config.device, &errorString);

        if ( canBusDevice == nullptr )
        {
            qCritical(LOG_TAG) << "Failed to create QCanBusDevice: " << errorString;
            return ICanInterfaceSharedPtr();
        }

        newCANInterface = std::make_shared<CanDevice>(canBusDevice, QUuid::createUuid().toString(), config.interfaceType, config.device);

        newCANInterface->setLocalEchoEnabled(config.enableLocalEcho);
        newCANInterface->setFlexibleDataRateEnabled(config.enableFlexibleDataRate);
        newCANInterface->setBitRate(config.bitRate);

        if ( config.enableFlexibleDataRate )
        {
            newCANInterface->setDataBitRate(config.dataBitRate);
        }

        return newCANInterface;
    }

    ICanInterfaceSharedPtr CanInterfaceManager::createBridge(const CanInterfaceConfig &config)
    {
        std::shared_ptr<CanBridge> newBridge = std::make_shared<CanBridge>( QUuid::createUuid().toString() );

        for (const QString &requestedBridgeInterfaceId : qAsConst(config.bridgedInterfaces) )
        {
            if ( m_interfacesById.contains( requestedBridgeInterfaceId ) )
            {
                newBridge->mountInterface( m_interfacesById[ requestedBridgeInterfaceId ]  );
            }
        }

        return newBridge;
    }
}
