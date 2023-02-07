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

#ifndef READDATABYIDENTIFIERWIDGET_H
#define READDATABYIDENTIFIERWIDGET_H

#include <QWidget>
#include <QElapsedTimer>
#include "cantransport/isotransportprotocol.h"

namespace Ui { class ReadDataByIdentifierWidget; }

namespace Lindwurm::Lib
{
    class ReadDataByIdentifierMapper;
}

namespace Lindwurm::Core
{
    class DataByIdTableModel;

    class ReadDataByIdentifierWidget : public QWidget
    {
        Q_OBJECT
        public:

            explicit ReadDataByIdentifierWidget(QWidget *parent = nullptr);
            ~ReadDataByIdentifierWidget();

        private slots:

            void    startScan();
            void    stopScan();
            void    dataReceived(quint16 id, const QByteArray &data);
            void    mappingFinished();
            void    exportToCSV();
            void    progress(int currentCount, int totalCount);

        private:

            Ui::ReadDataByIdentifierWidget*     ui;
            Lib::IsoTransportProtocol*          m_transport = { nullptr };
            Lib::ReadDataByIdentifierMapper*    m_mapper = { nullptr };
            DataByIdTableModel*                 m_dataIdModel = { nullptr };
            QElapsedTimer                       m_durationTimer;
            qint64                              m_lastElapsedTime;
    };
}

#endif // READDATABYIDENTIFIERWIDGET_H
