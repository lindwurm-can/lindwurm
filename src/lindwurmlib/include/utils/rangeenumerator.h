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

#ifndef RANGEENUMERATOR_H
#define RANGEENUMERATOR_H

#include "lindwurmlib_global.h"
#include <type_traits>
#include "range.h"

namespace Lindwurm::Lib
{
    template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class LINDWURMLIB_EXPORT RangeEnumerator : public Range<T>
    {
        public:

            RangeEnumerator(T begin, T end) : Range<T>(begin, end)
            {
                m_current = begin;
                m_isAtEnd = false;
            }

            RangeEnumerator(const Range<T> &other) : Range<T>(other.begin(), other.end())
            {
                m_current = other.begin();
                m_isAtEnd = false;
            }

            void reset()
            {
                m_current = this->m_begin;
                m_isAtEnd = false;
            }

            bool hasNext() const
            {
                return ( (this->m_isValid) && (! this->m_isAtEnd) && (this->m_current <= this->m_end) );
            }

            T next()
            {
                if (this->m_current == this->m_end)
                {
                    // avoid overflow errors if m_end is the maximum of type T
                    this->m_isAtEnd = true;
                }

                return this->m_current++;
            }

        protected:

            T       m_current;
            bool    m_isAtEnd;
    };
}

#endif // RANGEENUMERATOR_H
