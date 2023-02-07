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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "ioptionspage.h"

#include <QPushButton>
#include <QDialogButtonBox>

#include <QMessageBox>

namespace Lindwurm::Core
{
    std::vector<std::unique_ptr<IOptionsPage>>  SettingsDialog::s_optionsPages;

    SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::SettingsDialog)
    {
        ui->setupUi(this);

        for (const IOptionsPageUniquePtr& page : s_optionsPages)
        {
            ui->optionTabs->addTab( page->widget(), page->caption() );
        }

        connect( ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &SettingsDialog::apply);
    }

    SettingsDialog::~SettingsDialog()
    {
        delete ui;
    }

    void SettingsDialog::addOptionsPage(IOptionsPageUniquePtr page)
    {
        s_optionsPages.push_back( std::move(page) );
    }

    bool SettingsDialog::apply()
    {
        bool restartNeeded = false;

        for (const IOptionsPageUniquePtr& page : s_optionsPages)
        {
            if ( page->apply() )
            {
                restartNeeded = true;
            }
        }

        return restartNeeded;
    }

    void SettingsDialog::accept()
    {
       if ( apply() )
       {
           QMessageBox::information(this, "Restart required", "A restart of Lindwurm is required for the settings to be applied.");
       }

       QDialog::accept();
    }
}
