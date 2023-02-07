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

#ifndef TABTOOLWIDGET_H
#define TABTOOLWIDGET_H

#include "coreplugin_global.h"

#include <QWidget>
#include <QIcon>
#include <QByteArray>

class QSplitter;
class QVBoxLayout;
class QToolButton;

namespace Lindwurm::Core
{
    /**
     * @brief The TabToolWidget class provides a stack of splitted widgets controlled by a tool bar.
     *
     * The TabToolWidget provides a side bar which represents the available tools by a QToolButton and
     * a central QSplitter widget which displays the corresponding tool widgets. By inserting a tool
     * the TabToolWidget returns a QToolButton instance, which can be used to control the tool's widget
     * visibility.
     */
    class COREPLUGIN_EXPORT TabToolWidget : public QWidget
    {
        Q_OBJECT
        public:

            /**
             * @brief Constructs a TabToolWidget with the provided parent.
             * @param parent the QObject parent.
             */
            explicit TabToolWidget(QWidget *parent = nullptr);

            /**
             * @brief Inserts a tool with the given icon, tool label and tool widget.
             *
             * @param icon          the icon which represents the tool.
             * @param label         a caption that describes the tool.
             * @param toolWidget    the tool's widget.
             * @return the instance of the created button within the side bar.
             */
            QToolButton*    insertTool(const QIcon &icon, const QString &label, QWidget *toolWidget);

            /**
             * @brief Saves the geometry of the tools layout within the splitter widget.
             * @return the state of the layout.
             */
            QByteArray      saveToolsGeometry() const;

            /**
             * @brief Restores the tools layout to the specified geometry.
             * @param geometry  the geometry to restore.
             * @return `true` if the geometry is restored, otherwise `false`
             */
            bool            restoreToolsGeometry(const QByteArray &geometry);

        protected:

            QWidget*        m_tabBar = { nullptr };
            QVBoxLayout*    m_tabBarLayout = { nullptr };
            QSplitter*      m_toolWidgetSplitter = { nullptr };
    };
}

#endif // TABTOOLWIDGET_H
