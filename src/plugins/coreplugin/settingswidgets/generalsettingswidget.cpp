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

#include "generalsettingswidget.h"
#include "ui_generalsettingswidget.h"

#include <QSettings>

namespace Lindwurm::Core
{
    GeneralSettingsWidget::GeneralSettingsWidget() :
        ui(new Ui::GeneralSettingsWidget)
    {
        ui->setupUi(this);

        QSettings settings;

        ui->themeSelection->addItem("Light", false);
        ui->themeSelection->addItem("Dark", true);

        m_darkModeInitialSetting = settings.value("core/theme.dark").toBool();

        if ( m_darkModeInitialSetting )
        {
            ui->themeSelection->setCurrentIndex(1);
        }
        else
        {
            ui->themeSelection->setCurrentIndex(0);
        }

        ui->autoCreateSocketCAN->setChecked( settings.value("core/autoCreateSocketCAN", true).toBool() );
    }

    GeneralSettingsWidget::~GeneralSettingsWidget()
    {
        delete ui;
    }

    bool GeneralSettingsWidget::apply()
    {
        QSettings settings;

        bool darkModeSetting = ui->themeSelection->currentData().toBool();

        settings.setValue("core/theme.dark", darkModeSetting);
        settings.setValue("core/autoCreateSocketCAN", ui->autoCreateSocketCAN->isChecked() );

        // if dark mode setting is changed a restart is needed
        return (darkModeSetting != m_darkModeInitialSetting);
    }
}
