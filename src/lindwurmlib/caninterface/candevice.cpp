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

#include "caninterface/candevice.h"
#include <QVariant>

namespace Lindwurm::Lib
{
    CanDevice::CanDevice(QCanBusDevice *canBusDevice, const QString &id, QString interfaceType, QString device)
        : AbstractCanInterface(id, interfaceType, device), m_canBusDevice(canBusDevice)
    {
        connect(m_canBusDevice, &QCanBusDevice::stateChanged, this, &CanDevice::stateChanged);
        connect(m_canBusDevice, &QCanBusDevice::framesReceived, this, &CanDevice::framesReceived);
    }

    CanDevice::~CanDevice()
    {
        disconnect(m_canBusDevice, &QCanBusDevice::stateChanged, this, &CanDevice::stateChanged);
        disconnect(m_canBusDevice, &QCanBusDevice::framesReceived, this, &CanDevice::framesReceived);

        delete m_canBusDevice;
    }

    bool CanDevice::supportsFlexibleDataRate() const
    {
        // TODO: QCanBusDevice does not seem to have a way to check the support for CAN FD
        return false;
    }

    bool CanDevice::flexibleDataRateEnabled() const
    {
        return m_canBusDevice->configurationParameter(QCanBusDevice::CanFdKey).toBool();
    }

    int CanDevice::bitRate() const
    {
        return m_canBusDevice->configurationParameter(QCanBusDevice::BitRateKey).toInt();
    }

    int CanDevice::dataBitRate() const
    {
        return m_canBusDevice->configurationParameter(QCanBusDevice::DataBitRateKey).toInt();
    }

    bool CanDevice::localEchoEnabled() const
    {
        return m_canBusDevice->configurationParameter(QCanBusDevice::LoopbackKey).toBool() ||
                m_canBusDevice->configurationParameter(QCanBusDevice::ReceiveOwnKey).toBool();
    }

    bool CanDevice::connected() const
    {
        return m_connected;
    }

    bool CanDevice::connectInterface()
    {
        if ( ! m_canBusDevice->connectDevice() )
        {
            return false;
        }

        m_connected = true;
        return true;
    }

    bool CanDevice::disconnectInterface()
    {
        m_canBusDevice->disconnectDevice();
        m_connected = false;
        return true;
    }

    bool CanDevice::sendFrame(const QCanBusFrame &frame)
    {
        return m_canBusDevice->writeFrame(frame);
    }

    void CanDevice::setFlexibleDataRateEnabled(bool enabled)
    {
        m_canBusDevice->setConfigurationParameter(QCanBusDevice::CanFdKey, enabled);
    }

    void CanDevice::setBitRate(int bitRate)
    {
        m_canBusDevice->setConfigurationParameter(QCanBusDevice::BitRateKey, bitRate);
    }

    void CanDevice::setDataBitRate(int dataBitRate)
    {
        m_canBusDevice->setConfigurationParameter(QCanBusDevice::DataBitRateKey, dataBitRate);
    }

    void CanDevice::setLocalEchoEnabled(bool enable)
    {
        m_canBusDevice->setConfigurationParameter(QCanBusDevice::LoopbackKey, enable);
        m_canBusDevice->setConfigurationParameter(QCanBusDevice::ReceiveOwnKey, enable);
    }

    void CanDevice::stateChanged(QCanBusDevice::CanBusDeviceState state)
    {
        switch (state)
        {
            case QCanBusDevice::ConnectedState:

                m_connected = true;
                emit interfaceConnected();
                break;

            case QCanBusDevice::UnconnectedState:

                m_connected = true;
                emit interfaceDisconnected();
                break;

            default:
                break;
        }
    }

    void CanDevice::framesReceived()
    {
        QVector<QCanBusFrame> frames = m_canBusDevice->readAllFrames();

       for (const QCanBusFrame &frame : frames)
       {
           emit frameReceived(frame, m_name );
       }
    }
}


