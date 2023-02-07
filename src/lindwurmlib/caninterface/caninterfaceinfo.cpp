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

#include "caninterface/caninterfaceinfo.h"

#include "caninterface/icaninterface.h"

namespace Lindwurm::Lib
{
    CanInterfaceInfo::CanInterfaceInfo()
        : m_Interface()
    {
        // default constructor (needed for qRegisterMetaType and error handling (empty object) )
        // creates an invalid info object
    }

    CanInterfaceInfo::CanInterfaceInfo(const std::weak_ptr<ICanInterface> &interface)
    {
        m_Interface = interface;
    }

    CanInterfaceInfo::CanInterfaceInfo(const CanInterfaceInfo &other)
        : m_Interface(other.m_Interface)
    {

    }

    CanInterfaceInfo::~CanInterfaceInfo()
    {

    }

    CanInterfaceInfo &CanInterfaceInfo::operator=(const CanInterfaceInfo &other)
    {
        m_Interface = other.m_Interface;
        return *this;
    }

    QString CanInterfaceInfo::id() const
    {
        auto interface = m_Interface.lock();

        if ( interface )
        {
            return interface->id();
        }

        return "";
    }

    QString CanInterfaceInfo::name() const
    {
        auto interface = m_Interface.lock();

        if ( interface )
        {
            return interface->name();
        }

        return "";
    }

    QString CanInterfaceInfo::interfaceType() const
    {
        auto interface = m_Interface.lock();

        if ( interface )
        {
            return interface->interfaceType();
        }

        return "";
    }

    QString CanInterfaceInfo::device() const
    {
        auto interface = m_Interface.lock();

        if ( interface )
        {
            return interface->device();
        }

        return "";
    }

    bool CanInterfaceInfo::supportsFlexibleDataRate() const
    {
        auto interface = m_Interface.lock();

        if ( interface )
        {
            return interface->supportsFlexibleDataRate();
        }

        return false;
    }

    bool CanInterfaceInfo::flexibleDataRateEnabled() const
    {
        auto interface = m_Interface.lock();

        if ( interface )
        {
            return interface->flexibleDataRateEnabled();
        }

        return false;
    }

    int CanInterfaceInfo::bitRate() const
    {
        auto interface = m_Interface.lock();

        if ( interface )
        {
            return interface->bitRate();
        }

        return 0;
    }

    int CanInterfaceInfo::dataBitRate() const
    {
        auto interface = m_Interface.lock();

        if ( interface )
        {
            return interface->dataBitRate();
        }

        return 0;
    }

    bool CanInterfaceInfo::localEchoEnabled() const
    {
        auto interface = m_Interface.lock();

        if ( interface )
        {
            return interface->localEchoEnabled();
        }

        return false;
    }

    bool CanInterfaceInfo::connected() const
    {
        auto interface = m_Interface.lock();

        if ( interface )
        {
            return interface->connected();
        }

        return false;
    }

    bool CanInterfaceInfo::isValid() const
    {
        return ! m_Interface.expired();
    }
}
