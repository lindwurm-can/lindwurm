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

#ifndef FANCYTABSTYLE_H
#define FANCYTABSTYLE_H

#include <QProxyStyle>

namespace Lindwurm::Core
{
    /**
     * @brief The FancyTabStyle class is a QStyle adaption for tweaking the add tab button of the FancyTabWidget.
     */
    class FancyTabStyle : public QProxyStyle
    {
        Q_OBJECT
        public:

            explicit FancyTabStyle(QObject *parent = nullptr);

            virtual QRect subElementRect(SubElement subElement, const QStyleOption *option, const QWidget *widget) const override;
    };
}

#endif // FANCYTABSTYLE_H
