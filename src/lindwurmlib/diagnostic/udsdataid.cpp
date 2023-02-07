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

#include "diagnostic/udsdataid.h"

namespace Lindwurm::Lib
{
    QMap<quint16, UdsDataId> UdsDataId::s_byValue
    {
        { UdsDataId::BootSoftwareIdentification,                    {UdsDataId::BootSoftwareIdentification, "BootSoftwareIdentification"} },
        { UdsDataId::ApplicationSoftwareIdentification,             {UdsDataId::ApplicationSoftwareIdentification, "ApplicationSoftwareIdentification"} },
        { UdsDataId::ApplicationDataIdentification,                 {UdsDataId::ApplicationDataIdentification, "ApplicationDataIdentification"} },
        { UdsDataId::BootSoftwareFingerprint,                       {UdsDataId::BootSoftwareFingerprint, "BootSoftwareFingerprint"} },
        { UdsDataId::ApplicationSoftwareFingerprint,                {UdsDataId::ApplicationSoftwareFingerprint, "ApplicationSoftwareFingerprint"} },
        { UdsDataId::ApplicationDataFingerprint,                    {UdsDataId::ApplicationDataFingerprint, "ApplicationDataFingerprint"} },
        { UdsDataId::ActiveDiagnosticSession,                       {UdsDataId::ActiveDiagnosticSession, "ActiveDiagnosticSession"} },
        { UdsDataId::VehicleManufacturerSparePartNumber,            {UdsDataId::VehicleManufacturerSparePartNumber, "VehicleManufacturerSparePartNumber"} },
        { UdsDataId::VehicleManufacturerECUSoftwareNumber,          {UdsDataId::VehicleManufacturerECUSoftwareNumber, "VehicleManufacturerECUSoftwareNumber"} },
        { UdsDataId::VehicleManufacturerECUSoftwareVersionNumber,   {UdsDataId::VehicleManufacturerECUSoftwareVersionNumber, "VehicleManufacturerECUSoftwareVersionNumber"} },
        { UdsDataId::SystemSupplierIdentifierDataIdentifier,        {UdsDataId::SystemSupplierIdentifierDataIdentifier, "SystemSupplierIdentifierDataIdentifier"} },
        { UdsDataId::ECUManufacturingDate,                          {UdsDataId::ECUManufacturingDate, "ECUManufacturingDate"} },
        { UdsDataId::ECUSerialNumber,                               {UdsDataId::ECUSerialNumber, "ECUSerialNumber"} },
        { UdsDataId::SupportedFunctionalUnits,                      {UdsDataId::SupportedFunctionalUnits, "SupportedFunctionalUnits"} },
        { UdsDataId::VehicleManufacturerKitAssemblyPartNumber,      {UdsDataId::VehicleManufacturerKitAssemblyPartNumber, "VehicleManufacturerKitAssemblyPartNumber"} },
        { UdsDataId::ISOSAEReservedStandardized,                    {UdsDataId::ISOSAEReservedStandardized, "ISOSAEReservedStandardized"} },
        { UdsDataId::VIN,                                           {UdsDataId::VIN, "VIN"} },
        { UdsDataId::VehicleManufacturerECUHardwareNumber,          {UdsDataId::VehicleManufacturerECUHardwareNumber, "VehicleManufacturerECUHardwareNumber"} },
        { UdsDataId::SystemSupplierECUHardwareNumber,               {UdsDataId::SystemSupplierECUHardwareNumber, "SystemSupplierECUHardwareNumber"} },
        { UdsDataId::SystemSupplierECUHardwareVersionNumber,        {UdsDataId::SystemSupplierECUHardwareVersionNumber, "SystemSupplierECUHardwareVersionNumber"} },
        { UdsDataId::SystemSupplierECUSoftwareNumber,               {UdsDataId::SystemSupplierECUSoftwareNumber, "SystemSupplierECUSoftwareNumber"} },
        { UdsDataId::SystemSupplierECUSoftwareVersionNumber,        {UdsDataId::SystemSupplierECUSoftwareVersionNumber, "SystemSupplierECUSoftwareVersionNumber"} },
        { UdsDataId::ExhaustRegulationOrTypeApprovalNumber,         {UdsDataId::ExhaustRegulationOrTypeApprovalNumber, "ExhaustRegulationOrTypeApprovalNumber"} },
        { UdsDataId::SystemNameOrEngineType,                        {UdsDataId::SystemNameOrEngineType, "SystemNameOrEngineType"} },
        { UdsDataId::RepairShopCodeOrTesterSerialNumber,            {UdsDataId::RepairShopCodeOrTesterSerialNumber, "RepairShopCodeOrTesterSerialNumber"} },
        { UdsDataId::ProgrammingDate,                               {UdsDataId::ProgrammingDate, "ProgrammingDate"} },
        { UdsDataId::CalibrationRepairShopCodeOrCalibrationEquipmentSerialNumber, {UdsDataId::CalibrationRepairShopCodeOrCalibrationEquipmentSerialNumber, "CalibrationRepairShopCodeOrCalibrationEquipmentSerialNumber"} },
        { UdsDataId::CalibrationDate,                               {UdsDataId::CalibrationDate, "CalibrationDate"} },
        { UdsDataId::CalibrationEquipmentSoftwareNumber,            {UdsDataId::CalibrationEquipmentSoftwareNumber, "CalibrationEquipmentSoftwareNumber"} },
        { UdsDataId::ECUInstallationDate,                           {UdsDataId::ECUInstallationDate, "ECUInstallationDate"} },
        { UdsDataId::ODXFile,                                       {UdsDataId::ODXFile, "ODXFile"} },
        { UdsDataId::Entity,                                        {UdsDataId::Entity, "Entity"} },

        { UdsDataId::NumberOfEDRDevices,                            {UdsDataId::NumberOfEDRDevices, "NumberOfEDRDevices"} },
        { UdsDataId::EDRIdentification,                             {UdsDataId::EDRIdentification, "EDRIdentification"} },
        { UdsDataId::EDRDeviceAddressInformation,                   {UdsDataId::EDRDeviceAddressInformation, "EDRDeviceAddressInformation"} },

        { UdsDataId::EDREntry1,                                     {UdsDataId::EDREntry1, "EDREntry1"} },
        { UdsDataId::EDREntry2,                                     {UdsDataId::EDREntry2, "EDREntry2"} },
        { UdsDataId::EDREntry3,                                     {UdsDataId::EDREntry3, "EDREntry3"} },
        { UdsDataId::EDREntry4,                                     {UdsDataId::EDREntry4, "EDREntry4"} },
        { UdsDataId::EDREntry5,                                     {UdsDataId::EDREntry5, "EDREntry5"} },
        { UdsDataId::EDREntry6,                                     {UdsDataId::EDREntry6, "EDREntry6"} },

        { UdsDataId::UDSVersion,                                    {UdsDataId::UDSVersion, "UDSVersion"} }
    };

