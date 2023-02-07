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

#ifndef CANINTERFACECONFIG_H
#define CANINTERFACECONFIG_H

#include "lindwurmlib_global.h"

#include <QList>
#include <QStringList>

namespace Lindwurm::Lib
{
    /**
     * @brief The CanInterfaceConfig class is a container for the interface configuration while adding or editing them.
     */
    class LINDWURMLIB_EXPORT CanInterfaceConfig
    {
        public:
            QString         id = {""};
            QString         name = {""};
            QString         interfaceType = {""};
            QString         device = {""};
            bool            enableFlexibleDataRate = {false};
            int             bitRate = {0};
            int             dataBitRate = {0};
            bool            enableLocalEcho = {true};
            QStringList     bridgedInterfaces = {};
    };
}

#endif // CANINTERFACECONFIG_H
