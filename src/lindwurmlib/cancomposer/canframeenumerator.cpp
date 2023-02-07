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

#include "cancomposer/canframeenumerator.h"

#include <QDebug>
#include <QLoggingCategory>
#include <QRegularExpression>

namespace
{
    const int BASE_16 = 16;
    Q_LOGGING_CATEGORY(LOG_TAG, "lindwurm.lib.composer")
}

namespace Lindwurm::Lib
{
    CanFrameEnumerator::CanFrameEnumerator()
    {

    }

    quint64 CanFrameEnumerator::frameCount() const
    {
        if ( m_hasStaticPayload )
        {
            return m_endFrameID - m_startFrameID;
        }

        // number of frame IDs to enumerate * number of payloads for each ID
        return  (m_endFrameID - m_startFrameID) * m_dynamicPayload.itemCount();
    }

    quint64 CanFrameEnumerator::currentCount() const
    {
        if ( m_hasStaticPayload )
        {
            return m_currentFrameID - m_startFrameID;
        }

        //     ( number of alread fully enumerated ID * number of payloads each ID ) + payload count for current ID
        return ( (m_currentFrameID - m_startFrameID) * m_dynamicPayload.itemCount() ) + m_dynamicPayload.currentCount();
    }

    bool CanFrameEnumerator::hasNext() const
    {
        // even if we have a dynamic payload we don't need to consider m_dynamicPayload.hasNext()
        // because end condition is always the end frame ID
        return (m_currentFrameID < m_endFrameID) && (m_isValid == true);
    }

    QCanBusFrame CanFrameEnumerator::next()
    {
        if ( (m_currentFrameID >= m_endFrameID) || (m_isValid == false) )
        {
            return QCanBusFrame();
        }

        QCanBusFrame frame;

        if ( m_hasStaticPayload )
        {
            frame.setFrameId(m_currentFrameID);
            frame.setPayload(m_staticPayload);

            m_currentFrameID++;
        }
        else
        {
            Q_ASSERT_X( m_dynamicPayload.hasNext() , "CanFrameEnumerator::next", "Unexpected condition: dynamic payload has no next enumeration!");

            frame.setFrameId( m_currentFrameID );
            frame.setPayload( m_dynamicPayload.next() );

            if ( ! m_dynamicPayload.hasNext() )
            {
                // if we have no more enumerations in this round
                // we skip to the next frame ID and reset the payload enumerator to start over
                m_currentFrameID++;
                m_dynamicPayload.reset();
            }
        }

        return frame;
    }

    bool CanFrameEnumerator::parseFrame(QString textFrame)
    {
        static QRegularExpression newLine("[\r\n]");

        if ( textFrame.contains( newLine ) )
        {
            qCritical(LOG_TAG) << "Parsing error: A frame cannot span multiple lines!";
            return false;
        }

        QStringList frameElements = textFrame.split(QRegExp("\\s+"), Qt::SkipEmptyParts);

        if ( frameElements.size() < 2 )
        {
            qCritical(LOG_TAG) << "Parsing error: A frame needs at least an ID and one data byte!";
            return false;
        }

        if ( ! parseFrameID( frameElements.at(0) ) )
        {
            qCritical(LOG_TAG) << "Failed to parse frame ID";
            return false;
        }

        frameElements.pop_front();

        QString payloadString = frameElements.join(" ");

        // check if any of the payload elements contains an interval
        if ( payloadString.contains('-') )
        {
            m_hasStaticPayload = false;

            if ( ! m_dynamicPayload.parse(payloadString) )
            {
                qCritical(LOG_TAG) << "Failed to parse frame payload at: " << payloadString;
                return false;
            }
        }
        else
        {
            m_hasStaticPayload = true;

            if ( ! parseStaticPayload(frameElements) )
            {
                qCritical(LOG_TAG) << "Failed to parse frame payload bytes";
                return false;
            }
        }

        m_isValid = true;

        return true;
    }

    void CanFrameEnumerator::reset()
    {
        m_currentFrameID = m_startFrameID;
        m_dynamicPayload.reset();
    }

    bool CanFrameEnumerator::parseFrameID(const QString &idElement)
    {
        bool toIntOk;

        if ( idElement.contains('-') )
        {
            // parse frame interval startID-endID (e.g. FF0-FFF)

            QStringList frameIDInterval = idElement.split('-', Qt::SkipEmptyParts);

            if ( frameIDInterval.size() != 2 )
            {
                qCritical(LOG_TAG) << "Failed to parse interval (unexpected number of interval parts) for frame ID at: " << idElement;
                return false;
            }

            int frameStartID = frameIDInterval.at(0).toInt(&toIntOk, BASE_16);

            if ( ! toIntOk )
            {
                qCritical(LOG_TAG) << "Failed to convert frame start ID to integer at: " << frameIDInterval.at(0);
                return false;
            }

            int frameEndID = frameIDInterval.at(1).toInt(&toIntOk, BASE_16);

            if ( ! toIntOk )
            {
                qCritical(LOG_TAG) << "Failed to convert frame end ID to integer at: " << frameIDInterval.at(1);
                return false;
            }

            // TODO: check ID boundaries and start < end
            m_startFrameID = frameStartID;
            m_currentFrameID = m_startFrameID;

            m_endFrameID = frameEndID + 1;
        }
        else
        {
            // parse simple frame ID (e.g. FFF)

            int frameID = idElement.toInt(&toIntOk, BASE_16);

            if ( ! toIntOk )
            {
                qCritical(LOG_TAG) << "Failed to convert frame ID to integer at: " << idElement;
                return false;
            }

            // TODO: check ID boundaries and start < end
            m_startFrameID = frameID;
            m_currentFrameID = m_startFrameID;

            // TODO: Consider and check type boundaries
            m_endFrameID = m_startFrameID + 1;
        }

        return true;
    }

    bool CanFrameEnumerator::parseStaticPayload(const QStringList &payloadElements)
    {
        bool toIntOk;

        m_staticPayload.clear();

        // parse static payload (e.g. AA BB CC DD EE) by converting each string element to byte

        for (const QString &payloadElement : qAsConst(payloadElements) )
        {
            int payloadByte = payloadElement.toInt(&toIntOk, BASE_16);

            if ( !toIntOk )
            {
                qCritical(LOG_TAG) << "Failed to convert payload data to byte at: " << payloadElement;
                return false;
            }

            m_staticPayload.append(payloadByte);
        }

        return true;
    }
}
