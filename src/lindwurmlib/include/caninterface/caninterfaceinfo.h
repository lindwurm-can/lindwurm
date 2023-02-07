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

#ifndef CANINTERFACEINFO_H
#define CANINTERFACEINFO_H

#include "lindwurmlib_global.h"

#include <QString>
#include <memory>

namespace Lindwurm::Lib
{
    class ICanInterface;

    /**
     * @brief The CanInterfaceInfo class provides general information about a specific CAN bus interface.
     *
     * This class provides only a view for the information about a specific CAN bus interface.
     */
    class LINDWURMLIB_EXPORT CanInterfaceInfo
    {
        public:

            /**
             * @brief Constructs a default but invalid CanInterfaceInfo.
             */
            CanInterfaceInfo();

            /**
             * @brief Constructs a CanInterfaceInfo object for the provided ICanInterface.
             * @param interface the ICanInterface represented by the CanInterfaceInfo.
             */
            CanInterfaceInfo(const std::weak_ptr<ICanInterface>& interface);

            /**
             * @brief Copy constructs a CanInterfaceInfo
             * @param other the CanInterfaceInfo to copy
             */
            CanInterfaceInfo(const CanInterfaceInfo& other);

            ~CanInterfaceInfo();

            CanInterfaceInfo& operator=(const CanInterfaceInfo& other);

            /**
             * @brief Returns the id of the CAN interface
             * @return the id of the represented CAN interface.
             */
            QString     id() const;

            /**
             * @brief Returns the textual name of the represented CAN inteface
             * @return the name of the represented CAN inteface.
             */
            QString     name() const;

            /**
             * @brief Returns the type of the represented CAN interface
             * @return the type of the represented CAN interface.
             */
            QString     interfaceType() const;

            /**
             * @brief Returns the device descriptor of the represented CAN interface
             * @return the device descriptor of the represented CAN interface.
             */
            QString     device() const;

            /**
             * @brief Returns `true` if the represented CAN interface supports CAN FD
             * @return `true` if the represented CAN interface supports CAN FD.
             */
            bool        supportsFlexibleDataRate() const;

            /**
             * @brief Returns `true` if the represented CAN interface has CAN FD enabled
             * @return `true` if the represented CAN interface has CAN FD enabled.
             */
            bool        flexibleDataRateEnabled() const;

            /**
             * @brief Returns the current set bitrate of the represented CAN interface
             * @return the current set bitrate of the represented CAN interface.
             */
            int         bitRate() const;

            /**
             * @brief Returns the current set data bitrate of the represented CAN interface.
             *
             * This value is only valid, if the represented interface supports CAN FD and flexible data rate is enabled. Otherwise `0` is returned.
             *
             * @return the current set data bitrate of the represented CAN interface.
             */
            int         dataBitRate() const;

            /**
             * @brief Returns `true` if local echo is enabled
             *
             * If local echo is enabled, each frame sent by the represented CAN interface is echoed and will be received back by this inteface.
             *
             * @return `true` if local echo is enabled.
             */
            bool        localEchoEnabled() const;

            /**
             * @brief Returns `true` if the represented CAN inteface is currently connected to the bus
             * @return `true` if the represented CAN inteface is currently connected to the bus.
             */
            bool        connected() const;

            /**
             * @brief Returns `true` if the CanInterfaceInfo validly represents a CAN interface.
             * @return `true` if the CanInterfaceInfo validly represents a CAN interface.
             */
            bool        isValid() const;

        private:

            std::weak_ptr<ICanInterface>    m_Interface;
    };
}


#endif // CANINTERFACEINFO_H
