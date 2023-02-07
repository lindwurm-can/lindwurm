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

#ifndef UDSECUDISCOVERYSCANNER_H
#define UDSECUDISCOVERYSCANNER_H

#include "lindwurmlib_global.h"

#include <QObject>
#include <QMap>

#include "caninterface/icaninterfacehandlesharedptr.h"

class QThread;

namespace Lindwurm::Lib
{
    class UdsEcuDiscoveryScanWorker;

    /**
     * @brief The UdsEcuDiscoveryScanner class implements a scanner that discovers ECUs on the bus that reply to UDS commands.
     *
     * Depending on the used scan type, the scanner sends UDS requests to possible ECUs within the specified
     * address range and waits for corresponding responses. If a possible response was detected the scanner
     * increases it's timeouts and tries to verify the discovered ECU. If the response can be verified the scanner
     * reports the found ECU with it's source and target address.
     */
    class LINDWURMLIB_EXPORT UdsEcuDiscoveryScanner : public QObject
    {
        Q_OBJECT
        public:

            /**
             * @brief Constructs a UdsEcuDiscoveryScanner instance with the provided QObject parent
             * @param parent the QObject parent for this object.
             */
            explicit UdsEcuDiscoveryScanner(QObject *parent = nullptr);

            /**
             * @brief The ScanType enum describes the available scanning strategies.
             */
            enum class ScanType
            {
                DefaultDiagnosticSession, /*! The scanner sends a request to start a default UDS diagnostic session and waits for a positive ECU response. */
                InvalidCommand /*! The scanner sends an invalid command to an UDS address and waits for a negative ECU response. */
            };

            /**
             * @brief Returns the available scan types.
             * @return the available scan types.
             */
            QMap<QString, UdsEcuDiscoveryScanner::ScanType>    scanTypes() const;

            /**
             * @brief Mount the provided CAN interface to be used for scanning.
             * @param interface the interface to be used for scanning.
             */
            void    mountCANInterface(ICanInterfaceHandleSharedPtr interface);

            /**
             * @brief Unmount the interface used for scanning.
             */
            void    unmountCANInterface();

            /**
             * @brief Starts a scan within the provided address range.
             * @param startAddress the address where the scan should start.
             * @param endAddress the address where the scan should end.
             * @param type one of the types of ScanType
             */
            void    startDiscovery(quint32 startAddress, quint32 endAddress, ScanType type);

        signals:

            /**
             * @brief This signal is emitted when the discovery process was started.
             */
            void    discoveryStarted();

            /**
             * @brief This signal is emitted when the discovery process was finished.
             */
            void    discoveryFinished();

            /**
             * @brief This signal periodically provides the progress of the scan in percent.
             * @param precent the progress of the scan.
             */
            void    progress(int precent);

            /**
             * @brief This signal is emitted when a ECU was discovered.
             * @param sourceAddress the source address for the discovered device.
             * @param targetAddress the target address for the discovered device.
             */
            void    ecuDiscovered(quint32 sourceAddress, quint32 targetAddress);

        private slots:

            void    workerThreadFinished();

        private:

            ICanInterfaceHandleSharedPtr    m_canInterface = {};
            bool                            m_discoveryIsRunning = { false };
            UdsEcuDiscoveryScanWorker*      m_discoveryWorker = { nullptr };
            QThread*                        m_workerThread = { nullptr };
    };
}

#endif // UDSECUDISCOVERYSCANNER_H
