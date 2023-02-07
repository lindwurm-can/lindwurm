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

#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include "coreplugin_global.h"
#include <pluginsystem/iplugin.h>

namespace Lindwurm::Core
{
    class MainWindow;
    class ICore;

    /**
     * @brief The CorePlugin provides and initializes the core components of Lindwurm.
     *
     * For example it instantiates the ICore interface (which in turn creates the MainWindow) and prepares
     * the basic settings. In addition it registers the basic CAN tools like the interface manager and the logger.
     */
    class CorePlugin : public PluginSystem::IPlugin
    {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID IPLUGIN_IID)
        public:

                            CorePlugin();
                            virtual ~CorePlugin();

            virtual bool    initialize(const QStringList &arguments) override;
            virtual void    pluginsInitialized() override;
            virtual bool    shutdown() override;
            virtual QString errorString() override;
            virtual QString pluginName() override;

        private:

            ICore*          m_core = { nullptr };
    };
}

#endif // COREPLUGIN_H
