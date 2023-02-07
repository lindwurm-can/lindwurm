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

#include "udstoolsplugin.h"

#include "cantoolmanager.h"
#include "udstoolswidget.h"
#include "cantool.h"
#include "coreconstants.h"

namespace Lindwurm::Core
{
    UdsToolsPlugin::UdsToolsPlugin()
    {

    }

    UdsToolsPlugin::~UdsToolsPlugin()
    {

    }

    bool UdsToolsPlugin::initialize(const QStringList &arguments)
    {
        Q_UNUSED(arguments)

        CanToolManager::addCanTool( std::make_shared<CanTool>(Constants::TOOL_ID_UDS, Constants::TOOL_INDEX_UDS, QIcon(":/images/icons/tool-uds/uds.png"), new UdsToolsWidget, "UDS") );

        return true;
    }

    bool UdsToolsPlugin::shutdown()
    {
        return true;
    }

    QString UdsToolsPlugin::errorString()
    {
        return "";
    }

    QString UdsToolsPlugin::pluginName()
    {
        return "Lindwurm::UDSTools";
    }
}
