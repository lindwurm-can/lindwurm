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

#include "activetheme.h"

#include "itheme.h"

namespace Lindwurm::Core
{
    ITheme* ActiveTheme::s_activeTheme = nullptr;

    QString ActiveTheme::name()
    {
        if ( s_activeTheme )
        {
            return s_activeTheme->themeName();
        }

        return "";
    }

    QString ActiveTheme::stylesheet()
    {
        if ( s_activeTheme )
        {
            return s_activeTheme->themeStylesheet();
        }

        return "";
    }

    QIcon ActiveTheme::icon(const QString &iconName)
    {
        if ( s_activeTheme )
        {
            return s_activeTheme->themeIcon(iconName);
        }

        return QIcon();
    }

    QPalette ActiveTheme::palette()
    {
        if ( s_activeTheme )
        {
            return s_activeTheme->themePalette();
        }

        return QPalette();
    }

    void ActiveTheme::setActiveTheme(ITheme *theme)
    {
        s_activeTheme = theme;
    }
}
