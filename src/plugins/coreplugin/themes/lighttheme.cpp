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

#include "lighttheme.h"
#include <QApplication>
#include <QStyle>

namespace Lindwurm::Core
{
    LightTheme::LightTheme()
    {
        setActiveTheme(this);
    }

    QString LightTheme::themeName() const
    {
        return "LightTheme";
    }

    QString LightTheme::themeStylesheet() const
    {
        return "";
    }

    QIcon LightTheme::themeIcon(const QString &iconName) const
    {
        if ( iconName == "tool-tracer/clear-filter-active" )
        {
            return QIcon(":/images/icons/tool-tracer/clear-filter.svg");
        }

        if ( iconName == "tool-tracer/apply-filter-active" )
        {
            return QIcon(":/images/icons/tool-tracer/apply-filter.svg");
        }

        return QIcon(":/images/icons/" + iconName + ".svg");
    }

    QPalette LightTheme::themePalette() const
    {
        // the light theme uses the standard fusion palette
        return QPalette();
    }
}
