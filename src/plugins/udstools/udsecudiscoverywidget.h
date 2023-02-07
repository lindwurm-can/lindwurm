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

#ifndef UDSECUDISCOVERYWIDGET_H
#define UDSECUDISCOVERYWIDGET_H

#include <QWidget>

namespace Ui { class UdsEcuDiscoveryWidget; }

namespace Lindwurm::Lib
{
    class UdsEcuDiscoveryScanner;
}

namespace Lindwurm::Core
{
    class UdsEcuDiscoveryWidget : public QWidget
    {
        Q_OBJECT
        public:

            explicit UdsEcuDiscoveryWidget(QWidget *parent = nullptr);
            ~UdsEcuDiscoveryWidget();

            void    startDiscovery();

        private slots:

            void    ecuDiscovered(quint32 sourceAddress, quint32 targetAddress);
            void    progress(int precent);
            void    discoveryFinished();

        private:

            Ui::UdsEcuDiscoveryWidget*      ui;
            Lib::UdsEcuDiscoveryScanner*    m_ecuDiscoveryScanner;
    };
}

#endif // UDSECUDISCOVERYWIDGET_H
