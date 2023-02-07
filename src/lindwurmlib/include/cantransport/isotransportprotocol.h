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

#ifndef ISOTRANSPORTPROTOCOL_H
#define ISOTRANSPORTPROTOCOL_H

#include "lindwurmlib_global.h"
#include <QObject>
#include <QCanBusFrame>
#include <QByteArray>

#include "cantransport/isotransportprotocolframe.h"
#include "caninterface/icaninterfacehandlesharedptr.h"

namespace Lindwurm::Lib
{
    class LINDWURMLIB_EXPORT IsoTransportProtocol : public QObject
    {
        Q_OBJECT
        public:

            explicit            IsoTransportProtocol(quint32 sourceAddress, quint32 targetAddress, QObject *parent = nullptr);

            enum class IsoTpState
            {
                Idle,
                Sending,
                Receiving
            };

            enum class IsoTpError
            {
                Timeout,
                DataError,
                MalformedFrame,
                UnexpectedFrame,
                OutOfOrderData
            };

            void                setPaddingEnabled(bool enablePadding);

            void                mountCANInterface(ICanInterfaceHandleSharedPtr interface);
            void                unmountCANInterface();

            quint32             sourceAddress() const;
            quint32             targetAddress() const;

            bool                sendData(const QByteArray &data);

        signals:

            void                errorOccurred(Lindwurm::Lib::IsoTransportProtocol::IsoTpError error);
            void                dataSent(const QByteArray &data);
            void                dataReceived(const QByteArray &data);


        private slots:

            void                canFrameReceived(const QCanBusFrame &frame, const QString &sourceInterface);

        private:

            bool                sendSingleFrame(const QByteArray &data);
            void                singleFrameReceived(IsoTransportProtocolFrame &tpFrame);
            void                firstFrameReceived(IsoTransportProtocolFrame &tpFrame);
            void                consecutiveFrameReceived(IsoTransportProtocolFrame &tpFrame);

            int                 paddingSize() const;
            void                resetState();

        private:

            IsoTpState                      m_state = { IsoTpState::Idle };
            bool                            m_usePadding = { true };

            quint32                         m_sourceAddress;
            quint32                         m_targetAddress;

            ICanInterfaceHandleSharedPtr    m_canInterface = {};

            int                             m_nextExpectedSequenceNumber = { 0 };
            int                             m_expectedDataLength = { 0 };
            QByteArray                      m_receiveBuffer = {};
    };
}

#endif // ISOTRANSPORTPROTOCOL_H
