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

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "icore.h"

namespace Lindwurm::Core
{
    AboutDialog::AboutDialog(QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::AboutDialog)
    {
        ui->setupUi(this);
        setStyleSheet("QWidget { background:  rgba(64, 66, 68, 255); color: white; }");

        ui->versionLabel->setTextFormat(Qt::RichText);
        ui->versionLabel->setText("<html><head/><body><p><br/>Version " + ICore::version() + "<br/></p></body></html>");
    }

    AboutDialog::~AboutDialog()
    {
        delete ui;
    }
}
