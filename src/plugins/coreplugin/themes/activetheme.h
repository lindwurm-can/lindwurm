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

#ifndef ACTIVETHEME_H
#define ACTIVETHEME_H

#include "coreplugin_global.h"

#include <QString>
#include <QIcon>
#include <QPalette>

namespace Lindwurm::Core
{
    class ITheme;

    /**
     * @brief The ActiveTheme class is an accessor proxy for the current active theme.
     *
     * The ActiveTheme class encapsulates the current active ITheme implementation and provides a static
     * access to the current theme. If you need theme specific data (e.g. palette or in most cases the
     * theme specific icon) ActiveTheme simplifies access to the current active theme via its static methods.
     *
     * To create a custom theme you must implement the ITheme interface and subclass ActiveTheme to be able
     * to activate the theme. You activate the theme by calling the setActiveTheme() method within the constructor
     * of your ITheme class. The MainWindow activates the theme by instantiating only the selected ITheme
     * implementation. Therefore you must not instantiate a theme by your own. Currently it's not supported
     * to change a theme on the fly.
     */
    class COREPLUGIN_EXPORT ActiveTheme
    {
        public:

            /**
             * @brief Provides the name of the theme.
             * @return the name of the theme.
             */
            static QString  name();

            /**
             * @brief Provides the custom style sheet for the theme.
             * @return the style sheet for the theme.
             */
            static QString  stylesheet();

            /**
             * @brief Provides theme specific icons.
             * @param iconName the caption of the icon (without a file extentsion).
             * @return the specified icon.
             */
            static QIcon    icon(const QString &iconName);

            /**
             * @brief Provides the custom palette for the theme.
             * @return the palette for the theme.
             */
            static QPalette palette();

        protected:

            /**
             * @brief Sets the currently active theme.
             * @param theme the ITheme implementation to set as active.
             */
            static void     setActiveTheme(ITheme *theme);

        private:

            /**
             * @brief s_activeTheme holds the current active theme implementation.
             */
            static ITheme* s_activeTheme;
    };
}

#endif // ACTIVETHEME_H
