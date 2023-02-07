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

#include "diagnostic/udsecudiscoveryscanner.h"

#include "caninterface/icaninterfacehandle.h"
#include "udsecudiscoveryscanworker.h"
#include <QThread>

namespace Lindwurm::Lib
{
    UdsEcuDiscoveryScanner::UdsEcuDiscoveryScanner(QObject *parent)
        : QObject{parent}
    {

    }

    QMap<QString, UdsEcuDiscoveryScanner::ScanType> UdsEcuDiscoveryScanner::scanTypes() const
    {
        QMap<QString, UdsEcuDiscoveryScanner::ScanType> types;

        types.insert("Default session", ScanType::DefaultDiagnosticSession);
        //types.insert("Invalid command", ScanType::InvalidCommand);

        return types;
    }

    void UdsEcuDiscoveryScanner::mountCANInterface(ICanInterfaceHandleSharedPtr interface)
    {
        unmountCANInterface();

        m_canInterface = interface;
    }

    void UdsEcuDiscoveryScanner::unmountCANInterface()
    {
        if ( m_canInterface )
        {
            m_canInterface->unmount();
            m_canInterface.reset();
        }
    }

    void UdsEcuDiscoveryScanner::startDiscovery(quint32 startAddress, quint32 endAddress, ScanType type)
    {
        // we currently do not implemented different scan types
        Q_UNUSED(type)

        if ( m_discoveryIsRunning )
        {
            return;
        }

        if ( ! ( m_canInterface && m_canInterface->isMounted() ) )
        {
            return;
        }

        m_discoveryIsRunning = true;

        m_discoveryWorker = new UdsEcuDiscoveryScanWorker(m_canInterface, startAddress, endAddress);
        m_workerThread =    new QThread();

        m_discoveryWorker->moveToThread(m_workerThread);

        connect(m_workerThread,     &QThread::started,      m_discoveryWorker,  &UdsEcuDiscoveryScanWorker::startDiscovery);
        connect(m_workerThread,     &QThread::finished,     this,               &UdsEcuDiscoveryScanner::workerThreadFinished);

        connect(m_discoveryWorker,  &UdsEcuDiscoveryScanWorker::discoveryFinished,  m_workerThread, &QThread::quit);
        connect(m_discoveryWorker,  &UdsEcuDiscoveryScanWorker::progress,           this,           &UdsEcuDiscoveryScanner::progress);
        connect(m_discoveryWorker,  &UdsEcuDiscoveryScanWorker::ecuDiscovered,      this,           &UdsEcuDiscoveryScanner::ecuDiscovered);

        emit discoveryStarted();

        m_workerThread->start();
    }

    void UdsEcuDiscoveryScanner::workerThreadFinished()
    {
        emit discoveryFinished();

        m_workerThread->deleteLater();
        m_workerThread = nullptr;

        m_discoveryWorker->deleteLater();
        m_discoveryWorker = nullptr;

        m_discoveryIsRunning = false;
    }
}