    UdsDataId::UdsDataId()
    {

    }

    UdsDataId::UdsDataId(quint16 id, const QString &description)
        : m_id(id), m_description(description)
    {

    }

    QString UdsDataId::description() const
    {
        if ( ! m_description.isEmpty() )
        {
            return m_description;
        }

        return descriptionForId(m_id);
    }

    QString UdsDataId::id() const
    {
        return "0x" + QString::number(m_id, 16).toUpper().rightJustified(4, '0');
    }

    quint16 UdsDataId::value() const
    {
        return m_id;
    }

    UdsDataId UdsDataId::get(quint16 id)
    {
        // if the requested id is not in the map, we construct a default UdsDataId object with this id and an empty description
        // a description is then returned based on the range in which this id is located.
        return s_byValue.value(id, UdsDataId(id, "") );
    }

    void UdsDataId::add(const UdsDataId &dataId)
    {
        s_byValue.insert(dataId.value(), dataId);
    }

    QMap<quint16, UdsDataId> UdsDataId::all()
    {
        return s_byValue;
    }

    QMap<QString, UdsDataId::IdCategory> UdsDataId::idCategories()
    {
        QMap<QString, UdsDataId::IdCategory> categories;

        categories.insert("Standard IDs",                       IdCategory::StandardIds);
        categories.insert("Vehicle manufacturer specific IDs",  IdCategory::VehicleManufacturerSpecificIds);
        categories.insert("Network config for trailer IDs",     IdCategory::NetworkConfigDataForTrailerApplicationIds);
        categories.insert("Identification option IDs",          IdCategory::IdentificationOptionIds);
        categories.insert("Periodic IDs",                       IdCategory::PeriodicIds);
        categories.insert("Dynamically defined IDs",            IdCategory::DynamicallyDefinedIds);
        categories.insert("OBD specific IDs",                   IdCategory::ObdSpecificIds);
        categories.insert("Tachograph IDs",                     IdCategory::TachographIds);
        categories.insert("Airbag deployment IDs",              IdCategory::AirbagDeploymentIds);
        categories.insert("Safety system IDs",                  IdCategory::SafetySystemIds);
        categories.insert("System supplier specific IDs",       IdCategory::SystemSupplierSpecificIds);
        categories.insert("Reserved IDs",                       IdCategory::ReservedIds);

        return categories;
    }

