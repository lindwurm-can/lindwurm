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

#include "framecomposerplugin.h"

#include "cantoolmanager.h"
#include "canframecomposerwidget.h"
#include "cantool.h"
#include "coreconstants.h"

namespace Lindwurm::Core
{
    FrameComposerPlugin::FrameComposerPlugin()
    {

    }

    Lindwurm::Core::FrameComposerPlugin::~FrameComposerPlugin()
    {

    }

    bool FrameComposerPlugin::initialize(const QStringList &arguments)
    {
        Q_UNUSED(arguments)

        CanToolManager::addCanTool( std::make_shared<CanTool>(Constants::TOOL_ID_COMPOSER, Constants::TOOL_INDEX_COMPOSER, QIcon(":/images/icons/tool-composer/composer.png"), new CanFrameComposerWidget, "Composer") );

        return true;
    }

    bool FrameComposerPlugin::shutdown()
    {
        return true;
    }

    QString FrameComposerPlugin::errorString()
    {
        return "";
    }

    QString FrameComposerPlugin::pluginName()
    {
        return "Lindwurm::FrameComposer";
    }
}
