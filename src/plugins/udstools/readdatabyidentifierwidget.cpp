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

#include "readdatabyidentifierwidget.h"
#include "ui_readdatabyidentifierwidget.h"

#include "caninterface/icaninterfacemanager.h"
#include "caninterface/icaninterfacehandle.h"
#include "diagnostic/readdatabyidentifiermapper.h"
#include "diagnostic/udsdataid.h"
#include "utils/range.h"

#include "databyidtablemodel.h"

#include <QDebug>
#include <QLoggingCategory>
#include <QAction>
#include <QClipboard>
#include <QPropertyAnimation>
#include <QTime>

using namespace Lindwurm::Lib;

namespace
{
    Q_LOGGING_CATEGORY(LOG_TAG, "lindwurm.udstools")
    const char*     COMPONENT_NAME      = "UDS Read Data Scanner";
    int             BASE_16             = 16;
}

namespace Lindwurm::Core
{
    ReadDataByIdentifierWidget::ReadDataByIdentifierWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ReadDataByIdentifierWidget)
    {
        ui->setupUi(this);

        ui->selectInterface->setModel( Lib::ICanInterfaceManager::instance()->interfaceListModel() );
        ui->selectInterface->setMinimumWidth(150);

        connect(ui->selectInterface, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index)
        {
            if (  index == -1 )
            {
                // if no interfaces are currently available disable start button
                ui->startScanButton->setEnabled(false);
            }
            else
            {
                // and if at least one interface is available enable start button
                ui->startScanButton->setEnabled(true);
            }
        });

        // if no interfaces are available (yet) disable start button
        if ( ui->selectInterface->count() == 0)
        {
            ui->startScanButton->setEnabled(false);
        }

        ui->progressBar->setMaximum(100);
        ui->progressBar->setValue(0);

        m_dataIdModel = new DataByIdTableModel(this);
        ui->dataTable->setModel(m_dataIdModel);

        ui->selectedIdCategories->setShowMultipleItems(false);
        ui->selectedIdCategories->setPlaceholderText("No categories selected");
        ui->selectedIdCategories->setMultipleCheckedText("Multiple categories selected");

        QMap<QString, UdsDataId::IdCategory> categories = UdsDataId::idCategories();

        for (const QString& category : categories.keys() )
        {
            bool checked = categories[category] == UdsDataId::IdCategory::StandardIds ? true : false;
            ui->selectedIdCategories->addItem(category, checked, categories[category]);
        }

        connect(ui->selectedIdCategories, &CheckableComboBox::selectedIndexesChanged, this, [=](int numberOfSelectedIndexes)
        {
            if (numberOfSelectedIndexes > 0)
            {
                ui->startScanButton->setEnabled(true);
            }
            else
            {
                ui->startScanButton->setEnabled(false);
            }
        });

        int columnCount = m_dataIdModel->columnCount();

        for (int i = 0; i < columnCount; i++)
        {
            ui->dataTable->resizeColumnToContents(i);
        }

        QAction*    exportToCsvAction = new QAction("Copy selection to CSV", this);
        exportToCsvAction->setShortcut( QKeySequence(tr("Ctrl+C")) );
        connect(exportToCsvAction, &QAction::triggered, this, &ReadDataByIdentifierWidget::exportToCSV);

        ui->dataTable->addAction( exportToCsvAction );

        connect(ui->startScanButton,    &QPushButton::clicked, this, &ReadDataByIdentifierWidget::startScan);
        connect(ui->stopScanButton,     &QPushButton::clicked, this, &ReadDataByIdentifierWidget::stopScan);
    }

    ReadDataByIdentifierWidget::~ReadDataByIdentifierWidget()
    {
        delete ui;
    }

    void ReadDataByIdentifierWidget::startScan()
    {

        QString interfaceId = ui->selectInterface->currentData().toString();
        ICanInterfaceHandleSharedPtr canInterface = ICanInterfaceManager::instance()->mountInterface(interfaceId, COMPONENT_NAME);

        if ( ! canInterface )
        {
            qCritical(LOG_TAG) << "Could not mount interface";
            return;
        }

        bool convertOK;

        quint32 sourceAddress = ui->sourceAddress->text().toUInt(&convertOK, BASE_16);

        if ( !convertOK )
        {
            qCritical(LOG_TAG) << "Invalid source address format.";
            return;
        }

        quint32 targetAddress = ui->targetAddress->text().toUInt(&convertOK, BASE_16);

        if ( !convertOK )
        {
            qCritical(LOG_TAG) << "Invalid target address format.";
            return;
        }

        if ( ! canInterface->connected() )
        {
            qCritical(LOG_TAG) << "Failed to start Read Data By Identifier mapper: Interface is not connected.";
            return;
        }

        m_transport = new IsoTransportProtocol(sourceAddress, targetAddress, this);
        m_transport->mountCANInterface(canInterface);

        QVariantList selectedCategories = ui->selectedIdCategories->currentData().toList();
        QList<UdsDataIdRange> rangesToScan;

        for (const QVariant& category : selectedCategories)
        {
            rangesToScan.append( UdsDataId::rangesForCategory( static_cast<UdsDataId::IdCategory>( category.toInt() ) ) );
        }

        for (const UdsDataIdRange& range : rangesToScan)
        {
            qDebug() << "Scanning range from 0x" << Qt::hex << range.begin() << " to 0x" << Qt::hex << range.end();
        }

        m_mapper = new ReadDataByIdentifierMapper(m_transport, rangesToScan, this );

        m_dataIdModel->reset();

        connect(m_mapper, &ReadDataByIdentifierMapper::dataReceived,    this, &ReadDataByIdentifierWidget::dataReceived);
        connect(m_mapper, &ReadDataByIdentifierMapper::mappingFinished, this, &ReadDataByIdentifierWidget::mappingFinished);
        connect(m_mapper, &ReadDataByIdentifierMapper::progress,        this, &ReadDataByIdentifierWidget::progress);

        qDebug() << "Start scan";

        ui->startScanButton->setEnabled(false);
        ui->selectInterface->setEnabled(false);
        ui->selectedIdCategories->setEnabled(false);
        ui->sourceAddress->setEnabled(false);
        ui->targetAddress->setEnabled(false);

        ui->stopScanButton->setEnabled(true);

        ui->progressBar->setValue(0);
        ui->progressBar->setMaximum( m_mapper->totalCount() );
        ui->progressBar->setFormat( " %p % | Estimated time remaining:         ");

        m_lastElapsedTime = 0;
        m_durationTimer.start();

        QPropertyAnimation *a = new QPropertyAnimation(ui->progressBar,"maximumHeight");

        a->setDuration(250);
        a->setStartValue( 0 );
        a->setEndValue( ui->progressBar->sizeHint().height() );
        a->start(QPropertyAnimation::DeleteWhenStopped);

        m_mapper->start();
    }

    void ReadDataByIdentifierWidget::stopScan()
    {
        if (m_mapper)
        {
            m_mapper->stop();
        }
    }

    void ReadDataByIdentifierWidget::dataReceived(quint16 id, const QByteArray &data)
    {
        qInfo() << "Data received: " << data;
        m_dataIdModel->addData(id, data);
    }

    void ReadDataByIdentifierWidget::mappingFinished()
    {
        qDebug() << "ReadDataByIdentifierWidget::mappingFinished";
        m_mapper->deleteLater();
        m_mapper = nullptr;

        m_transport->unmountCANInterface();
        m_transport->deleteLater();
        m_transport = nullptr;

        QPropertyAnimation *a = new QPropertyAnimation(ui->progressBar,"maximumHeight");

        a->setDuration(250);
        a->setStartValue( ui->progressBar->height() );
        a->setEndValue( 0 );
        a->start(QPropertyAnimation::DeleteWhenStopped);

        ui->startScanButton->setEnabled(true);
        ui->selectInterface->setEnabled(true);
        ui->selectedIdCategories->setEnabled(true);
        ui->sourceAddress->setEnabled(true);
        ui->targetAddress->setEnabled(true);

        ui->stopScanButton->setEnabled(false);
    }

    void ReadDataByIdentifierWidget::exportToCSV()
    {
        QModelIndexList selection = ui->dataTable->selectionModel()->selectedRows();

        // selection model returns selection int the order in which the rows were selected by the user
        // to preserve the original order in the table, we sort the index by their row
        std::sort(selection.begin(), selection.end(), [](QModelIndex& a, QModelIndex& b) { return a.row() < b.row(); } );

        int numberOfColumns = m_dataIdModel->columnCount();

        QString csvData;

        for (int column = 0; column < numberOfColumns; column++)
        {
            if ( column != 0 ) csvData += ",";
            csvData += m_dataIdModel->headerData(column, Qt::Horizontal).toString();
        }
        csvData += "\n";

        for (const QModelIndex &index : selection)
        {
            int row = index.row();

            for (int column = 0; column < numberOfColumns; column++)
            {
                if ( column != 0 ) csvData += ",";

                // TODO: raw data with null bytes causes truncated CSV data
                csvData += m_dataIdModel->data( m_dataIdModel->index(row, column) ).toString();
            }
            csvData += "\n";
        }

        QGuiApplication::clipboard()->setText(csvData);
    }

    void ReadDataByIdentifierWidget::progress(int currentCount, int totalCount)
    {
        qint64 elapsedMillisecondsSinceStart = m_durationTimer.elapsed();

        if ( (elapsedMillisecondsSinceStart - m_lastElapsedTime) >= 1000 )
        {
            m_lastElapsedTime = elapsedMillisecondsSinceStart;

            qint64 remainingMiliseconds = ( (double) elapsedMillisecondsSinceStart / currentCount) * (totalCount - currentCount);
            if ( ((100.0f / totalCount) * currentCount) >= 10 )
            {
                ui->progressBar->setFormat( "%p % | Estimated time remaining: " + QTime::fromMSecsSinceStartOfDay(remainingMiliseconds).toString("hh:mm:ss"));
            }
            else
            {
                ui->progressBar->setFormat( " %p % | Estimated time remaining: " + QTime::fromMSecsSinceStartOfDay(remainingMiliseconds).toString("hh:mm:ss"));
            }
            ui->progressBar->setValue( currentCount );
        }
    }
}