    QList<UdsDataIdRange> UdsDataId::rangesForCategory(IdCategory category)
    {
        QList<UdsDataIdRange> result;

        switch (category)
        {
            case IdCategory::StandardIds:
                result.append( UdsDataIdRange(0xF180, 0xF19F) );    // Standard
                result.append( UdsDataIdRange(0xFA10, 0xFA18) );    // EDR Devices
                result.append( UdsDataIdRange(0xFF00, 0xFF00) );    // UDS Version
                break;

            case IdCategory::ReservedIds:
                result.append( UdsDataIdRange(0x0000, 0x00FF) );    // ISOSAEReserved
                result.append( UdsDataIdRange(0xFF01, 0xFFFF) );    // ISOSAEReserved
                result.append( UdsDataIdRange(0xF500, 0xF5FF) );    // OBDDataIdentifier Reserved
                result.append( UdsDataIdRange(0xF700, 0xF7FF) );    // OBDMonitor Reserved
                break;

            case IdCategory::VehicleManufacturerSpecificIds:
                result.append( UdsDataIdRange(0x0100, 0xA5FF) );
                result.append( UdsDataIdRange(0xA800, 0xACFF) );
                result.append( UdsDataIdRange(0xB000, 0xB1FF) );
                result.append( UdsDataIdRange(0xC000, 0xC2FF) );
                result.append( UdsDataIdRange(0xCF00, 0xEFFF) );
                result.append( UdsDataIdRange(0xF010, 0xF0FF) );
                break;

            case IdCategory::NetworkConfigDataForTrailerApplicationIds:
                result.append( UdsDataIdRange(0xF000, 0xF00F) );
                break;

            case IdCategory::IdentificationOptionIds:
                result.append( UdsDataIdRange(0xF100, 0xF17F) );    // IdentificationOptionVehicleManufacturerSpecific
                result.append( UdsDataIdRange(0xF1A0, 0xF1EF) );    // IdentificationOptionVehicleManufacturerSpecific
                result.append( UdsDataIdRange(0xF1F0, 0xF1FF) );    // IdentificationOptionSystemSupplierSpecific
                break;

            case IdCategory::PeriodicIds:
                result.append( UdsDataIdRange(0xF200, 0xF2FF) );
                break;

            case IdCategory::DynamicallyDefinedIds:
                result.append( UdsDataIdRange(0xF300, 0xF3FF) );
                break;

            case IdCategory::ObdSpecificIds:
                result.append( UdsDataIdRange(0xF400, 0xF4FF) );    // OBDDataIdentifier
                result.append( UdsDataIdRange(0xF600, 0xF6FF) );    // OBDMonitor
                result.append( UdsDataIdRange(0xF800, 0xF8FF) );    // OBDInfoType
                break;

            case IdCategory::TachographIds:
                result.append( UdsDataIdRange(0xF900, 0xF9FF) );
                break;

            case IdCategory::AirbagDeploymentIds:
                result.append( UdsDataIdRange(0xFA00, 0xFA0F) );
                break;

            case IdCategory::SafetySystemIds:
                result.append( UdsDataIdRange(0xFA19, 0xFAFF) );
                break;

            case IdCategory::SystemSupplierSpecificIds:
                result.append( UdsDataIdRange(0xFD00, 0xFEFF) );
                break;
        }

        return result;
    }

