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

#include "isotpwidget.h"
#include "ui_isotpwidget.h"

#include "caninterface/icaninterfacemanager.h"
#include "caninterface/icaninterfacehandlesharedptr.h"
#include "caninterface/icaninterfacehandle.h"
#include "cantransport/isotransportprotocol.h"

#include <QDebug>
#include <QLoggingCategory>

namespace
{
    Q_LOGGING_CATEGORY(LOG_TAG, "lindwurm.isotptool")
    int             BASE_16             = 16;
    const char*     COMPONENT_NAME      = "ISO Transport Protocol";
}

namespace Lindwurm::Core
{
    IsoTpWidget::IsoTpWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::IsoTpWidget)
    {
        ui->setupUi(this);

        ui->selectInterface->setModel( Lib::ICanInterfaceManager::instance()->interfaceListModel() );
        ui->selectInterface->setMinimumWidth(150);

        connect(ui->bindButton, &QPushButton::clicked, this, &IsoTpWidget::bindButtonClicked);
        connect(ui->sendButton, &QPushButton::clicked, this, &IsoTpWidget::sendButtonClicked);

        connect(ui->selectInterface, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index)
        {
            if (  index == -1 )
            {
                // if no interfaces are currently available disable bind button
                ui->bindButton->setEnabled(false);
            }
            else
            {
                // and if at least one interface is available enable bind button
                ui->bindButton->setEnabled(true);
            }
        });

        // if no interfaces are available (yet) disable bind button
        if ( ui->selectInterface->count() == 0)
        {
            ui->bindButton->setEnabled(false);
        }

        ui->sendButton->setEnabled(false);
    }

    IsoTpWidget::~IsoTpWidget()
    {
        delete ui;
    }

    void IsoTpWidget::sendButtonClicked()
    {
        QByteArray dataToSend = QByteArray::fromHex( ui->dataEdit->text().toUtf8() );

        if (m_transportProtocol)
        {
            m_transportProtocol->sendData(dataToSend);
        }
    }

    void IsoTpWidget::bindButtonClicked()
    {
        if ( m_isBound )
        {
            unbind();
        }
        else
        {
            bind();
        }
    }

    void IsoTpWidget::setBindControlsEnabled(bool enabled)
    {
        ui->sendButton->setEnabled(! enabled );
        ui->selectInterface->setEnabled(enabled);
        ui->sourceAddressEdit->setEnabled(enabled);
        ui->targetAddressEdit->setEnabled(enabled);
        ui->usePadding->setEnabled(enabled);
    }

    void IsoTpWidget::bind()
    {
        bool convertOK;

        quint32 sourceAddress = ui->sourceAddressEdit->text().toUInt(&convertOK, BASE_16);

        if ( !convertOK )
        {
            qCritical(LOG_TAG) << "Invalid source address format.";
            return;
        }

        quint32 targetAddress = ui->targetAddressEdit->text().toUInt(&convertOK, BASE_16);

        if ( !convertOK )
        {
            qCritical(LOG_TAG) << "Invalid target address format.";
            return;
        }

        qDebug(LOG_TAG) << "Bind ISO-TP to source address" << Qt::hex << sourceAddress << "/ targetAddress" << Qt::hex << targetAddress << ".";

        m_transportProtocol = new Lib::IsoTransportProtocol(sourceAddress, targetAddress, this);
        m_transportProtocol->setPaddingEnabled( ui->usePadding->isChecked() );

        QString interfaceId = ui->selectInterface->currentData().toString();
        Lib::ICanInterfaceHandleSharedPtr canInterface = Lib::ICanInterfaceManager::instance()->mountInterface(interfaceId, COMPONENT_NAME);

        if ( ! canInterface )
        {
            qCritical(LOG_TAG) << "Could not mount interface";
            return;
        }

        if ( ! canInterface->connected() )
        {
            qCritical(LOG_TAG) << "Failed to start bind ISO-TP: Interface is not connected.";
            return;
        }

        m_transportProtocol->mountCANInterface(canInterface);

        connect(m_transportProtocol, &Lib::IsoTransportProtocol::dataSent,      this, &IsoTpWidget::dataSent);
        connect(m_transportProtocol, &Lib::IsoTransportProtocol::dataReceived,  this, &IsoTpWidget::dataReceived);

        ui->bindButton->setText("Unbind");
        m_isBound = true;
        setBindControlsEnabled(false);
    }

    void IsoTpWidget::unbind()
    {
        m_transportProtocol->unmountCANInterface();

        disconnect(m_transportProtocol, &Lib::IsoTransportProtocol::dataSent,      this, &IsoTpWidget::dataSent);
        disconnect(m_transportProtocol, &Lib::IsoTransportProtocol::dataReceived,  this, &IsoTpWidget::dataReceived);

        delete m_transportProtocol;
        m_transportProtocol = nullptr;

        ui->bindButton->setText("Bind");
        m_isBound = false;
        setBindControlsEnabled(true);
    }

    void IsoTpWidget::dataSent(const QByteArray &data)
    {
        QString hexdump = ui->sourceAddressEdit->text().toUpper() + ": " + toHexdump(data);

        QString addressIndentation;
        addressIndentation.fill(' ', ui->sourceAddressEdit->text().length() + 2);

        hexdump.replace("\n", "<br>" + addressIndentation);
        hexdump.replace(" ", "&nbsp;");

        ui->dataView->appendHtml("<span style='background-color: #fbeded; color: #891212'>" + hexdump + "</span>");
    }

    void IsoTpWidget::dataReceived(const QByteArray &data)
    {
        QString hexdump = ui->targetAddressEdit->text().toUpper() + ": " + toHexdump(data);

        QString addressIndentation;
        addressIndentation.fill(' ', ui->targetAddressEdit->text().length() + 2);

        hexdump.replace("\n", "<br>" + addressIndentation);
        hexdump.replace(" ", "&nbsp;");

        ui->dataView->appendHtml("<span style='background-color: #ededfb; color: #121289'>" + hexdump + "</span>");
    }

    QString IsoTpWidget::toHexdump(const QByteArray &data, int indentation) const
    {
        QString hexdump;
        int currentDataOffset = 0;
        int dataSize = data.size();

        while ( currentDataOffset < dataSize )
        {
            QString currentLine;
            QString currentAsciiLine;

            for (int i = 0; i < 2; i++)
            {
                int remainingBytes = dataSize - currentDataOffset;
                int chunkSize = 8;

                if (remainingBytes < 8)
                {
                    chunkSize = remainingBytes;
                }

                if ( remainingBytes > 0 )
                {
                    QByteArray dataChunk = data.mid(currentDataOffset, chunkSize);

                    currentLine += dataChunk.toHex(' ').toUpper();

                    if ( i == 0)
                    {
                        currentAsciiLine += "|" + toASCIIString(dataChunk);
                    }
                    else
                    {
                        currentAsciiLine += " " + toASCIIString(dataChunk);
                    }

                    currentDataOffset += chunkSize;
                }

                while ( chunkSize < 8 )
                {
                    if ( chunkSize > 0)
                    {
                        currentLine += "   ";
                    }
                    else
                    {
                        currentLine += "  ";
                    }
                    chunkSize++;
                }

                currentLine += "  ";
            }

            currentLine += " " + currentAsciiLine + "|";

            for (int i = 0; i < indentation; i++)
            {
                hexdump += " ";
            }

            // padd all lines to 70 chars + indentation
            int paddingLength = (70 + indentation) - currentLine.length();
            for (int i = 0; i < paddingLength; i++)
            {
                currentLine += " ";
            }

            hexdump += currentLine;

            // avoid new line at end of hexdump
            if ( currentDataOffset < dataSize )
            {
                hexdump += "\n";
            }
        }

        return hexdump;
    }

    QString IsoTpWidget::toASCIIString(const QByteArray &data) const
    {
        QString asciiString;
        int length = data.length();
        QChar dataChar;

        for (int i = 0; i < length; i++)
        {
            dataChar = QChar(data.at(i));
            if ( dataChar.isPrint() )
                asciiString.append( QString( dataChar ) );
            else
                asciiString.append( "." );
        }

        return asciiString;
    }
}
