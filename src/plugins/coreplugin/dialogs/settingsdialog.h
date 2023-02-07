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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "coreplugin_global.h"

namespace Ui { class SettingsDialog; }

namespace Lindwurm::Core
{
    class IOptionsPage;
    using IOptionsPageUniquePtr      = std::unique_ptr<IOptionsPage>;

    /**
     * @brief The SettingsDialog class provides a dialog for changing the global Lindwurm settings.
     *
     * The global settings are organized in different pages (categories) that are displayed as individual tabs
     * within the dialog. The different pages are defined via the IOptionsPage interface are made available
     * with the static `addOptionsPage` method. All option pages should be added during the plugin's initialize
     * phase.
     */
    class COREPLUGIN_EXPORT SettingsDialog : public QDialog
    {
        Q_OBJECT
        public:

            explicit SettingsDialog(QWidget *parent = nullptr);
            ~SettingsDialog();

            /**
             * @brief Makes the defined option page available as a tab in the settings dialog.
             * @param page the options page to add.
             */
            static void addOptionsPage(IOptionsPageUniquePtr page);

        private slots:

            bool    apply();
            void    accept();

        private:

            Ui::SettingsDialog*                         ui;
            static std::vector<IOptionsPageUniquePtr>   s_optionsPages;
    };
}

#endif // SETTINGSDIALOG_H