    QString UdsDataId::descriptionForId(quint16 id)
    {
        if ( id <= 0x00FF )                 { return "ISOSAEReserved"; }
        if ( id >= 0x0100 && id <= 0xA5FF ) { return "VehicleManufacturerSpecific"; }
        if ( id >= 0xA600 && id <= 0xA7FF ) { return "ReservedForLegislativeUse"; }
        if ( id >= 0xA800 && id <= 0xACFF ) { return "VehicleManufacturerSpecific"; }
        if ( id >= 0xAD00 && id <= 0xAFFF ) { return "ReservedForLegislativeUse"; }
        if ( id >= 0xB000 && id <= 0xB1FF ) { return "VehicleManufacturerSpecific"; }
        if ( id >= 0xB200 && id <= 0xBFFF ) { return "ReservedForLegislativeUse"; }
        if ( id >= 0xC000 && id <= 0xC2FF ) { return "VehicleManufacturerSpecific"; }
        if ( id >= 0xC300 && id <= 0xCEFF ) { return "ReservedForLegislativeUse"; }
        if ( id >= 0xCF00 && id <= 0xEFFF ) { return "VehicleManufacturerSpecific"; }
        if ( id >= 0xF000 && id <= 0xF00F ) { return "NetworkConfigurationDataForTractorTrailerApplicationData"; }
        if ( id >= 0xF010 && id <= 0xF0FF ) { return "VehicleManufacturerSpecific"; }
        if ( id >= 0xF100 && id <= 0xF17F ) { return "IdentificationOptionVehicleManufacturerSpecific"; }

        if ( id >= 0xF1A0 && id <= 0xF1EF ) { return "IdentificationOptionVehicleManufacturerSpecific"; }
        if ( id >= 0xF1F0 && id <= 0xF1FF ) { return "IdentificationOptionSystemSupplierSpecific"; }
        if ( id >= 0xF200 && id <= 0xF2FF ) { return "PeriodicDataIdentifier"; }
        if ( id >= 0xF300 && id <= 0xF3FF ) { return "DynamicallyDefinedDataIdentifier"; }
        if ( id >= 0xF400 && id <= 0xF4FF ) { return "OBDDataIdentifier"; }
        if ( id >= 0xF500 && id <= 0xF5FF ) { return "OBDDataIdentifier Reserved"; }
        if ( id >= 0xF600 && id <= 0xF6FF ) { return "OBDMonitor"; }
        if ( id >= 0xF700 && id <= 0xF7FF ) { return "OBDMonitor Reserved"; }
        if ( id >= 0xF800 && id <= 0xF8FF ) { return "OBDInfoType"; }
        if ( id >= 0xF900 && id <= 0xF9FF ) { return "TachographDataIdentifier"; }
        if ( id >= 0xFA00 && id <= 0xFA0F ) { return "AirbagDeploymentDataIdentifier"; }

        if ( id >= 0xFA13 && id <= 0xFA18 ) { return "EDREntries"; }

        if ( id >= 0xFA19 && id <= 0xFAFF ) { return "SafetySystemDataIdentifier"; }
        if ( id >= 0xFB00 && id <= 0xFCFF ) { return "ReservedForLegislativeUse"; }
        if ( id >= 0xFD00 && id <= 0xFEFF ) { return "SystemSupplierSpecific"; }
        if ( id >= 0xFF01 )                 { return "ISOSAEReserved"; }

        switch (id)
        {
            case 0xF180: return "BootSoftwareIdentification";
            case 0xF181: return "ApplicationSoftwareIdentification";
            case 0xF182: return "ApplicationDataIdentification";
            case 0xF183: return "BootSoftwareFingerprint";
            case 0xF184: return "ApplicationSoftwareFingerprint";
            case 0xF185: return "ApplicationDataFingerprint";
            case 0xF186: return "ActiveDiagnosticSession";
            case 0xF187: return "VehicleManufacturerSparePartNumber";
            case 0xF188: return "VehicleManufacturerECUSoftwareNumber";
            case 0xF189: return "VehicleManufacturerECUSoftwareVersionNumber";
            case 0xF18A: return "SystemSupplierIdentifierDataIdentifier";
            case 0xF18B: return "ECUManufacturingDate";
            case 0xF18C: return "ECUSerialNumber";
            case 0xF18D: return "SupportedFunctionalUnits";
            case 0xF18E: return "VehicleManufacturerKitAssemblyPartNumber";
            case 0xF18F: return "ISOSAEReservedStandardized";
            case 0xF190: return "VIN";
            case 0xF191: return "VehicleManufacturerECUHardwareNumber";
            case 0xF192: return "SystemSupplierECUHardwareNumber";
            case 0xF193: return "SystemSupplierECUHardwareVersionNumber";
            case 0xF194: return "SystemSupplierECUSoftwareNumber";
            case 0xF195: return "SystemSupplierECUSoftwareVersionNumber";
            case 0xF196: return "ExhaustRegulationOrTypeApprovalNumber";
            case 0xF197: return "SystemNameOrEngineType";
            case 0xF198: return "RepairShopCodeOrTesterSerialNumber";
            case 0xF199: return "ProgrammingDate";
            case 0xF19A: return "CalibrationRepairShopCodeOrCalibrationEquipmentSerialNumber";
            case 0xF19B: return "CalibrationDate";
            case 0xF19C: return "CalibrationEquipmentSoftwareNumber";
            case 0xF19D: return "ECUInstallationDate";
            case 0xF19E: return "ODXFile";
            case 0xF19F: return "Entity";

            case 0xFA10: return "NumberOfEDRDevices";
            case 0xFA11: return "EDRIdentification";
            case 0xFA12: return "EDRDeviceAddressInformation";

            case 0xFF00: return "UDSVersion";
        }

        return "";
    }
}
