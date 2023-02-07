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

#ifndef ICANINTERFACE_H
#define ICANINTERFACE_H

#include "lindwurmlib_global.h"

#include <QObject>
#include <QCanBusFrame>
#include <QStringList>

namespace Lindwurm::Lib
{
    /**
     * @brief The ICanInterface class provides an interface specification to an actual CAN interface.
     *
     * While the ICanInterface represents the actual CAN interface it is generally not intented to
     * actually use the interface (e.g. send/receive frames from the bus). The ICanInterface is
     * commonly used by the CanInterfaceManager to access the interface and configure it's parameters
     * (e.g. such as bit rate, name, etc.).
     *
     * To actually use a CAN interface you have to obtain a ICanInterfaceHandle by mounting a CAN
     * interface via the ICanInterfaceManager.
     */
    class LINDWURMLIB_EXPORT ICanInterface : public QObject
    {
        Q_OBJECT
        public:

            explicit ICanInterface() : QObject(nullptr) {}
            virtual ~ICanInterface() {}

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
             * @brief Connects the CAN interface to the CAN bus
             * @return `true` if connecting to the bus was successfully initiated.
             */
            virtual bool        connectInterface() = 0;

            /**
             * @brief Disconnects the CAN interface from the CAN bus
             * @return `true` if disconnecting from the bus was successfully initiated.
             */
            virtual bool        disconnectInterface() = 0;

            /**
             * @brief Send the provided frame to the CAN bus
             * @param frame the QCanBusFrame to send.
             * @return true on success; otherwise false
             */
            virtual bool        sendFrame(const QCanBusFrame &frame) = 0;

            /**
             * @brief Sets the textual name of the CAN inteface
             * @param the name of the CAN inteface.
             */
            virtual void        setName(const QString &name) = 0;

            /**
             * @brief Enables or disables flexible data rate for this CAN interface.
             *
             * This has only an effect, if the inteface supports CAN FD.
             *
             * @param enabled set to `true` to enable CAN FD.
             */
            virtual void        setFlexibleDataRateEnabled(bool enabled) = 0;

            /**
             * @brief Sets the current bit rate of the CAN interface
             * @param bitRate the bitrate to use.
             */
            virtual void        setBitRate(int bitRate) = 0;

            /**
             * @brief Sets the current data bit rate of the CAN interface
             *
             * This has only an effect, if the inteface supports CAN FD.
             *
             * @param dataBitRate the data bitrate to use.
             */
            virtual void        setDataBitRate(int dataBitRate) = 0;

            /**
             * @brief Enables or disables the locale frame echo
             *
             * If local echo is enabled, each frame sent by the CAN interface is echoed and will be received back by this inteface.
             *
             * @param enable set to `true` to enable local frame echo.
             */
            virtual void        setLocalEchoEnabled(bool enable) = 0;

            /**
             * @brief Mount the CAN interface to the specified component
             * @param component the component name to mount
             */
            virtual void        mountComponent(const QString &component) = 0;

            /**
             * @brief Unmount the specified component from the CAN interface
             * @param component the component name to unmount
             */
            virtual void        unmountComponent(const QString &component) = 0;

            /**
             * @brief Returns `true` if the handle is still mounted and usable.
             * @return `true` if the handle is proper mounted.
             */
            virtual bool        isMounted() const = 0;

            /**
             * @brief Returns a list of components that currently are mounted to this interface
             * @return a list of currently mounted components.
             */
            virtual QStringList mountedComponents() const = 0;


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

#endif // ICANINTERFACE_H
