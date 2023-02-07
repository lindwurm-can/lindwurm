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

#include "cancomposer/canframecomposit.h"

#include <QRegularExpression>
#include <QDebug>
#include <QLoggingCategory>

namespace
{
    Q_LOGGING_CATEGORY(LOG_TAG, "lindwurm.lib.composer")
}


namespace Lindwurm::Lib
{
    CanFrameComposit::CanFrameComposit()
    {

    }

    bool CanFrameComposit::parseFrameComposit(QString textFrames)
    {
        m_isValid = false;
        m_frameCount = 0;
        m_currentCount = 0;
        m_currentFrameIndex = 0;

        m_frames.clear();

        // remove any comments
        static QRegularExpression comments("#.*");
        textFrames.replace(comments, "");

        // remove lines with only whitespace
        static QRegularExpression whitespace("\\s*[\r\n]");
        textFrames.replace(whitespace, "\n");

        QStringList frameLines = textFrames.split( QRegExp("[\r\n]"), Qt::SkipEmptyParts );

        // if memory that may have been previously allocated is substantially bigger than now required
        if ( m_frames.capacity() > (frameLines.size() + 20) )
        {
            // we release the memory first
            m_frames.squeeze();
        }

        // allocate new needed memory in advance or increase previously allocated memory to avoid reallocation (with a small buffer)
        m_frames.reserve( frameLines.size() + 5 );

        for (const QString& frameLine : qAsConst(frameLines))
        {
            CanFrameEnumerator frameEnumerator;

            if ( ! frameEnumerator.parseFrame(frameLine) )
            {
                qCritical(LOG_TAG) << "Failed to parse frame: " << frameLine;

                m_frames.clear();
                m_frames.squeeze();

                return false;
            }

            m_frameCount = m_frameCount + frameEnumerator.frameCount();

            m_frames.append( std::move(frameEnumerator) );
        }

        m_isValid = true;

        return true;
    }

    quint64 CanFrameComposit::frameCount() const
    {
        return m_frameCount;
    }

    quint64 CanFrameComposit::currentCount() const
    {
        return m_currentCount;
    }

    bool CanFrameComposit::hasNext() const
    {
        if ( m_currentFrameIndex >= m_frames.size() )
        {
            return false;
        }

        // we use a temporary index here because we only want to look ahead instead of
        // modifying the current object state here
        int tempIndex = m_currentFrameIndex;

        while ( ! m_frames[tempIndex].hasNext() )
        {
            tempIndex++;

            if ( tempIndex >= m_frames.size() )
            {
                return false;
            }
        }

        return true;
    }

    QCanBusFrame CanFrameComposit::next()
    {
        if ( m_currentFrameIndex >= m_frames.size() )
        {
            return QCanBusFrame();
        }

        while ( ! m_frames[m_currentFrameIndex].hasNext() )
        {
            m_currentFrameIndex++;

            if ( m_currentFrameIndex >= m_frames.size() )
            {
                return QCanBusFrame();
            }
        }

        m_currentCount++;
        return m_frames[m_currentFrameIndex].next();
    }

    void CanFrameComposit::reset()
    {
        m_currentCount = 0;
        m_currentFrameIndex = 0;

        int numberOfEnumerators = m_frames.size();

        for(int i = 0; i < numberOfEnumerators; i++)
        {
            m_frames[i].reset();
        }
    }
}
