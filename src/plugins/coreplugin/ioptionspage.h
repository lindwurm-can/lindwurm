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

#ifndef IOPTIONSPAGE_H
#define IOPTIONSPAGE_H

#include "coreplugin_global.h"

#include <QWidget>
#include <QPointer>

namespace Lindwurm::Core
{
    /**
     * @brief The IOptionsPageWidget class defines the interface for the settings widget of an options page.
     */
    class COREPLUGIN_EXPORT IOptionsPageWidget : public QWidget
    {
        Q_OBJECT
        public:

            IOptionsPageWidget() { }
            virtual ~IOptionsPageWidget() { }

            /**
             * @brief This method is called when the Apply button on the settings dialog was clicked.
             *
             * Subclasses should check if any changes were made in the widget and persist the changed settings.
             *
             * @return `true` if the changed settings need a restart of Lindwurm, `false` otherwise.
             */
            virtual bool apply() = 0;
    };

    /**
     * @brief The IOptionsPage class is an interface for providing option tabs in the settings dialog.
     *
     * To provide an option page subclass IOptionsPage set a caption for the page with `setCaption` and
     * override the `widget` method or provide a WidgetCreator to create a widget for the page on demand.
     */
    class COREPLUGIN_EXPORT IOptionsPage
    {
        public:

                                IOptionsPage();
            virtual             ~IOptionsPage();

            /**
             * @brief Returns the caption of the options tab.
             * @return the caption of the options tab.
             */
            QString             caption() const { return m_caption; }

            /**
             * @brief A WidgetCreator is a callback function that creates settings widget on demand.
             */
            using WidgetCreator = std::function<IOptionsPageWidget *()>;

            /**
             * @brief Set a WidgetCreator callback to create settings widgets on demand.
             * @param widgetCreator the WidgetCreator to be set.
             */
            void                setWidgetCreator(const WidgetCreator &widgetCreator);

            /**
             * @brief Returns the widget to show in the options tab.
             *
             * Override this method to create a widget lazily here. Alternatively, use setWidgetCreator()
             * to set a callback function that is used to create a widget on demand. The widget will
             * be deleted as a child of the settings dialog' QTabWidget.
             *
             * @return the widget for the options tab.
             */
            virtual QWidget*    widget();

            /**
             * @brief This method is called when the Apply button on the settings dialog was clicked.
             *
             * The default implementation passes the `apply()` call over to the options widget.
             *
             * @return `true` if the changed settings need a restart of Lindwurm, `false` otherwise.
             */
            virtual bool        apply();


        protected:

            /**
             * @brief Set the caption for the options page.
             * @param caption the caption for the options page.
             */
            void                setCaption(const QString& caption) { m_caption = caption; }

        protected:

            QString                         m_caption;
            WidgetCreator                   m_widgetCreator;
            QPointer<IOptionsPageWidget>    m_optionsWidget = { nullptr };
    };
}

#endif // IOPTIONSPAGE_H
