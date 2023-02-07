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

#ifndef CANINTERFACEHANDLE_H
#define CANINTERFACEHANDLE_H

#include "icaninterfacehandle.h"
#include "icaninterfacesharedptr.h"

namespace Lindwurm::Lib
{
    class ICanInterface;

    /**
     * @brief The CanInterfaceHandle class represents a handle to a mounted CAN interface and provides access to it.
     */
    class CanInterfaceHandle : public ICanInterfaceHandle
    {
        Q_OBJECT
        public:

            /**
             * @brief Constructs a CanInterfaceHandle for the interface and mounts it to the provided component.
             * @param interface the ICanInterface to handle.
             * @param component the component to which the handle is mounted.
             */
            CanInterfaceHandle(const std::shared_ptr<ICanInterface> &interface, const QString &component);

            /**
             * @brief Constructs an invalid (unmounted) CanInterfaceHandle.
             */
            CanInterfaceHandle();

            virtual ~CanInterfaceHandle();

            virtual QString     id() const override;
            virtual QString     name() const override;
            virtual QString     interfaceType() const override;
            virtual QString     device() const override;
            virtual bool        supportsFlexibleDataRate() const override;
            virtual bool        flexibleDataRateEnabled() const override;
            virtual int         bitRate() const override;
            virtual int         dataBitRate() const override;
            virtual bool        localEchoEnabled() const override;
            virtual bool        connected() const override;

            virtual bool        sendFrame(const QCanBusFrame &frame) override;

            virtual bool        unmount() override;
            virtual bool        isMounted() const override;

        private:

            ICanInterfaceSharedPtr  m_interface;
            QString                 m_mountedComponent;
    };
}

#endif // CANINTERFACEHANDLE_H
