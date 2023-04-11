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

#include "utils/bytearrayenumerator.h"
#include <QMutexLocker>
#include <QStringList>

namespace
{
    const int BASE_16 = 16;
}

namespace Lindwurm::Lib
{
    ByteArrayEnumerator::ByteArrayEnumerator()
    {

    }

    bool ByteArrayEnumerator::parse(QString byteArrayEnumeratorSource)
    {
        int positionIndex = 0;

        m_byteArray.clear();
        m_currentCount = 0;
        m_enumerations.clear();
        m_itemCount = 0;

        bool toShortOk;

        QStringList enumeratorElements = byteArrayEnumeratorSource.split(QRegExp("\\s+"), Qt::SkipEmptyParts);

        for (const QString &enumeratorElement : qAsConst(enumeratorElements) )
        {
            if ( enumeratorElement.contains('-') )
            {
                QStringList enumeratorIntervals = enumeratorElement.split('-', Qt::SkipEmptyParts);

                if ( enumeratorIntervals.size() != 2 )
                {
                    return false;
                }

                ByteArrayEnumerator::EnumerationPosition position;

                position.index = positionIndex;
                quint8 start = enumeratorIntervals[0].toShort(&toShortOk, BASE_16);
                if ( !toShortOk )
                {
                    return false;
                }

                quint8 end = enumeratorIntervals[1].toShort(&toShortOk, BASE_16);
                if ( !toShortOk )
                {
                    return false;
                }

                position.enumerator = RangeEnumerator<quint8>(start, end);

                m_enumerations.push_front(position);

                m_byteArray.append( (start & 0xFF) );
            }
            else
            {
                int enumeratorValue = enumeratorElement.toShort(&toShortOk, BASE_16);

                if ( !toShortOk )
                {
                    return false;
                }

                if ( (enumeratorValue < 0) || (enumeratorValue > 255) )
                {
                    return false;
                }

                m_byteArray.append( (enumeratorValue & 0xFF) );
            }

            positionIndex++;
        }

        reset();

        return true;
    }

    bool ByteArrayEnumerator::hasNext() const
    {
        if ( m_itemCount == 0 )
        {
            return false;
        }

        return m_currentCount < m_itemCount;
    }

    QByteArray ByteArrayEnumerator::next()
    {
        if ( m_currentCount >= m_itemCount )
        {
            return QByteArray();
        }

        m_currentCount++;

        if (m_currentCount == 1)
        {
            // this is the first byte array of this enumeration and all positions are already initialized by reset()
            // so we just return the byte array as initialized

            // we must handle this case separately because if we have multiple enumeration positions, each position
            // must be initialized in the first run (which is already done in reset)

            return m_byteArray;
        }

        for ( EnumerationPosition &position : m_enumerations )
        {
            if ( position.enumerator.hasNext() )
            {
                m_byteArray[ position.index ] = position.enumerator.next();

                // we have no overflow in this position,
                // so subsequent position stay at the current value and we skip them in this loop
                break;
            }
            else
            {
                position.enumerator.reset();
                m_byteArray[ position.index ] = position.enumerator.next();
            }
        }

        return m_byteArray;
    }

    void ByteArrayEnumerator::reset()
    {
        m_currentCount = 0;

        for ( EnumerationPosition &position : m_enumerations )
        {
            position.enumerator.reset();
            m_byteArray[ position.index ] = position.enumerator.next();
        }

        computeItemCount();
    }

    quint64 ByteArrayEnumerator::itemCount() const
    {
        return m_itemCount;
    }

    quint64 ByteArrayEnumerator::currentCount() const
    {
        return m_currentCount;
    }

    void ByteArrayEnumerator::computeItemCount()
    {
        m_itemCount = 1;

        for ( EnumerationPosition position : qAsConst(m_enumerations) )
        {
            m_itemCount = m_itemCount * ( position.enumerator.size() );
        }
    }
}
