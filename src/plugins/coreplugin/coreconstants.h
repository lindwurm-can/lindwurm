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

#ifndef CORECONSTANTS_H
#define CORECONSTANTS_H

/**
 * @brief Defines some global core constants for Lindwurm.
 */
namespace Lindwurm::Core::Constants
{
    const int TOOL_INDEX_LOGGER     = 0;
    const int TOOL_INDEX_INTERFACES = 1;
    const int TOOL_INDEX_TRACER     = 5;
    const int TOOL_INDEX_COMPOSER   = 2;
    const int TOOL_INDEX_UDS        = 3;
    const int TOOL_INDEX_ISOTP      = 4;

    const char TOOL_ID_LOGGER[]     = "core.tool.logger";
    const char TOOL_ID_INTERFACES[] = "core.tool.interfacemanager";
    const char TOOL_ID_TRACER[]     = "core.tool.tracer";
    const char TOOL_ID_COMPOSER[]   = "core.tool.composer";
    const char TOOL_ID_UDS[]        = "core.tool.uds";
    const char TOOL_ID_ISOTP[]        = "core.tool.isotp";
}

#endif // CORECONSTANTS_H
