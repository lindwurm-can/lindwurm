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

#include "caninterface/canbridge.h"

#include <QDebug>
#include <QLoggingCategory>

namespace
{
    Q_LOGGING_CATEGORY(LOG_TAG, "lindwurm.lib.interface")
}

namespace Lindwurm::Lib
{
    CanBridge::CanBridge(const QString &id)
        : AbstractCanInterface(id, "bridge", "")
    {

    }

    CanBridge::~CanBridge()
    {
    }

    bool CanBridge::connected() const
    {
        return m_bridgeConnected;
    }

    bool CanBridge::connectInterface()
    {
        m_bridgeConnected = true;
        return true;
    }

    bool CanBridge::disconnectInterface()
    {
        m_bridgeConnected = false;
        return true;
    }

    bool CanBridge::sendFrame(const QCanBusFrame &frame)
    {
        if ( ! m_bridgeConnected )
        {
            return false;
        }

        for (const ICanInterfaceSharedPtr &interface : qAsConst(m_mountedInterfaces) )
        {
            interface->sendFrame(frame);
        }

        return true;
    }

    QString CanBridge::device() const
    {
        QString bridgedInterfaces;

        for ( const ICanInterfaceSharedPtr &interface : m_mountedInterfaces )
        {
            if ( bridgedInterfaces.isEmpty() )
            {
                bridgedInterfaces.append( interface->name() );
            }
            else
            {
                bridgedInterfaces.append( "|" + interface->name() );
            }
        }

        return bridgedInterfaces;
    }

    void CanBridge::frameReceivedForBridge(const QCanBusFrame &frame, const QString &sourceInterface)
    {
        if ( ! m_bridgeConnected )
        {
            return;
        }

        emit frameReceived(frame, sourceInterface);

        if ( frame.hasLocalEcho() )
        {
            // this is a frame which was (probably) send by this bride
            // ignore to avoid a loop
            return;
        }

        for (const ICanInterfaceSharedPtr &interface : qAsConst(m_mountedInterfaces) )
        {
            if ( interface->name() != sourceInterface )
            {
                interface->sendFrame(frame);
            }
        }
    }

    bool CanBridge::mountInterface(const ICanInterfaceSharedPtr &interface)
    {
        if ( m_mountedInterfaces.contains(interface) )
        {
            qWarning(LOG_TAG) << "Interface already connected to this bridge";
            return false;
        }

        connect(interface.get(), &ICanInterface::frameReceived, this, &CanBridge::frameReceivedForBridge);
        m_mountedInterfaces.append(interface);

        // TODO: This label is displayed to the user when trying to delete a mounted interface. It would be better
        // to display the name of the bridge, the interfaces is mounted to. However, because the name is changeable
        // we cannot use it here (as unmounting with a possible new name would fail).
        // A workaround would be to override setName and adjust the mounted component dynamically (e.g. unmount old name, mount new name).
        // Better: make component a complex object, with ID and displayed name as separated attributes.
        interface->mountComponent( "bridge." + id() );

        return true;
    }

    bool CanBridge::unmountInterface(const ICanInterfaceSharedPtr &interface)
    {
        if ( ! interface )
        {
            return false;
        }

        m_mountedInterfaces.removeAll(interface);
        disconnect(interface.get(), &ICanInterface::frameReceived, this, &CanBridge::frameReceivedForBridge);
        interface->unmountComponent( "bridge." + id() );

        return true;
    }

    QStringList CanBridge::bridgedInterfaceIds() const
    {
        QStringList bridgedInterfaceIDs;

        for ( const ICanInterfaceSharedPtr &interface : m_mountedInterfaces )
        {
            bridgedInterfaceIDs.append( interface->id() );
        }

        return bridgedInterfaceIDs;
    }

    // functions unsupported by CAN bridge

    bool CanBridge::supportsFlexibleDataRate() const
    {
        return false;
    }

    bool CanBridge::flexibleDataRateEnabled() const
    {
        return false;
    }

    int CanBridge::bitRate() const
    {
        return 0;
    }

    int CanBridge::dataBitRate() const
    {
        return 0;
    }

    bool CanBridge::localEchoEnabled() const
    {
        return false;
    }

    void CanBridge::setFlexibleDataRateEnabled(bool enabled)
    {
        Q_UNUSED(enabled)
    }

    void CanBridge::setBitRate(int bitRate)
    {
        Q_UNUSED(bitRate)
    }

    void CanBridge::setDataBitRate(int dataBitRate)
    {
        Q_UNUSED(dataBitRate)
    }

    void CanBridge::setLocalEchoEnabled(bool enable)
    {
        Q_UNUSED(enable)
    }
}


