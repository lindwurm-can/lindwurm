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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "pluginsystem_global.h"

#include <QObject>
#include <QStringList>
#include <QMap>

namespace Lindwurm::PluginSystem
{
    class IPlugin;

    class PLUGINSYSTEM_EXPORT PluginManager : public QObject
    {
        Q_OBJECT
        public:

            /**
             * @brief Get the unique PluginManager instance
             * @return the unique PluginManager instance
             */
            static  PluginManager *instance();

            PluginManager();
            virtual ~PluginManager();

        public slots:

            bool    loadPlugins(const QStringList &arguments);
            void    shutdown();

        private:

            QMap<QString, IPlugin*>     _plugins;
    };
}

#endif // PLUGINMANAGER_H
