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

#ifndef ICANINTERFACEMANAGER_H
#define ICANINTERFACEMANAGER_H

#include "lindwurmlib_global.h"
#include <QObject>
#include <QStringList>
#include <QList>

#include "caninterfaceinfo.h"
#include "caninterfaceconfig.h"
#include "icaninterfacehandlesharedptr.h"

class QAbstractItemModel;

namespace Lindwurm::Lib
{
    /**
     * @brief The ICanInterfaceManager class provides an interface to manage the available CAN interfaces.
     */
    class LINDWURMLIB_EXPORT ICanInterfaceManager : public QObject
    {
        Q_OBJECT
        public:

            /**
             * @brief The InterfaceError enum describes all possible interface errors
             */
            enum class InterfaceError
            {
                NoError,                    /*! No errors have occured. */
                DuplicateInterfaceName,     /*! This interface name is already used. */
                DeviceNotFound,             /*! The specified device was not found. */
                CanFDNotSupported,          /*! CAN FD is not supported by this device. */
                LocalEchoNotSupported,      /*! Local echo is not supported by this device. */
                InterfaceMounted,           /*! The interface is still mounted. */
                InterfaceNotFound,          /*! The specified interface was not found. */
                UnknownError                /*! An unknown error occured. */
            };

                                                    ICanInterfaceManager(QObject *parent = nullptr);

            /**
             * @brief Returns the single instance of the ICanInterfaceManager class.
             * @return the single instance of the ICanInterfaceManager class.
             */
            static ICanInterfaceManager*            instance() { return s_instance; }

            /**
             * @brief Get the number of available CAN interfaces
             * @return the number of currently available CAN interfaces
             */
            virtual int                             numberOfInterfaces(void) const = 0;
            virtual QStringList                     interfaceTypes() const = 0;
            virtual QStringList                     availableDevicesOf(const QString &interfaceType) const = 0;
            virtual QList<CanInterfaceInfo>         availableInterfaces() const = 0;

            /**
             * @brief Get the detailed information of a specific interface.
             * @param interfaceId   the ID of the interface.
             * @return the detailed information for the specific interface.
             */
            virtual CanInterfaceInfo                interfaceInfoFor(const QString &interfaceId) = 0;

            /**
             * @brief Get a list model for the available CAN interfaces.
             *
             * The obtained model is shared and owned by the CanInterfaceManager and therefore must not be deleted!
             *
             * @return  the list model.
             */
            virtual QAbstractItemModel*             interfaceListModel() const = 0;

            /**
             * @brief Mounts a CAN interface to a component and returns a handle to it.
             * @param interfaceId   the ID of the interface to mount.
             * @param component     the component to which the interface is to be mounted.
             * @return              a handle to the mounted interface.
             */
            virtual ICanInterfaceHandleSharedPtr    mountInterface(const QString &interfaceId, const QString &component) = 0;

        signals:

            /**
             * @brief The interfaceAdded signal is emitted when a new CAN interface was created.
             * @param interface     the CanInterfaceInfo for the new interface
             */
            void    interfaceAdded(     const Lindwurm::Lib::CanInterfaceInfo  &interface);

            /**
             * @brief The interfaceModified signal is emitted when a CAN interface was modified.
             * @param interface     the CanInterfaceInfo for the modified interface
             */
            void    interfaceModified(  const Lindwurm::Lib::CanInterfaceInfo  &interface);

            /**
             * @brief The interfaceRemoved signal is emitted when a CAN interface was removed.
             * @param interfaceId     the Id of the removed interface
             */
            void    interfaceRemoved(   const QString           &interfaceId);

        protected:

            static ICanInterfaceManager*            s_instance;
    };
}

#endif // ICANINTERFACEMANAGER_H
