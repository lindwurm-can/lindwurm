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

#include "udsecudiscoveryscanworker.h"

#include "caninterface/icaninterfacehandle.h"

Q_DECLARE_METATYPE(QCanBusFrame)

namespace Lindwurm::Lib
{
    UdsEcuDiscoveryScanWorker::UdsEcuDiscoveryScanWorker(ICanInterfaceHandleSharedPtr canInterface, quint32 startAddress, quint32 endAddress, int responseTimeoutMsec, int verificationTimeoutMsec, QObject *parent)
        : QObject{parent}
        , m_canInterface(canInterface)
        , m_startAddress(startAddress)
        , m_endAddress(endAddress)
        , m_responseTimeoutMsec(responseTimeoutMsec)
        , m_verificationTimeoutMsec(verificationTimeoutMsec)
        , m_startDiagnosticSessionPayload( QByteArrayLiteral("\x02\x10\x01\x00\x00\x00\x00\x00") )
        , m_responseTimeoutTimer(this)
        , m_delayVerificationTimer(this)
    {
        qRegisterMetaType<QCanBusFrame>();

        m_responseTimeoutTimer.setSingleShot(true);
        m_responseTimeoutTimer.setInterval(m_responseTimeoutMsec);

        m_delayVerificationTimer.setSingleShot(true);
        m_delayVerificationTimer.setInterval(500);
    }

    void UdsEcuDiscoveryScanWorker::startDiscovery()
    {
        m_currentScannedAddress = m_startAddress;
        m_discoveryState        = DiscoveryState::Scanning;

        connect(m_canInterface.get(),       &ICanInterfaceHandle::frameReceived, this, &UdsEcuDiscoveryScanWorker::canFrameReceived);

        connect(&m_responseTimeoutTimer,    &QTimer::timeout, this, &UdsEcuDiscoveryScanWorker::responseTimeout);
        connect(&m_delayVerificationTimer,  &QTimer::timeout, this, &UdsEcuDiscoveryScanWorker::startVerification);

        m_responseTimeoutTimer.setInterval(m_responseTimeoutMsec);

        scanCurrentAddress();
    }

    bool UdsEcuDiscoveryScanWorker::isValidResponse(const QCanBusFrame &frame) const
    {
        QByteArray payload = frame.payload();

        if ( payload.length() >= 2 )
        {
            if ( payload.at(1) == 0x50 && payload.at(2) == 0x01 )
            {
                return true;
            }
        }

        return false;
    }

    void UdsEcuDiscoveryScanWorker::canFrameReceived(const QCanBusFrame &frame, const QString &sourceInterface)
    {
        Q_UNUSED(sourceInterface)

        if ( (! frame.isValid() ) || frame.hasLocalEcho() )
        {
            // this is a self-sent frame
            return;
        }

        if ( ! isValidResponse(frame) )
        {
            // ignore any frames that are not a valid response to our diagnostic request
            return;
        }

        m_responseTimeoutTimer.stop();

        switch ( m_discoveryState )
        {
            case DiscoveryState::Scanning:

                // found possible valid response, back up and verify response

                // we don't want to start verification immediately, just idle a small amount of time
                // to clean the receiving queue from possible interfere responses
                m_discoveryState = DiscoveryState::Idle;
                m_delayVerificationTimer.start();

                break;

            case DiscoveryState::Verification:

                emit ecuDiscovered(m_currentScannedAddress, frame.frameId() );

                if ( m_currentScannedAddress == m_endAddress )
                {
                    emit discoveryFinished();
                    m_discoveryState = DiscoveryState::Idle;

                    return;
                }

                // don't scan the found address again
                m_blacklistedIDs.insert(m_currentScannedAddress);

                // continue with normal scanning mode
                m_discoveryState = DiscoveryState::Scanning;

                m_responseTimeoutTimer.setInterval(m_responseTimeoutMsec);

                // we continue scanning with the next address after the found ECU address
                m_currentScannedAddress++;

                scanCurrentAddress();

                break;

            case DiscoveryState::Idle:

                // ignore any valid responses in idle state (e.g. when waiting to start a verification)

                break;
        }
    }

    void UdsEcuDiscoveryScanWorker::scanCurrentAddress()
    {
        // if current address is blacklisted find the next address to scan
        while ( m_blacklistedIDs.contains(m_currentScannedAddress) )
        {
            m_currentScannedAddress++;

            if (m_currentScannedAddress > m_endAddress )
            {
                emit discoveryFinished();
                m_discoveryState = DiscoveryState::Idle;

                return;
            }
        }

        QCanBusFrame    frame(m_currentScannedAddress, m_startDiagnosticSessionPayload);

        int percentProgess = 100.0f / (m_endAddress - m_startAddress) * (m_currentScannedAddress - m_startAddress);

        // because we decrement the current scanned address in verification step (which also decrements the progress)
        // we only emit if percentage is greater than last time
        if ( percentProgess > m_currentProgress )
        {
            m_currentProgress = percentProgess;
            emit progress(m_currentProgress);
        }

        m_canInterface->sendFrame(frame);
        m_responseTimeoutTimer.start();
    }

    void UdsEcuDiscoveryScanWorker::responseTimeout()
    {
        if ( m_currentScannedAddress == m_endAddress )
        {
            // the last address to be scanned timed out, so we're finished
            emit discoveryFinished();
            m_discoveryState = DiscoveryState::Idle;

            return;
        }

        if ( ( m_discoveryState == DiscoveryState::Verification) && (m_currentScannedAddress == m_endVerificationAddress) )
        {
            // verification failed, go back to normal scanning
            m_discoveryState = DiscoveryState::Scanning;
            m_responseTimeoutTimer.setInterval(m_responseTimeoutMsec);
        }

        m_currentScannedAddress++;
        scanCurrentAddress();
    }

    void UdsEcuDiscoveryScanWorker::startVerification()
    {
        m_discoveryState = DiscoveryState::Verification;

        // we end the verification step at the current scanned address
        m_endVerificationAddress = m_currentScannedAddress;

        // we back up a few addresses and scan these addresses again with a larger timeout for verification
        m_currentScannedAddress -= 5;

        m_responseTimeoutTimer.setInterval(m_verificationTimeoutMsec);
        scanCurrentAddress();
    }
}
