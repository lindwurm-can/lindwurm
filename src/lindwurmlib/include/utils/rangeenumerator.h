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
    class LINDWURMLIB_EXPORT RangeEnumerator
    {
        public:

            RangeEnumerator(T begin, T end) : m_range( Range<T>(begin, end) )
            {
                m_current   = begin;
                m_isAtEnd   = false;
            }

            RangeEnumerator(const Range<T> &range) : m_range( Range<T>(range.begin(), range.end()) )
            {
                m_current   = range.begin();
                m_isAtEnd   = false;
            }

            int size() const
            {
                return m_range.size();
            }

            T begin() const
            {
                return m_range.begin();
            }

            T end() const
            {
                return m_range.end();
            }

            void reset()
            {
                m_current = m_range.begin();
                m_isAtEnd = false;
            }

            bool hasNext() const
            {
                return (! this->m_isAtEnd) && (this->m_current <= m_range.end() );
            }

            T next()
            {
                if ( this->m_current == m_range.end() )
                {
                    // avoid overflow errors if m_end is the maximum of type T
                    this->m_isAtEnd = true;
                }

                return this->m_current++;
            }

        protected:

            Range<T>    m_range;
            T           m_current;
            bool        m_isAtEnd;
    };
}

#endif // RANGEENUMERATOR_H
