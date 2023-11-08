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
#include <QTimer>

#include "cantransport/isotransportprotocolframe.h"
#include "caninterface/icaninterfacehandlesharedptr.h"

namespace Lindwurm::Lib
{
    class LINDWURMLIB_EXPORT IsoTransportProtocol : public QObject
    {
        Q_OBJECT
        public:

            explicit            IsoTransportProtocol(quint32 sourceAddress, quint32 targetAddress, QObject *parent = nullptr);

            enum class IsoTpError
            {
                Timeout,
                DataError,
                MalformedFrame,
                UnexpectedFrame,
                OutOfOrderData,
                OverFlow
            };

            void                mountCANInterface(ICanInterfaceHandleSharedPtr interface);
            void                unmountCANInterface();

            void                setPaddingEnabled(bool enablePadding);

            quint32             sourceAddress() const;
            quint32             targetAddress() const;

            bool                sendData(const QByteArray &data);

        signals:

            void                errorOccurred(Lindwurm::Lib::IsoTransportProtocol::IsoTpError error);
            void                dataSent(const QByteArray &data);
            void                dataReceived(const QByteArray &data);

        private slots:

            void                sendTimeout();
            void                receiveTimeout();
            void                canFrameReceived(const QCanBusFrame &frame, const QString &sourceInterface);

            bool                sendNextConsecutiveFrame();
            void                continueSending();

        private:

            int                 paddingSize() const;

            void                resetSendConnection();
            void                resetReceiveConnection();

            void                singleFrameReceived(IsoTransportProtocolFrame &tpFrame);
            void                firstFrameReceived(IsoTransportProtocolFrame &tpFrame);
            void                consecutiveFrameReceived(IsoTransportProtocolFrame &tpFrame);

            void                sendFlowControlFrame();

            bool                sendSingleFrame(const QByteArray &data);
            bool                sendFirstFrame();

            void                flowControlFrameReceived(IsoTransportProtocolFrame &tpFrame);

        private:

            enum class IsoTpConnectionState
            {
                Idle,
                Transmission
            };

            /**
             * @brief The IsoTpConnection struct stores all attributes of one directed TP connection.
             */
            struct IsoTpConnection
            {
                IsoTpConnectionState        state;
                int                         dataLength;
                QByteArray                  data;

                quint8                      blockSize;
                quint8                      minSeparationTime;

                quint8                      currentSequenceNumber;
                quint8                      currentBlockNumber;
                quint8                      currentWaitCycle;

                QTimer                      timeoutTimer;
            };

            IsoTpConnection                 m_receiveConnection;
            IsoTpConnection                 m_sendConnection;

            ICanInterfaceHandleSharedPtr    m_canInterface = {};

            bool                            m_usePadding = { true };

            // TODO: While sending we currently remove every sent part of the data from the connection's byte array
            // To be able to emit the dataSend signal on a successful transmission with the original data, we maintain a temporary copy here
            // maybe we can refactor this to keep the connection's data buffer intact and use an offset to the remaining data in the array
            QByteArray                      m_dataToSentCopy;

            QTimer                          m_separationTimer;

            quint32                         m_sourceAddress;
            quint32                         m_targetAddress;
    };
}

#endif // ISOTRANSPORTPROTOCOL_H
