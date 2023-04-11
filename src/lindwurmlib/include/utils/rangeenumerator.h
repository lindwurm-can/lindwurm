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

            RangeEnumerator() : m_range( Range<T>(0, 0) )
            {
                m_current   = 0;
                m_step      = 0;
                m_hasNext   = false;
                m_isValid   = false;
            }

            RangeEnumerator(T begin, T end) : m_range( Range<T>(begin, end) )
            {
                m_current   = begin;
                m_hasNext   = true;
                m_isValid   = true;

                if (begin <= end)
                {
                    m_step = 1;
                }
                else
                {
                    m_step = -1;
                }
            }

            RangeEnumerator(const Range<T> &range) : m_range( Range<T>(range.begin(), range.end()) )
            {
                m_current   = range.begin();
                m_hasNext   = true;
                m_isValid   = true;

                if ( range.begin() <= range.end() )
                {
                    m_step = 1;
                }
                else
                {
                    m_step = -1;
                }
            }

            int size() const
            {
                if ( ! m_isValid )
                {
                    return 0;
                }

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
                if ( ! m_isValid )
                {
                    return;
                }

                m_current = m_range.begin();
                m_hasNext = true;
            }

            bool hasNext() const
            {
                return m_hasNext;
            }

            T next()
            {
                if ( ! m_hasNext )
                {
                    return 0;
                }

                if ( m_current == m_range.end() )
                {
                    // avoid overflow errors if range end is the maximum/minmum of type T
                    m_hasNext = false;
                }

                T current = m_current;

                m_current = m_current + m_step;

                return current;
            }

        private:

            Range<T>    m_range;
            T           m_current;
            T           m_step;
            bool        m_isValid;
            bool        m_hasNext;
    };
}

#endif // RANGEENUMERATOR_H
