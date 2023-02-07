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

#ifndef DARKTHEME_H
#define DARKTHEME_H

#include "activetheme.h"
#include "itheme.h"

namespace Lindwurm::Core
{
    /**
     * @brief The DarkTheme class provides a dark theme for Lindwurm.
     */
    class DarkTheme : public ITheme, private ActiveTheme
    {
        public:

            DarkTheme();

            virtual QString     themeName() const override;
            virtual QString     themeStylesheet() const override;
            virtual QIcon       themeIcon(const QString &iconName) const override;
            virtual QPalette    themePalette() const override;
    };
}

#endif // DARKTHEME_H
