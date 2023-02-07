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

#include "caninterfaceconfigdialog.h"
#include "ui_caninterfaceconfigdialog.h"
#include "caninterface/caninterfacemanager.h"
#include "caninterface/canbridge.h"

#include <QMessageBox>

using namespace Lindwurm::Lib;

namespace Lindwurm::Core
{
    CanInterfaceConfigDialog::CanInterfaceConfigDialog(Lib::CanInterfaceManager* manager, QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::CanInterfaceConfigDialog)
        , m_interfaceManager(manager)
    {
        ui->setupUi(this);

        setupBitRates();

        connect(ui->interfaceTypeBox,   &QComboBox::currentTextChanged, this, &CanInterfaceConfigDialog::selectedInterfaceTypeChanged);
        connect(ui->canFDCheckBox,      &QCheckBox::toggled, ui->dataBitRateBox, &QComboBox::setEnabled);

        connect(ui->addButton,      &QPushButton::clicked, this, &CanInterfaceConfigDialog::addButtonClicked);
        connect(ui->applyButton,    &QPushButton::clicked, this, &CanInterfaceConfigDialog::applyButtonClicked);
        connect(ui->cancelButton,   &QPushButton::clicked, this, &CanInterfaceConfigDialog::close);
    }

    CanInterfaceConfigDialog::~CanInterfaceConfigDialog()
    {
        delete ui;
    }

    void CanInterfaceConfigDialog::addInterface()
    {
        ui->addButton->setVisible(true);
        ui->applyButton->setVisible(false);

        ui->interfaceTypeBox->clear();
        ui->interfaceNameEdit->setText("interface #" + QString::number(m_interfaceAddCounter));

        setupBridgeableDevices();

        ui->interfaceTypeBox->setEnabled(true);
        ui->deviceBox->setEnabled(true);

        ui->localEchoCheckBox->setChecked(false);

        ui->canFDCheckBox->setChecked(false);
        ui->dataBitRateBox->setEnabled(false);
        ui->dataBitRateBox->setCurrentText("2000000");

        ui->interfaceTypeBox->addItems( m_interfaceManager->interfaceTypes() );

        show();
    }

    void CanInterfaceConfigDialog::editInterface(QString interfaceId)
    {
        ui->applyButton->setVisible(true);
        ui->addButton->setVisible(false);

        CanInterfaceInfo interfaceInfo = m_interfaceManager->interfaceInfoFor(interfaceId);

        if ( ! interfaceInfo.isValid() )
        {
            return;
        }

        m_interfaceIdToEdit = interfaceId;

        ui->interfaceTypeBox->clear();
        ui->interfaceTypeBox->addItem( interfaceInfo.interfaceType() );
        ui->interfaceTypeBox->setEnabled(false);

        ui->interfaceNameEdit->setText( interfaceInfo.name() );

        // Configuring the device of this interface
        ui->deviceBox->clear();

        // Configuring bridge interface list
        if ( interfaceInfo.interfaceType() == "bridge" )
        {
            setupBridgedDevices( m_interfaceManager->interfaceById(interfaceId) );
            ui->bridgeInterfaceList->setEnabled(true);
        }
        else
        {
            ui->deviceBox->addItem( interfaceInfo.device() );
            ui->deviceBox->setEnabled(false);

            ui->bridgeInterfaceList->clear();
            ui->bridgeInterfaceList->setEnabled(false);
        }

        ui->bitRateBox->setCurrentText( QString::number( interfaceInfo.bitRate() ) );

        ui->localEchoCheckBox->setChecked( interfaceInfo.localEchoEnabled() );

        if ( interfaceInfo.flexibleDataRateEnabled() == true )
        {
            ui->canFDCheckBox->setChecked(true);
            ui->dataBitRateBox->setCurrentText( QString::number( interfaceInfo.dataBitRate() ) );
        }
        else
        {
            ui->canFDCheckBox->setChecked(false);
            ui->dataBitRateBox->setCurrentText("2000000");
        }

        show();
    }

