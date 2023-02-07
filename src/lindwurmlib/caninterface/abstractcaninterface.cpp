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

#include "caninterface/abstractcaninterface.h"

namespace Lindwurm::Lib
{
    AbstractCanInterface::AbstractCanInterface(const QString &id, const QString &interfaceType, const QString &device)
        : ICanInterface(), m_id(id), m_interfaceType(interfaceType), m_device(device)
    {

    }

    QString AbstractCanInterface::id() const
    {
        return m_id;
    }

    QString AbstractCanInterface::name() const
    {
        return m_name;
    }

    QString AbstractCanInterface::interfaceType() const
    {
        return m_interfaceType;
    }

    QString AbstractCanInterface::device() const
    {
        return m_device;
    }

    void AbstractCanInterface::setName(const QString &name)
    {
        m_name = name;
    }

    void AbstractCanInterface::mountComponent(const QString &component)
    {
        // don't mount components twice
        if ( ! m_mountedComponents.contains(component) )
        {
            m_mountedComponents.append(component);
        }
    }

    void AbstractCanInterface::unmountComponent(const QString &component)
    {
        m_mountedComponents.removeAll(component);
    }

    bool AbstractCanInterface::isMounted() const
    {
        return m_mountedComponents.size() > 0;
    }

    QStringList AbstractCanInterface::mountedComponents() const
    {
        return m_mountedComponents;
    }
}




