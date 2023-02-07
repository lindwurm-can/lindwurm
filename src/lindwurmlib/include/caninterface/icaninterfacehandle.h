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

#ifndef ICANINTERFACEHANDLE_H
#define ICANINTERFACEHANDLE_H

#include "lindwurmlib_global.h"
#include <QObject>
#include <QString>
#include <QCanBusFrame>

#include "icaninterfacehandlesharedptr.h"

namespace Lindwurm::Lib
{
    /**
     * @brief The ICanInterfaceHandle class defines the class interface to a CAN interface handle.
     *
     * A CAN interface handle (implemented in the class CanInterfaceHandle) represents a handle to a
     * mounted CAN interface and provides access to it. This class defines the interface of a handle
     * and provides the basic methods to work with a CAN interface (e.g. get the properties of a CAN
     * interface and receive and send CAN frames to the interface). A handle can be obtained by
     * mounting a CAN interface via the ICanInterfaceManager.
     *
     * As the handle represents only a mounted interface it does not allow to modify the actual CAN
     * interface (such as e.g. changing the bit rate or disconnecting the bus). Modifying the actual
     * interface is only supported by the CanInterfaceManager via the ICanInterface interface.
     */
    class LINDWURMLIB_EXPORT ICanInterfaceHandle : public QObject
    {
        Q_OBJECT
        public:

            ICanInterfaceHandle() : QObject(nullptr) {}

            virtual             ~ICanInterfaceHandle() {}

            /**
             * @brief Returns the id of the CAN interface
             * @return the id of the CAN interface.
             */
            virtual QString     id() const = 0;

            /**
             * @brief Returns the textual name of the CAN inteface
             * @return the name of the CAN inteface.
             */
            virtual QString     name() const = 0;

            /**
             * @brief Returns the type of the CAN interface
             * @return the type of the CAN interface.
             */
            virtual QString     interfaceType() const = 0;

            /**
             * @brief Returns the device descriptor of the CAN interface
             * @return the device descriptor of the CAN interface.
             */
            virtual QString     device() const = 0;

            /**
             * @brief Returns `true` if the CAN interface supports CAN FD
             * @return `true` if the CAN interface supports CAN FD.
             */
            virtual bool        supportsFlexibleDataRate() const = 0;

            /**
             * @brief Returns `true` if the CAN interface has CAN FD enabled
             * @return `true` if the CAN interface has CAN FD enabled.
             */
            virtual bool        flexibleDataRateEnabled() const = 0;

            /**
             * @brief Returns the current set bitrate of the CAN interface
             * @return the current set bitrate of the CAN interface.
             */
            virtual int         bitRate() const = 0;

            /**
             * @brief Returns the current set data bitrate of the CAN interface.
             *
             * This value is only valid, if the interface supports CAN FD and flexible data rate is enabled. Otherwise `0` is returned.
             *
             * @return the current set data bitrate of the CAN interface.
             */
            virtual int         dataBitRate() const = 0;

            /**
             * @brief Returns `true` if local echo is enabled
             *
             * If local echo is enabled, each frame sent by the CAN interface is echoed and will be received back by this inteface.
             *
             * @return `true` if local echo is enabled.
             */
            virtual bool        localEchoEnabled() const = 0;

            /**
             * @brief Returns `true` if the CAN inteface is currently connected to the bus
             * @return `true` if the CAN inteface is currently connected to the bus.
             */
            virtual bool        connected() const = 0;

            /**
             * @brief Send the provided frame to the CAN bus
             * @param frame the QCanBusFrame to send.
             * @return true on success; otherwise false
             */
            virtual bool        sendFrame(const QCanBusFrame &frame) = 0;

            /**
             * @brief Unmount the handle from the actual CAN interface.
             *
             * If you no longer need access to a CAN interface you have to unmount it. This disconnects
             * the handle from the actual interface and invalidates the handle. If you need access again,
             * you need to obtain a new handle by mounting the CAN interface again.
             *
             * @return true on success; otherwise false
             */
            virtual bool        unmount() = 0;

            /**
             * @brief Returns `true` if the handle is still mounted and usable.
             * @return `true` if the handle is proper mounted.
             */
            virtual bool        isMounted() const = 0;

        signals:

            /**
             * @brief This signal is emitted when the CAN inteface was connected to the bus
             */
            void                interfaceConnected();

            /**
             * @brief This signal is emitted when the CAN inteface was disconnected from the bus
             */
            void                interfaceDisconnected();

            /**
             * @brief This signal is emitted when a CAN frame have been received
             * @param frame             the received CAN frame
             * @param sourceInterface   the textual name of the source CAN interface
             */
            void                frameReceived(const QCanBusFrame &frame, const QString &sourceInterface);
    };
}

#endif // ICANINTERFACEHANDLE_H
