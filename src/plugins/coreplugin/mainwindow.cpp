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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogs/aboutdialog.h"
#include "dialogs/settingsdialog.h"
#include "utils/tabtoolwidget.h"

#include "cantoolmanager.h"

#include "themes/activetheme.h"
#include "themes/darktheme.h"
#include "themes/lighttheme.h"

#include <QSettings>
#include <QApplication>

namespace Lindwurm::Core
{
    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
        , m_canToolsWidget( new TabToolWidget )
        , m_canToolManager(new CanToolManager(m_canToolsWidget, this) )
    {
        ui->setupUi(this);
        ui->statusbar->setVisible(false);

        QSettings settings;
        bool darkTheme = settings.value("core/theme.dark").toBool();

        if ( darkTheme )
        {
            m_theme = new DarkTheme();
        }
        else
        {
            m_theme = new LightTheme();
        }

        qApp->setPalette( ActiveTheme::palette() );
        qApp->setStyleSheet( ActiveTheme::stylesheet() );

        this->setCentralWidget(m_canToolsWidget);

        connect(ui->actionShowAbout,    &QAction::triggered, this, &MainWindow::showAboutDialog);
        connect(ui->actionShowSettings, &QAction::triggered, this, &MainWindow::showSettingsDialog);
        connect(ui->actionExit,         &QAction::triggered, this, &MainWindow::close);

        showMaximized();
    }

    MainWindow::~MainWindow()
    {
        delete ui;
        delete m_theme;
    }

    void MainWindow::showAboutDialog()
    {
        if ( m_aboutDialog == nullptr )
        {
            m_aboutDialog = new AboutDialog(this);
            connect(m_aboutDialog, &QDialog::finished, this, &MainWindow::destroyAboutDialog);
            m_aboutDialog->show();
        }
        else
        {
            m_aboutDialog->show();
        }
    }

    void MainWindow::destroyAboutDialog()
    {
        if ( m_aboutDialog )
        {
            m_aboutDialog->deleteLater();
            m_aboutDialog = nullptr;
        }
    }

    void MainWindow::showSettingsDialog()
    {
        if ( m_settingsDialog == nullptr )
        {
            m_settingsDialog = new SettingsDialog(this);
            connect(m_settingsDialog, &QDialog::finished, this, &MainWindow::destroySettingsDialog);
            m_settingsDialog->show();
        }
        else
        {
            m_settingsDialog->show();
        }
    }

    void MainWindow::destroySettingsDialog()
    {
        if ( m_settingsDialog )
        {
            m_settingsDialog->deleteLater();
            m_settingsDialog = nullptr;
        }
    }

    void MainWindow::closeEvent(QCloseEvent *event)
    {
        Q_UNUSED(event)
        m_canToolManager->shutdown();
    }
}
