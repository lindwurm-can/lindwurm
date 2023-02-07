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

#ifndef UDSECUDISCOVERYSCANWORKER_H
#define UDSECUDISCOVERYSCANWORKER_H

#include <QObject>
#include <QByteArray>
#include <QTimer>
#include <QCanBusFrame>
#include <QSet>

#include "caninterface/icaninterfacehandlesharedptr.h"

namespace Lindwurm::Lib
{
    /**
     * @brief The UdsEcuDiscoveryScanWorker class implements the worker thread for the UdsEcuDiscoveryScanner class.
     */
    class UdsEcuDiscoveryScanWorker : public QObject
    {
        Q_OBJECT
        public:
            explicit UdsEcuDiscoveryScanWorker(ICanInterfaceHandleSharedPtr canInterface, quint32 startAddress, quint32 endAddress, int responseTimeoutMsec = 15, int verificationTimeoutMsec = 500, QObject *parent = nullptr);

        public slots:

            void    startDiscovery();

        signals:

            void    discoveryFinished();
            void    progress(int percent);
            void    ecuDiscovered(quint32 sourceAddress, quint32 targetAddress);

        private slots:

            void    canFrameReceived(const QCanBusFrame &frame, const QString &sourceInterface);
            void    scanCurrentAddress();
            void    responseTimeout();
            void    startVerification();

        private:

            bool    isValidResponse(const QCanBusFrame &frame) const;

            enum class DiscoveryState
            {
                Idle,
                Scanning,
                Verification
            };

            ICanInterfaceHandleSharedPtr    m_canInterface;
            quint32                         m_startAddress;
            quint32                         m_endAddress;
            int                             m_responseTimeoutMsec;
            int                             m_verificationTimeoutMsec;

            quint32                         m_endVerificationAddress = {0};
            quint32                         m_currentScannedAddress = {0};
            QByteArray                      m_startDiagnosticSessionPayload;
            QTimer                          m_responseTimeoutTimer;
            QTimer                          m_delayVerificationTimer;
            DiscoveryState                  m_discoveryState = { DiscoveryState::Idle };
            QSet<quint32>                   m_blacklistedIDs = {};
            int                             m_currentProgress = {0};
    };
}

#endif // UDSECUDISCOVERYSCANWORKER_H
