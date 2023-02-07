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

#ifndef CANTOOLMANAGER_H
#define CANTOOLMANAGER_H

#include "coreplugin_global.h"
#include <QObject>
#include <memory>
#include <QIcon>
#include <QString>
#include <QWidget>
#include <QList>

namespace Lindwurm::Core
{
    class CanTool;
    class TabToolWidget;
    using CanToolSharedPtr = std::shared_ptr<CanTool>;

    /**
     * @brief The CanToolManager class manages the different CAN tools and their representation within the tool bar.
     *
     * During the plugin loading phase the CanToolManager collects all provided CAN tools. After all plugins are
     * initialized the CanToolManager adds the tools to the tool bar according to their index.
     */
    class CanToolManager : public QObject
    {
        friend class MainWindow;
        friend class CorePlugin;

        Q_OBJECT
        public:

            /**
             * @brief Add a CAN tool to the managed tools.
             * @param tool the CAN tool to add.
             */
            static void             addCanTool(CanToolSharedPtr tool);

        private:

            /**
             * @brief CanToolManager is created and managed by the MainWindow
             * @param tabToolWidget     the MainWindow's tool widget.
             * @param parent            the QObject parent.
             */
            explicit                CanToolManager(TabToolWidget* tabToolWidget, QObject *parent = nullptr);
                                    ~CanToolManager();

            /**
             * @brief Add all tools to the tool bar.
             */
            static void             pluginsInitialized();

            // TODO: add docu
            static bool             isToolVisibleByDefault(const QString &toolId);

            /**
             * @brief Saves the current geometry of all CAN tool widgets.
             *
             * This method is called from MainWindow when the application is about to shutdown.
             */
            void                    shutdown();

            static CanToolManager*          s_instance;
            static QList<CanToolSharedPtr>  s_canTools;
            TabToolWidget*                  m_canToolsWidget;
    };
}

#endif // CANTOOLMANAGER_H
