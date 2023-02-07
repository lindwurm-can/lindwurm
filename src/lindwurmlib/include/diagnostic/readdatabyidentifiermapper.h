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

#ifndef READDATABYIDENTIFIERMAPPER_H
#define READDATABYIDENTIFIERMAPPER_H

#include "lindwurmlib_global.h"

#include <QObject>
#include <QList>
#include <QByteArray>

#include "cantransport/isotransportprotocol.h"
#include "utils/range.h"
#include "utils/rangeenumerator.h"

namespace Lindwurm::Lib
{
    using UdsDataIdRange            = Range<quint16>;
    using UdsDataIdRangeEnumerator  = RangeEnumerator<quint16>;

    class LINDWURMLIB_EXPORT ReadDataByIdentifierMapper : public QObject
    {
        Q_OBJECT
        public:

            explicit ReadDataByIdentifierMapper(IsoTransportProtocol* transport, const QList<UdsDataIdRange> &idRanges, QObject *parent = nullptr);

            void                    start();
            void                    stop();
            int                     totalCount() const;

        signals:

            void                    mappingStarted();
            void                    dataReceived(quint16 id, const QByteArray &data);
            void                    noDataReceived(quint16 id);
            void                    mappingFinished();
            void                    progress(int currentCount, int totalCount);

        private slots:

            void                    transportDataReceived(const QByteArray &data);
            void                    errorOccurred(Lindwurm::Lib::IsoTransportProtocol::IsoTpError error);

        private:

            void                    readNextId();

        private:

            IsoTransportProtocol*           m_transport;
            QList<UdsDataIdRangeEnumerator> m_idRanges;
            int                             m_currentRangeIndex = {0};
            int                             m_totalCount;
            int                             m_currentCount;
            quint16                         m_currentId = {0};
            bool                            m_stopRequested = {false};
    };
}

#endif // READDATABYIDENTIFIERMAPPER_H
