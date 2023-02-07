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

#ifndef CANBRIDGE_H
#define CANBRIDGE_H

#include "lindwurmlib_global.h"
#include "abstractcaninterface.h"
#include "icaninterfacesharedptr.h"
#include <QList>
#include <QStringList>

namespace Lindwurm::Lib
{
    /**
     * @brief The CanBridge class allows to bridge two or multiple CAN busses.
     *
     * Furthermore it is a special implementation of the ICanInterface interface and therfore
     * can be handled as any other CAN interface. So you can send a CAN frame to a bridge
     * and it will be broadcasted to all bridged interfaces. Also each frame which is received
     * on a mounted interface will be forwarded to all bridged interfaces.
     *
     * As all ICanInterface instances a CanBridge should only be created with
     * CanInterfaceManager::addInterface(). To mount and unmount specific interfaces from the bridge
     * use CanInterfaceManager::editInterface.
     */
    class LINDWURMLIB_EXPORT CanBridge : public AbstractCanInterface
    {
        Q_OBJECT
        public:

                            CanBridge(const QString &id);
            virtual         ~CanBridge();

            virtual bool    connectInterface() override;
            virtual bool    disconnectInterface() override;

            virtual bool    sendFrame(const QCanBusFrame &frame) override;
            virtual QString device() const override;

            virtual bool    connected() const override;
            virtual bool    supportsFlexibleDataRate() const override;
            virtual bool    flexibleDataRateEnabled() const override;
            virtual int     bitRate() const override;
            virtual int     dataBitRate() const override;
            virtual bool    localEchoEnabled() const override;

            virtual void    setFlexibleDataRateEnabled(bool enabled) override;
            virtual void    setBitRate(int bitRate) override;
            virtual void    setDataBitRate(int dataBitRate) override;
            virtual void    setLocalEchoEnabled(bool enable) override;

            /**
             * @brief Mount a specific interface to the bridge
             * @param interface the interface to be mounted
             * @return `true` if the mount was successful.
             */
            bool            mountInterface(const ICanInterfaceSharedPtr &interface);

            /**
             * @brief Unmount a specific interface to the bridge
             * @param interface the interface to be unmounted
             * @return `true` if the unmount was successful.
             */
            bool            unmountInterface(const ICanInterfaceSharedPtr &interface);

            /**
             * @brief Provides a list with all interface ids mounted to the bridge.
             * @return all interfaces mounted to the bridge.
             */
            QStringList     bridgedInterfaceIds() const;

        private slots:

            void            frameReceivedForBridge(const QCanBusFrame &frame, const QString &sourceInterface);

        private:

            bool                                m_bridgeConnected = {false};
            QList<ICanInterfaceSharedPtr>       m_mountedInterfaces = {};
    };
}

#endif // CANBRIDGE_H
