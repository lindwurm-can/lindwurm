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

#include "udsecudiscoverywidget.h"
#include "ui_udsecudiscoverywidget.h"

#include "caninterface/icaninterfacemanager.h"
#include "caninterface/icaninterfacehandle.h"
#include "diagnostic/udsecudiscoveryscanner.h"

#include <QPropertyAnimation>
#include <QVariantMap>

#include <QDebug>
#include <QLoggingCategory>

using namespace Lindwurm::Lib;

namespace
{
    Q_LOGGING_CATEGORY(LOG_TAG, "lindwurm.udstools")
    const char*     COMPONENT_NAME      = "UDS ECU Discovery Scanner";
    int             BASE_16             = 16;
}

namespace Lindwurm::Core
{
    UdsEcuDiscoveryWidget::UdsEcuDiscoveryWidget(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::UdsEcuDiscoveryWidget)
        , m_ecuDiscoveryScanner( new Lib::UdsEcuDiscoveryScanner(this) )
    {
        ui->setupUi(this);

        ui->selectInterface->setModel( Lib::ICanInterfaceManager::instance()->interfaceListModel() );
        ui->selectInterface->setMinimumWidth(150);

        ui->progressBar->setMaximum(100);
        ui->progressBar->setValue(0);

        ui->discoveredDevicesTable->setHorizontalHeaderLabels( QStringList() << "Source address" << "Target address" );
        ui->discoveredDevicesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        
        ui->scanType->addItems( m_ecuDiscoveryScanner->scanTypes().keys() );

        connect(ui->selectInterface, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index)
        {
            if (  index == -1 )
            {
                // if no interfaces are currently available disable start button
                ui->startDiscoveryButton->setEnabled(false);
            }
            else
            {
                // and if at least one interface is available enable start button
                ui->startDiscoveryButton->setEnabled(true);
            }
        });

        // if no interfaces are available (yet) disable start button
        if ( ui->selectInterface->count() == 0)
        {
            ui->startDiscoveryButton->setEnabled(false);
        }

        connect(m_ecuDiscoveryScanner,      &Lib::UdsEcuDiscoveryScanner::ecuDiscovered,     this, &UdsEcuDiscoveryWidget::ecuDiscovered);
        connect(m_ecuDiscoveryScanner,      &Lib::UdsEcuDiscoveryScanner::progress,          this, &UdsEcuDiscoveryWidget::progress);
        connect(m_ecuDiscoveryScanner,      &Lib::UdsEcuDiscoveryScanner::discoveryFinished, this, &UdsEcuDiscoveryWidget::discoveryFinished);
        connect(ui->startDiscoveryButton,   &QPushButton::clicked,                           this, &UdsEcuDiscoveryWidget::startDiscovery);
    }

    UdsEcuDiscoveryWidget::~UdsEcuDiscoveryWidget()
    {
        delete ui;
    }

    void UdsEcuDiscoveryWidget::startDiscovery()
    {
        QString interfaceId = ui->selectInterface->currentData().toString();
        ICanInterfaceHandleSharedPtr canInterface = ICanInterfaceManager::instance()->mountInterface(interfaceId, COMPONENT_NAME);

        if ( ! canInterface )
        {
            qCritical(LOG_TAG) << "Could not mount interface";
            return;
        }

        bool convertOK;

        quint32 startAddress = ui->startAddress->text().toUInt(&convertOK, BASE_16);

        if ( !convertOK )
        {
            qCritical(LOG_TAG) << "Invalid start address format.";
            return;
        }

        quint32 endAddress = ui->endAddress->text().toUInt(&convertOK, BASE_16);

        if ( !convertOK )
        {
            qCritical(LOG_TAG) << "Invalid end address format.";
            return;
        }

        if ( ! canInterface->connected() )
        {
            qCritical(LOG_TAG) << "Failed to start ECU discovery: Interface is not connected.";
            return;
        }

        qInfo(LOG_TAG) << "UDS ECU discovery scan started from start address" << Qt::hex << startAddress << " to end address" << Qt::hex << endAddress << ".";

        m_ecuDiscoveryScanner->mountCANInterface(canInterface);

        ui->discoveredDevicesTable->setRowCount(0);
        ui->discoveredDevicesCSV->setPlainText("Source address;Target address");

        ui->progressBar->setValue(0);

        QPropertyAnimation *a = new QPropertyAnimation(ui->progressBar,"maximumHeight");

        a->setDuration(250);
        a->setStartValue( 0 );
        a->setEndValue( ui->progressBar->sizeHint().height() );
        a->start(QPropertyAnimation::DeleteWhenStopped);

        // TODO: Use selected scan type from UI when multiple scan types are implemented
        m_ecuDiscoveryScanner->startDiscovery(startAddress, endAddress, UdsEcuDiscoveryScanner::ScanType::DefaultDiagnosticSession );

        ui->startDiscoveryButton->setEnabled(false);
    }

    void UdsEcuDiscoveryWidget::ecuDiscovered(quint32 sourceAddress, quint32 targetAddress)
    {
        qInfo(LOG_TAG) << "Found UDS ECU with source address " << Qt::hex << sourceAddress << " and target address " << Qt::hex << targetAddress;

        ui->discoveredDevicesTable->insertRow( ui->discoveredDevicesTable->rowCount() );
        int newRowIndex = ui->discoveredDevicesTable->rowCount() - 1;

        QTableWidgetItem *newItem;

        newItem = new QTableWidgetItem( QString::number(sourceAddress, BASE_16).toUpper() );
        newItem->setTextAlignment(Qt::AlignCenter);
        ui->discoveredDevicesTable->setItem( newRowIndex, 0, newItem);

        newItem = new QTableWidgetItem( QString::number(targetAddress, BASE_16).toUpper() );
        newItem->setTextAlignment(Qt::AlignCenter);
        ui->discoveredDevicesTable->setItem( newRowIndex, 1, newItem );

        ui->discoveredDevicesCSV->appendPlainText(QString::number(sourceAddress, BASE_16).toUpper() + ";" + QString::number(targetAddress, BASE_16).toUpper());
    }

    void UdsEcuDiscoveryWidget::progress(int precent)
    {
        ui->progressBar->setValue(precent);
    }

    void UdsEcuDiscoveryWidget::discoveryFinished()
    {
        m_ecuDiscoveryScanner->unmountCANInterface();
        ui->startDiscoveryButton->setEnabled(true);

        QPropertyAnimation *a = new QPropertyAnimation(ui->progressBar,"maximumHeight");

        a->setDuration(250);
        a->setStartValue( ui->progressBar->height() );
        a->setEndValue( 0 );
        a->start(QPropertyAnimation::DeleteWhenStopped);

        qInfo(LOG_TAG) << "UDS ECU discovery scan finished";
    }
}
