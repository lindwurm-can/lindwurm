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

#ifndef ITHEME_H
#define ITHEME_H

#include "coreplugin_global.h"
#include <QString>
#include <QIcon>
#include <QPalette>

namespace Lindwurm::Core
{
    /**
     * @brief The ITheme class defines the abstract interface for GUI themes.
     *
     * The ITheme class is a simple interface to define custom GUI themes for Lindwurm. Every
     * theme must implement this interface and also subclass ActiveTheme to be able to
     * activate the theme (see ActiveTheme documentation).
     */
    class COREPLUGIN_EXPORT ITheme
    {
        public:

            virtual             ~ITheme() {}

            /**
             * @brief Provides the name of the theme.
             * @return the name of the theme.
             */
            virtual QString     themeName() const = 0;

            /**
             * @brief Provides the custom style sheet for the theme.
             * @return the style sheet for the theme.
             */
            virtual QString     themeStylesheet() const = 0;

            /**
             * @brief Provides theme specific icons.
             * @param iconName the caption of the icon (without a file extentsion).
             * @return the specified icon.
             */
            virtual QIcon       themeIcon(const QString &iconName) const = 0;

            /**
             * @brief Provides the custom palette for the theme.
             * @return the palette for the theme.
             */
            virtual QPalette    themePalette() const = 0;
    };
}

#endif // ITHEME_H
