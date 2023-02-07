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

#ifndef ABSTRACTCANINTERFACE_H
#define ABSTRACTCANINTERFACE_H

#include "lindwurmlib_global.h"
#include "icaninterface.h"
#include <QStringList>

namespace Lindwurm::Lib
{
    /**
     * @brief The AbstractCanInterface class implements the basics of the ICanInterface interface.
     *
     *  It provides a base implementation for CAN devices, which handles the basic attributes like id, name and
     *  component mounting. To implement specific CAN interfaces (such as CanDevice and CanBridge) subclass
     *  AbstractCanInterface and implement the specific inteface methods.
     */
    class LINDWURMLIB_EXPORT AbstractCanInterface : public ICanInterface
    {
        Q_OBJECT
        public:

            AbstractCanInterface(const QString &id, const QString &interfaceType, const QString &device);

        public:

            virtual QString         id() const override;
            virtual QString         name() const override;
            virtual QString         interfaceType() const override;
            virtual QString         device() const override;

            virtual void            setName(const QString &name) override;

            virtual void            mountComponent(const QString &component) override;
            virtual void            unmountComponent(const QString &component) override;

            virtual bool            isMounted() const override;
            virtual QStringList     mountedComponents() const override;

        protected:

            QString         m_id;
            QString         m_name;
            QString         m_interfaceType;
            QString         m_device;
            QStringList     m_mountedComponents;
    };
}

#endif // ABSTRACTCANINTERFACE_H
