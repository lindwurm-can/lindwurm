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

#ifndef BYTEARRAYENUMERATOR_H
#define BYTEARRAYENUMERATOR_H

#include "lindwurmlib_global.h"
#include <QByteArray>
#include <QVector>

namespace Lindwurm::Lib
{
    /**
     * @brief The ByteArrayEnumerator class allows enumerating over ranges within a byte array.
     *
     * This class allows a simple enumeration over one or multiple ranges within a byte array.
     * It parses a hex string with defined byte ranges (e.g. `00 00-1F 11 00-01`) converts
     * it to a QByteArray and allows to enumerate over all permutations of byte combinations.
     * So in the above example the first QByteArray would be `00 00 11 00`, the second `00 00 11 01`
     * and the third would be `00 01 11 00`. The enumeration then will stopp at `00 1F 11 01`.
     * This allows for an easy fuzzing of payloads.
     */
    class LINDWURMLIB_EXPORT ByteArrayEnumerator
    {
        public:

            ByteArrayEnumerator();

            /**
             * @brief Parses a hex string and initializes the ByteArrayEnumerator.
             *
             * Each byte in the hex string must be seperated by a space. A detailed
             * error handling is currently not implemented.
             *
             * @param byteArrayEnumeratorSource the hex string to be enumerated.
             * @return `true` if the hex string was parsed successfully and the enumerator is valid; otherwise `false`.
             */
            bool            parse(QString byteArrayEnumeratorSource);

            /**
             * @brief Returns true if there is at least one item remaining.
             * @return `true` if there is at least one item remaining; otherwise `false`
             */
            bool            hasNext() const;

            /**
             * @brief Returns the next available QByteArray in the sequence.
             * @return the next available QByteArray in the sequence; if no item is left an empty array.
             */
            QByteArray      next();

            /**
             * @brief Resets the enumerator back to the first item.
             */
            void            reset();

            /**
             * @brief Returns the total item count in the sequence.
             * @return total item count in the sequence.
             */
            quint64         itemCount() const;

            /**
             * @brief Returns the current item counter.
             * @return the current item counter.
             */
            quint64         currentCount() const;


        private:

            /**
             * @brief The EnumerationPosition struct stores the details of each
             * position in the byte array that represents a range.
             */
            struct EnumerationPosition
            {
                int         index;      /*! The index of the enumeration range in the target array. */
                int         start;      /*! The start of the enumeration range. */
                int         end;        /*! The end of the enumeration range. */
                int         value;      /*! The current value of the byte in the array. */
            };

            void                            computeItemCount();

            QByteArray                      m_byteArray = {};       /*! The target array with the current item of the sequence. */
            quint64                         m_itemCount = { 0 };
            quint64                         m_currentCount = { 0 };
            QVector<EnumerationPosition>    m_enumerations = {};    /*! Stores the details for each enumeration range. */
    };
}

#endif // BYTEARRAYENUMERATOR_H