    void CanInterfaceConfigDialog::setupBitRates()
    {
        ui->bitRateBox->clear();

        const QList<int> rates = { 10000, 20000, 50000, 100000, 125000, 250000, 500000, 800000, 1000000 };

        for (int rate : rates)
        {
            ui->bitRateBox->addItem(QString::number(rate), rate);
        }

        const QList<int> dataRates = { 2000000, 4000000, 8000000 };

        for (int rate : dataRates)
        {
            ui->dataBitRateBox->addItem(QString::number(rate), rate);
        }
    }

    void CanInterfaceConfigDialog::setupBridgeableDevices()
    {
        ui->bridgeInterfaceList->clear();

        QList<CanInterfaceInfo> availableInterfaces = m_interfaceManager->availableInterfaces();

        for (const auto &interfaceInfo : availableInterfaces)
        {
            if ( interfaceInfo.interfaceType() != "bridge" )
            {
                QListWidgetItem* interfaceItem = new QListWidgetItem(interfaceInfo.name(), ui->bridgeInterfaceList);
                interfaceItem->setFlags(interfaceItem->flags() | Qt::ItemIsUserCheckable);
                interfaceItem->setCheckState(Qt::Unchecked);
                interfaceItem->setData(Qt::UserRole, interfaceInfo.id() );
            }
        }
    }

    void CanInterfaceConfigDialog::setupBridgedDevices(const Lib::ICanInterfaceSharedPtr &interface)
    {
        QStringList bridgedInterfaceIDs;

        ui->bridgeInterfaceList->clear();

        std::shared_ptr<CanBridge> bridge = std::dynamic_pointer_cast<CanBridge>(interface);

        if ( bridge )
        {
            bridgedInterfaceIDs = bridge->bridgedInterfaceIds();
        }

        QList<CanInterfaceInfo> availableInterfaces = m_interfaceManager->availableInterfaces();

        for (const auto &interfaceInfo : availableInterfaces)
        {
            if ( interfaceInfo.interfaceType() != "bridge" )
            {
                QListWidgetItem* interfaceItem = new QListWidgetItem(interfaceInfo.name(), ui->bridgeInterfaceList);
                interfaceItem->setFlags(interfaceItem->flags() | Qt::ItemIsUserCheckable);

                if ( bridgedInterfaceIDs.contains( interfaceInfo.id() ) )
                {
                    interfaceItem->setCheckState(Qt::Checked);
                }
                else
                {
                    interfaceItem->setCheckState(Qt::Unchecked);
                }

                interfaceItem->setData(Qt::UserRole, interfaceInfo.id() );
            }
        }
    }

    void CanInterfaceConfigDialog::selectedInterfaceTypeChanged(const QString &interfaceType)
    {
        // set up the dialog in according to the selected interface type
        ui->deviceBox->clear();

        if ( interfaceType == "bridge" )
        {
            ui->deviceBox->setEnabled(false);
            ui->bitRateBox->setEnabled(false);
            ui->canFDCheckBox->setEnabled(false);
            ui->dataBitRateBox->setEnabled(false);
            ui->localEchoCheckBox->setEnabled(false);

            ui->canFDCheckBox->setChecked(false);
            ui->localEchoCheckBox->setChecked(false);
            ui->bitRateBox->clear();
            ui->dataBitRateBox->clear();

            ui->bridgeInterfaceList->setEnabled(true);

            if ( ui->interfaceNameEdit->text().startsWith("interface #") )
            {
                ui->interfaceNameEdit->setText("bridge #" + QString::number(m_interfaceAddCounter));
            }
        }
        else
        {
            setupBitRates();

            ui->deviceBox->setEnabled(true);
            ui->bitRateBox->setEnabled(true);
            ui->canFDCheckBox->setEnabled(true);

            if ( ui->canFDCheckBox->isChecked() )
            {
                ui->dataBitRateBox->setEnabled(true);
            }
            else
            {
                ui->dataBitRateBox->setEnabled(false);
            }
            ui->localEchoCheckBox->setEnabled(true);

            ui->bridgeInterfaceList->setEnabled(false);

            ui->deviceBox->addItems( m_interfaceManager->availableDevicesOf(interfaceType) );

            if ( ui->interfaceNameEdit->text().startsWith("bridge #") )
            {
                ui->interfaceNameEdit->setText("interface #" + QString::number(m_interfaceAddCounter));
            }
        }
    }

