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

#ifndef ISOTPWIDGET_H
#define ISOTPWIDGET_H

#include <QWidget>

namespace Ui { class IsoTpWidget; }

namespace Lindwurm::Lib
{
    class IsoTransportProtocol;
}


namespace Lindwurm::Core
{
    class IsoTpWidget : public QWidget
    {
        Q_OBJECT
        public:

            explicit IsoTpWidget(QWidget *parent = nullptr);
            ~IsoTpWidget();

        private slots:

            void    sendButtonClicked();
            void    bindButtonClicked();

            void    setBindControlsEnabled(bool enabled);

            void    bind();
            void    unbind();

            void    dataSent(const QByteArray &data);
            void    dataReceived(const QByteArray &data);

        private:

            QString toHexdump(const QByteArray &data, int indentation = 0) const;
            QString toASCIIString(const QByteArray &data) const;

        private:

            Ui::IsoTpWidget*            ui;
            bool                        m_isBound = { false };
            Lib::IsoTransportProtocol*  m_transportProtocol = { nullptr };
    };
}

#endif // ISOTPWIDGET_H
