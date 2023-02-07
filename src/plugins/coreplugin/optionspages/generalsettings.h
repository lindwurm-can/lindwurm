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

#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <ioptionspage.h>

namespace Lindwurm::Core
{
    /**
     * @brief The GeneralSettings class implements the IOptionsPage for the general settings of Lindwurm.
     */
    class GeneralSettings : public IOptionsPage
    {
        public:

            GeneralSettings();
    };
}

#endif // GENERALSETTINGS_H
