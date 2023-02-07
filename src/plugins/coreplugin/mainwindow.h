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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui { class MainWindow; }

class QCloseEvent;

namespace Lindwurm::Core
{
    class AboutDialog;
    class SettingsDialog;
    class TabToolWidget;
    class CanToolManager;
    class ITheme;

    /**
     * @brief The MainWindow the main window of Lindwurm.
     *
     * The main window contains the tool bar on the left side and a central QSplitter widget.
     * Currently, all the content is managed by the CanToolManager so in general access to
     * the MainWindow is not needed. However, the instance is managed by the ICore class
     * and can be obtained via ICore::mainWindow().
     */
    class MainWindow : public QMainWindow
    {
        Q_OBJECT
        public:

            explicit    MainWindow(QWidget *parent = nullptr);
                        ~MainWindow();

        private slots:

            void                showAboutDialog();
            void                destroyAboutDialog();

            void                showSettingsDialog();
            void                destroySettingsDialog();

        protected:

            void                closeEvent(QCloseEvent *event) override;

        private:

            Ui::MainWindow*     ui;
            TabToolWidget*      m_canToolsWidget = { nullptr };
            CanToolManager*     m_canToolManager = { nullptr };
            AboutDialog*        m_aboutDialog = { nullptr };
            SettingsDialog*     m_settingsDialog = { nullptr };
            ITheme*             m_theme = { nullptr };
    };
}

#endif // MAINWINDOW_H
