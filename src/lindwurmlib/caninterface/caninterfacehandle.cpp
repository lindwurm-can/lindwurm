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

#include "caninterface/caninterfacehandle.h"
#include "caninterface/icaninterface.h"

namespace Lindwurm::Lib
{
    CanInterfaceHandle::CanInterfaceHandle(const std::shared_ptr<ICanInterface> &interface, const QString &component)
        : ICanInterfaceHandle(), m_interface(interface), m_mountedComponent(component)
    {
        if ( m_interface )
        {
            connect(m_interface.get(), &ICanInterface::interfaceConnected, this, &CanInterfaceHandle::interfaceConnected);
            connect(m_interface.get(), &ICanInterface::interfaceDisconnected, this, &CanInterfaceHandle::interfaceDisconnected);
            connect(m_interface.get(), &ICanInterface::frameReceived, this, &CanInterfaceHandle::frameReceived);

            m_interface->mountComponent(component);
        }
    }

    CanInterfaceHandle::CanInterfaceHandle()
        : ICanInterfaceHandle(), m_interface(), m_mountedComponent("")
    {

    }

    CanInterfaceHandle::~CanInterfaceHandle()
    {
        // ensure the interface is proper unmounted if handle is destructed
        if ( m_interface && (m_mountedComponent.isEmpty() == false) )
        {
            CanInterfaceHandle::unmount();
        }
    }

    QString CanInterfaceHandle::id() const
    {
        if (m_interface)
        {
            return m_interface->id();
        }

        return "";
    }

    QString CanInterfaceHandle::name() const
    {
        if (m_interface)
        {
            return m_interface->name();
        }

        return "";
    }

    QString CanInterfaceHandle::interfaceType() const
    {
        if (m_interface)
        {
            return m_interface->interfaceType();
        }

        return "";
    }

    QString CanInterfaceHandle::device() const
    {
        if (m_interface)
        {
            return m_interface->device();
        }

        return "";
    }

    bool CanInterfaceHandle::supportsFlexibleDataRate() const
    {
        if (m_interface)
        {
            return m_interface->supportsFlexibleDataRate();
        }

        return false;
    }

    bool CanInterfaceHandle::flexibleDataRateEnabled() const
    {
        if (m_interface)
        {
            return m_interface->flexibleDataRateEnabled();
        }

        return false;
    }

    int CanInterfaceHandle::bitRate() const
    {
        if (m_interface)
        {
            return m_interface->bitRate();
        }

        return 0;
    }

    int CanInterfaceHandle::dataBitRate() const
    {
        if (m_interface)
        {
            return m_interface->dataBitRate();
        }

        return 0;
    }

    bool CanInterfaceHandle::localEchoEnabled() const
    {
        if (m_interface)
        {
            return m_interface->localEchoEnabled();
        }

        return false;
    }

    bool CanInterfaceHandle::connected() const
    {
        if (m_interface)
        {
            return m_interface->connected();
        }

        return false;
    }

    bool CanInterfaceHandle::sendFrame(const QCanBusFrame &frame)
    {
        if (m_interface)
        {
            return m_interface->sendFrame(frame);
        }

        return false;
    }

    bool CanInterfaceHandle::unmount()
    {
        if ( m_interface )
        {
            disconnect(m_interface.get(), &ICanInterface::frameReceived, this, &CanInterfaceHandle::frameReceived);
        }

        m_interface->unmountComponent(m_mountedComponent);

        m_interface.reset();
        m_mountedComponent = "";

        return true;
    }

    bool CanInterfaceHandle::isMounted() const
    {
        if ( m_interface )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

}