    void CanInterfaceConfigDialog::addButtonClicked()
    {
        if ( ui->interfaceNameEdit->text().isEmpty() )
        {
            QMessageBox::warning(this, "Interface name empty", "Please fill in an interface name.");
            return;
        }

        if ( ui->deviceBox->currentText().isEmpty() && ui->interfaceTypeBox->currentText() != "bridge" )
        {
            QMessageBox::warning(this, "No Device selected", "No device available for this interface type. Please use a different interface type.");
            return;
        }

        CanInterfaceConfig config = getSelectedConfig();

        ICanInterfaceManager::InterfaceError addError;

        addError = m_interfaceManager->addInterface( config );

        switch (addError)
        {
            case ICanInterfaceManager::InterfaceError::DuplicateInterfaceName:

                QMessageBox::warning(this, "Duplicate interface name", "The interface name '" + config.name + "' is \nalready used. Please try another name.");

                break;

            case ICanInterfaceManager::InterfaceError::NoError:

                m_interfaceAddCounter++;
                close();

                break;

            default:

                QMessageBox::critical(this, "Unknown error", "The interface could not be created.");
                break;
        }
    }

    void CanInterfaceConfigDialog::applyButtonClicked()
    {
        if ( ui->interfaceNameEdit->text().isEmpty() )
        {
            QMessageBox::warning(this, "Interface name empty", "Please fill in an interface name.");
            return;
        }

        CanInterfaceConfig config = getSelectedConfig();
        config.id = m_interfaceIdToEdit;

        CanInterfaceManager::InterfaceError editError;

        editError = m_interfaceManager->editInterface(config);

        switch (editError)
        {
            case CanInterfaceManager::InterfaceError::DuplicateInterfaceName:

                QMessageBox::warning(this, "Duplicate interface name", "The interface name '" + config.name + "' is already used. Please choose a different name.");

            break;

            case CanInterfaceManager::InterfaceError::InterfaceNotFound:

                QMessageBox::warning(this, "Interface not found", "The interface '" + config.name + "' was not found by the CAN interface manager. This is a bug.");

                break;

            case CanInterfaceManager::InterfaceError::NoError:

                close();

                break;

            default:

                QMessageBox::critical(this, "Unknown error", "The interface could not be modified.");
                break;
        }
    }

    Lib::CanInterfaceConfig CanInterfaceConfigDialog::getSelectedConfig() const
    {
        Lib::CanInterfaceConfig config;

        config.name                     = ui->interfaceNameEdit->text();
        config.interfaceType            = ui->interfaceTypeBox->currentText();
        config.device                   = ui->deviceBox->currentText();
        config.enableLocalEcho          = ui->localEchoCheckBox->isChecked();
        config.enableFlexibleDataRate   = ui->canFDCheckBox->isChecked();
        config.bitRate                  = ui->bitRateBox->currentData().toInt();
        config.dataBitRate              = ui->dataBitRateBox->currentData().toInt();

        int bridgeInterfaceCount = ui->bridgeInterfaceList->count();

        for (int i = 0; i < bridgeInterfaceCount; i++)
        {
            if ( ui->bridgeInterfaceList->item(i)->data(Qt::CheckStateRole) == Qt::CheckState::Checked)
            {
                config.bridgedInterfaces.append( ui->bridgeInterfaceList->item(i)->data(Qt::UserRole).toString() );
            }
        }

        return config;
    }
}
