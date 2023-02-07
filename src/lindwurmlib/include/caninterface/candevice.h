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

#ifndef CANDEVICE_H
#define CANDEVICE_H

#include "lindwurmlib_global.h"
#include "abstractcaninterface.h"

#include <QCanBusDevice>

namespace Lindwurm::Lib
{
    /**
     * @brief The CanDevice class is a ICanInterface wrapper for QCanBusDevice.
     *
     * Lindwurm uses the Qt CAN bus library to actually use CAN interface hardware. However, to
     * actually use and handle the interfaces within Lindwurm (and to support CAN bus bridging)
     * the ICanInterface wrapps the lower level access. CanDevice therefore is a wrapper for
     * any QCanBusDevice instance and provides the ICanInterface API.
     *
     * Generally, it is not neccessary to create a CanDevice instance directly. Instead use
     * CanInterfaceManager::addInterface() to instanciate a specific CAN interface.
     */
    class LINDWURMLIB_EXPORT CanDevice : public AbstractCanInterface
    {
        Q_OBJECT
        public:

            CanDevice(QCanBusDevice* canBusDevice, const QString &id, QString interfaceType, QString device);
            virtual ~CanDevice();

            virtual bool    connectInterface() override;
            virtual bool    disconnectInterface() override;

            virtual bool    supportsFlexibleDataRate() const override;
            virtual bool    flexibleDataRateEnabled() const override;
            virtual int     bitRate() const override;
            virtual int     dataBitRate() const override;
            virtual bool    localEchoEnabled() const override;
            virtual bool    connected() const override;

            virtual bool    sendFrame(const QCanBusFrame &frame) override;

            virtual void    setFlexibleDataRateEnabled(bool enabled) override;
            virtual void    setBitRate(int bitRate) override;
            virtual void    setDataBitRate(int dataBitRate) override;
            virtual void    setLocalEchoEnabled(bool enable) override;

        private slots:

            void            stateChanged(QCanBusDevice::CanBusDeviceState state);
            void            framesReceived();

        private:

            QCanBusDevice*  m_canBusDevice;
            bool            m_connected = {false};
    };
}

#endif // CANDEVICE_H
