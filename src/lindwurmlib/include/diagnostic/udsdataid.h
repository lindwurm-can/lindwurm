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

#ifndef UDSDATAID_H
#define UDSDATAID_H

#include "lindwurmlib_global.h"

#include <QString>
#include <QMap>
#include <QList>
#include "utils/range.h"
#include "qmetatype.h"

namespace Lindwurm::Lib
{
    using UdsDataIdRange    = Range<quint16>;

    class LINDWURMLIB_EXPORT UdsDataId
    {
        public:

            enum
            {
                BootSoftwareIdentification                  = 0xF180,
                ApplicationSoftwareIdentification           = 0xF181,
                ApplicationDataIdentification               = 0xF182,
                BootSoftwareFingerprint                     = 0xF183,
                ApplicationSoftwareFingerprint              = 0xF184,
                ApplicationDataFingerprint                  = 0xF185,
                ActiveDiagnosticSession                     = 0xF186,
                VehicleManufacturerSparePartNumber          = 0xF187,
                VehicleManufacturerECUSoftwareNumber        = 0xF188,
                VehicleManufacturerECUSoftwareVersionNumber = 0xF189,
                SystemSupplierIdentifierDataIdentifier      = 0xF18A,
                ECUManufacturingDate                        = 0xF18B,
                ECUSerialNumber                             = 0xF18C,
                SupportedFunctionalUnits                    = 0xF18D,
                VehicleManufacturerKitAssemblyPartNumber    = 0xF18E,
                ISOSAEReservedStandardized                  = 0xF18F,
                VIN                                         = 0xF190,
                VehicleManufacturerECUHardwareNumber        = 0xF191,
                SystemSupplierECUHardwareNumber             = 0xF192,
                SystemSupplierECUHardwareVersionNumber      = 0xF193,
                SystemSupplierECUSoftwareNumber             = 0xF194,
                SystemSupplierECUSoftwareVersionNumber      = 0xF195,
                ExhaustRegulationOrTypeApprovalNumber       = 0xF196,
                SystemNameOrEngineType                      = 0xF197,
                RepairShopCodeOrTesterSerialNumber          = 0xF198,
                ProgrammingDate                             = 0xF199,
                CalibrationRepairShopCodeOrCalibrationEquipmentSerialNumber = 0xF19A,
                CalibrationDate                             = 0xF19B,
                CalibrationEquipmentSoftwareNumber          = 0xF19C,
                ECUInstallationDate                         = 0xF19D,
                ODXFile                                     = 0xF19E,
                Entity                                      = 0xF19F,

                NumberOfEDRDevices                          = 0xFA10,
                EDRIdentification                           = 0xFA11,
                EDRDeviceAddressInformation                 = 0xFA12,

                EDREntry1                                   = 0xFA13,
                EDREntry2                                   = 0xFA14,
                EDREntry3                                   = 0xFA15,
                EDREntry4                                   = 0xFA16,
                EDREntry5                                   = 0xFA17,
                EDREntry6                                   = 0xFA18,

                UDSVersion                                  = 0xFF00
            };

            enum IdCategory
            {
                StandardIds,
                VehicleManufacturerSpecificIds,
                NetworkConfigDataForTrailerApplicationIds,
                IdentificationOptionIds,
                PeriodicIds,
                DynamicallyDefinedIds,
                ObdSpecificIds,
                TachographIds,
                AirbagDeploymentIds,
                SafetySystemIds,
                SystemSupplierSpecificIds,
                ReservedIds
            };

            UdsDataId();
            UdsDataId(quint16 id, const QString &description);

            QString                                     description() const;
            QString                                     id() const;
            quint16                                     value() const;

            static UdsDataId                            get(quint16 id);
            static void                                 add(const UdsDataId& dataId);
            static QMap<quint16, UdsDataId>             all();
            static QMap<QString, UdsDataId::IdCategory> idCategories();
            static QList<UdsDataIdRange>                rangesForCategory(IdCategory category);
            static QString                              descriptionForId(quint16 id);

        private:

            quint16                                     m_id = { 0 };
            QString                                     m_description = {};
            static QMap<quint16, UdsDataId>             s_byValue;
    };
}

#endif // UDSDATAID_H
