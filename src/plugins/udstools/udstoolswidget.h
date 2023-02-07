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

#ifndef UDSTOOLSWIDGET_H
#define UDSTOOLSWIDGET_H

#include <QWidget>

namespace Ui { class UdsToolsWidget; }

namespace Lindwurm::Core
{
    /**
     * @brief The UdsToolsWidget class represents a container widget for various UDS tools.
     *
     * The different UDS tools are displayed as tabs within the UdsToolsWidget.
     */
    class UdsToolsWidget : public QWidget
    {
        Q_OBJECT
        public:

            explicit UdsToolsWidget(QWidget *parent = nullptr);
            ~UdsToolsWidget();

        private:

            Ui::UdsToolsWidget *ui;
    };
}

#endif // UDSTOOLSWIDGET_H
