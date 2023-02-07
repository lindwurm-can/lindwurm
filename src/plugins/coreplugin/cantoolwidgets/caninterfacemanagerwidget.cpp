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

#include "caninterfacemanagerwidget.h"
#include "ui_caninterfacemanagerwidget.h"
#include "caninterface/caninterfacemanager.h"
#include "caninterface/caninterfacemanagermodel.h"
#include "dialogs/caninterfaceconfigdialog.h"

#include "themes/activetheme.h"

#include <QSettings>
#include <QMessageBox>

using namespace Lindwurm::Lib;

namespace Lindwurm::Core
{
    CanInterfaceManagerWidget::CanInterfaceManagerWidget(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::CanInterfaceManagerWidget)
        , m_interfaceManager( new Lib::CanInterfaceManager(this) )
        , m_interfaceManagerModel(  new Lib::CanInterfaceManagerModel(m_interfaceManager, this) )
        , m_interfaceConfigDialog( new CanInterfaceConfigDialog(m_interfaceManager, this) )
    {
        ui->setupUi(this);

        setupToolBar();

        setInterfaceModel(m_interfaceManagerModel);

        QSettings settings;
        if ( settings.value("core/autoCreateSocketCAN", true).toBool() )
        {
            addAvailableSocketCANInterfaces();
        }
    }

    CanInterfaceManagerWidget::~CanInterfaceManagerWidget()
    {
        delete ui;
    }

    void CanInterfaceManagerWidget::setupToolBar()
    {
        ui->toolBar->setStyleSheet("QToolBar { border-bottom: 1px solid gray; }");

        m_addInterfaceAction    = ui->toolBar->addAction( ActiveTheme::icon("tool-interfaces/add"),     "Add interface");
        m_removeInterfaceAction = ui->toolBar->addAction( ActiveTheme::icon("tool-interfaces/remove"),  "Remove interface");
        m_editInterfaceAction   = ui->toolBar->addAction( ActiveTheme::icon("tool-interfaces/edit"),    "Edit interface");

        connect(m_addInterfaceAction, &QAction::triggered, this, [this]
        {
            m_interfaceConfigDialog->addInterface();
        });

        connect(m_removeInterfaceAction,    &QAction::triggered, this, &CanInterfaceManagerWidget::removeSelectedInterface);
        connect(m_editInterfaceAction,      &QAction::triggered, this, &CanInterfaceManagerWidget::editSelectedInterface);
        connect(ui->interfacesView,         &QTreeView::doubleClicked, this, &CanInterfaceManagerWidget::editInteface);
    }

    void CanInterfaceManagerWidget::addAvailableSocketCANInterfaces()
    {
        QStringList socketCanDevices = m_interfaceManager->availableDevicesOf("socketcan");

        Lib::CanInterfaceConfig config;

        // TODO: get current config from device!?

        config.interfaceType            = "socketcan";
        config.enableLocalEcho          = true;
        config.bitRate                  = 500000;
        config.enableFlexibleDataRate   = false;

        for (const QString &device : socketCanDevices)
        {
            config.name     = device;
            config.device   = device;

            m_interfaceManager->addInterface(config, true);
        }
    }

    void CanInterfaceManagerWidget::setInterfaceModel(QAbstractItemModel *model)
    {
        ui->interfacesView->setModel(model);

        int columnCount = m_interfaceManagerModel->columnCount();

        for (int i = 0; i < columnCount; i++)
        {
            ui->interfacesView->resizeColumnToContents(i);
        }
    }

    void CanInterfaceManagerWidget::removeSelectedInterface()
    {
        if ( ! ui->interfacesView->currentIndex().isValid() )
        {
            return;
        }

        QString interfaceId = ui->interfacesView->currentIndex().data(CanInterfaceManagerModel::InterfaceIdRole).toString();

        ICanInterfaceManager::InterfaceError result = m_interfaceManager->removeInterface(interfaceId);


        if ( result == ICanInterfaceManager::InterfaceError::InterfaceMounted )
        {
            QString mountedComponents = m_interfaceManager->mountedComponents(interfaceId).join("\n- ");
            CanInterfaceInfo interfaceInfo = m_interfaceManager->interfaceInfoFor(interfaceId);

            QMessageBox::warning(this, "Interface busy", "The interface '" + interfaceInfo.name() +
                "' is currently mounted\n by the following components:\n\n- " + mountedComponents + "\n\nPlease unmount first!");

            return;
        }
    }

    void CanInterfaceManagerWidget::editInteface(const QModelIndex &index)
    {
        if ( ! index.isValid() )
        {
            return;
        }

        QString interfaceID = index.data(CanInterfaceManagerModel::InterfaceIdRole).toString();

        Q_ASSERT_X(interfaceID.isEmpty() == false, "CanInterfaceManagerWidget", "ID for interface to edit is empty");

        m_interfaceConfigDialog->editInterface(interfaceID);
    }

    void CanInterfaceManagerWidget::editSelectedInterface()
    {
        editInteface( ui->interfacesView->currentIndex() );
    }
}
