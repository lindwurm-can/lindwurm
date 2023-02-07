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

#ifndef CANTOOL_H
#define CANTOOL_H

#include "coreplugin_global.h"
#include <QObject>
#include <QString>
#include <QIcon>
#include <QWidget>

namespace Lindwurm::Core
{
    /**
     * @brief The CanTool class defines a specific CAN tool in Lindwurm.
     *
     * An instance of this class defines the representation of a specific CAN tool as a tool
     * button in the tool bar on the left side of Lindwurm's main window and a tool widget
     * in the central QSplitter widget.
     *
     * To add a new CAN tool to Lindwum create an instance of the CanTool class with the
     * specific attributes and register the tool via the CanToolManager. When the tool button
     * is clicked the slot toolButtonClicked() is called. The default implementation toggles the
     * tool's widget visibility.
     *
     * The index of the CanTool defines the order in which the tool is added to the tool bar.
     * Therefore all CanTool instances must be registered in a plugin's initialize() method
     * as they are sorted in the core plugin's pluginsInitialized() method. Adding CAN tools
     * during runtime is currently not supported.
     */
    class COREPLUGIN_EXPORT CanTool : public QObject
    {
        Q_OBJECT
        public:

            CanTool();
            CanTool(const QString& id, int index, const QIcon& icon, QWidget* widget, const QString& caption);

            /**
             * @brief Set the id of the tool.
             *
             * The tool id should follow the current nameing scheme 'namespace.tool.cantool-name'. But may
             * subject to change in the future.
             */
            void        setID(const QString& id);

            /**
             * @brief Get the id of the tool.
             * @return the id.
             */
            QString     id() const;

            /**
             * @brief Get the index of the CanTool
             * @return the index.
             */
            int         index() const;

            /**
             * @brief Set the caption of the tool.
             *
             * The caption is displayed under to tool icon in the tool bar. Changeing the
             * caption during runtime is currently not supported.
             */
            void        setCaption(const QString &caption);

            /**
             * @brief Get the caption of the tool.
             * @return the caption.
             */
            QString     caption() const;

            /**
             * @brief Set the tool widget.
             *
             * The tool widget will be added to the central QSplitter widget of Lindwurms main window.
             */
            void        setWidget(QWidget *widget);

            /**
             * @brief Get the tool widget.
             * @return the tool widget.
             */
            QWidget*    widget() const;

            /**
             * @brief Set the tool icon.
             *
             * The icon is shown for the tool in the tool bar.
             */
            void        setIcon(const QIcon& icon);

            /**
             * @brief Get the tool icon.
             * @return the tool icon
             */
            QIcon       icon() const;

        public slots:

            /**
             * @brief This slot is called when the tool button in the tool bar was clicked.
             * @param checked is `true` when the button is checked, or galse if the button is unchecked.
             */
            virtual void toolButtonClicked(bool checked);

        protected:

            QString     m_ID;
            int         m_Index;
            QString     m_Caption;
            QIcon       m_Icon;
            QWidget*    m_Widget;
    };
}

#endif // CANTOOL_H
