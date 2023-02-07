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

#ifndef RANGE_H
#define RANGE_H

#include "lindwurmlib_global.h"
#include <type_traits>

namespace Lindwurm::Lib
{
    template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class LINDWURMLIB_EXPORT Range
    {
        public:

            Range(T begin, T end)
            {
                if (begin <= end)
                {
                    m_begin     = begin;
                    m_end       = end;
                    m_isValid   = true;
                }
                else
                {
                    m_isValid = false;
                }
            }

            T begin() const
            {
                return m_begin;
            }

            T end() const
            {
                return m_end;
            }

            T size() const
            {
                if ( ! m_isValid )
                {
                    return 0;
                }

                return (m_end - m_begin) + 1;
            }

        protected:

            T       m_begin   = {0};
            T       m_end     = {0};
            bool    m_isValid = {false};
    };
}

#endif // RANGE_H
