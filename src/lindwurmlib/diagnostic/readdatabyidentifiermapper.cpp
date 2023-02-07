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

#include "diagnostic/readdatabyidentifiermapper.h"

#include <QDebug>
#include <QLoggingCategory>

namespace
{
    Q_LOGGING_CATEGORY(LOG_TAG, "lindwurm.lib.diagnostic")
}

namespace Lindwurm::Lib
{
    ReadDataByIdentifierMapper::ReadDataByIdentifierMapper(IsoTransportProtocol *transport, const QList<Range<quint16> > &idRanges, QObject *parent)
        : QObject{parent}, m_transport(transport), m_totalCount(0)
    {
        connect(m_transport, &IsoTransportProtocol::dataReceived,   this, &ReadDataByIdentifierMapper::transportDataReceived);
        connect(m_transport, &IsoTransportProtocol::errorOccurred,  this, &ReadDataByIdentifierMapper::errorOccurred);

        for ( const UdsDataIdRange &range : qAsConst(idRanges) )
        {
            m_totalCount += range.size();
            m_idRanges.append( UdsDataIdRangeEnumerator(range) );
        }

        qDebug(LOG_TAG) << "Reading " << m_totalCount << " data IDs.";
    }

    void ReadDataByIdentifierMapper::start()
    {
        if ( m_totalCount == 0 )
        {
            return;
        }

        if ( m_currentRangeIndex > m_idRanges.size() )
        {
            return;
        }

        m_stopRequested = false;
        m_currentCount = 0;
        emit mappingStarted();

        readNextId();
    }

    void ReadDataByIdentifierMapper::stop()
    {
        m_stopRequested = true;
    }

    int ReadDataByIdentifierMapper::totalCount() const
    {
        return m_totalCount;
    }

    void ReadDataByIdentifierMapper::transportDataReceived(const QByteArray &data)
    {
        if ( data.at(0) == 0x62 )
        {
            QByteArray payload = data;

            quint16 receivedDataID = ((payload.at(1) & 0x00FF) << 8) | (payload.at(2) & 0x00FF);

            if ( receivedDataID == m_currentId)
            {
                payload.remove(0, 3);   // remove SID and data ID e.g. 62 F1 88
                emit dataReceived( m_currentId, payload);
            }
            else
            {
                qWarning(LOG_TAG) << "Received data for wrong data ID";
            }
        }
        else
        {
            emit noDataReceived( m_currentId );
        }

        m_currentCount++;
        emit progress(m_currentCount, m_totalCount);

        readNextId();
    }

    void ReadDataByIdentifierMapper::errorOccurred(IsoTransportProtocol::IsoTpError error)
    {
        Q_UNUSED(error)

        // TODO: error handling
        qCritical(LOG_TAG) << "ReadDataByIdentifierMapper::errorOccurred()";

        readNextId();
    }

    void ReadDataByIdentifierMapper::readNextId()
    {
        if (m_stopRequested)
        {
            emit mappingFinished();
            return;
        }

        while ( ! m_idRanges[m_currentRangeIndex].hasNext() )
        {
            m_currentRangeIndex++;
            if ( m_currentRangeIndex >= m_idRanges.size() )
            {
                emit mappingFinished();
                return;
            }
        }

        m_currentId = m_idRanges[m_currentRangeIndex].next();

        QByteArray request;
        request.append(0x22);

        request.append( (m_currentId & 0xFF00) >> 8 );
        request.append( (m_currentId & 0x00FF) );

        m_transport->sendData(request);
    }
}
