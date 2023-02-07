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

#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QObject>
#include <QStringList>

#include "pluginsystem_global.h"

#define IPLUGIN_IID "org.lindwurm-can.pluginsystem.iplugin"

namespace Lindwurm::PluginSystem
{
    class PLUGINSYSTEM_EXPORT IPlugin : public QObject
    {
        Q_OBJECT
        public:

            virtual ~IPlugin() {}

            /**
             * @brief Called after the plugin was loaded and the IPlugin instance has been created.
             *
             * Plugins should initialize their internal state in this function and returns
             * if the initilization was successfull.
             *
             * @param arguments to control the plugin
             * @return success of initialisation
             */
            virtual bool initialize(const QStringList &arguments) = 0;


            // TODO: Add docu
            virtual void pluginsInitialized() {}

            /**
             * @brief Called when the plugin will be unloaded.
             *
             * Implement this method to clean up internal data of the plugin before it
             * will be unloaded. If the shutdown was finished emit the shutdownFinished signal.
             * This could also be used for a delayed shutdown.
             *
             * @return true on success of shutdown or false for a delayed shutdown
             */
            virtual bool shutdown();

            /**
             * @brief   Returns a user-readable message of the last error.
             * @return  user-readable error string
             */
            virtual QString errorString() = 0;

            /**
             * @brief   Returns the name of the plugin.
             * @return  the name of the plugin
             */
            virtual QString pluginName() = 0;

        signals:

            void shutdownFinished();
    };
}

#endif // IPLUGIN_H
