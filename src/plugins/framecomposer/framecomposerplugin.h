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

#ifndef FRAMECOMPOSERPLUGIN_H
#define FRAMECOMPOSERPLUGIN_H

#include "framecomposer_global.h"
#include <pluginsystem/iplugin.h>

namespace Lindwurm::Core
{
    class FRAMECOMPOSER_EXPORT FrameComposerPlugin : public PluginSystem::IPlugin
    {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID IPLUGIN_IID)
        public:

            FrameComposerPlugin();
            virtual ~FrameComposerPlugin();

            virtual bool    initialize(const QStringList &arguments) override;
            virtual bool    shutdown() override;
            virtual QString errorString() override;
            virtual QString pluginName() override;
    };
}

#endif // FRAMECOMPOSERPLUGIN_H
