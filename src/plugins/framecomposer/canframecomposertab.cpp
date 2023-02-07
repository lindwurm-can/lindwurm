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

#include "canframecomposertab.h"
#include "ui_canframecomposertab.h"

#include "caninterface/icaninterfacemanager.h"
#include "cancomposer/canframecomposer.h"
#include "themes/activetheme.h"

#include <QAction>
#include <QTime>

#include <QDebug>
#include <QLoggingCategory>

namespace
{
    const char*     COMPONENT_NAME = "CAN Composer";
    Q_LOGGING_CATEGORY(LOG_TAG, "lindwurm.composer")
}

using namespace Lindwurm::Lib;

namespace Lindwurm::Core
{
    CanFrameComposerTab::CanFrameComposerTab(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CanFrameComposerTab),
        m_frameComposer( new CanFrameComposer(this ) )
    {
        ui->setupUi(this);

        connect(m_frameComposer, &CanFrameComposer::progress,           this, &CanFrameComposerTab::composerProgress);
        connect(m_frameComposer, &CanFrameComposer::composingFinished,  this, &CanFrameComposerTab::composingFinished);
        connect(m_frameComposer, &CanFrameComposer::composingPaused,    this, &CanFrameComposerTab::composingPaused);
        connect(m_frameComposer, &CanFrameComposer::composingContinued, this, &CanFrameComposerTab::composingContinued);

        //ui->toolBar->setStyleSheet("background-color: palette(window);");

        ui->selectInterfaceBox->setModel( Lib::ICanInterfaceManager::instance()->interfaceListModel() );
        ui->selectInterfaceBox->setMinimumWidth(150);

        ui->toolBar->layout()->setMargin(0);
        ui->toolBar->addWidget(ui->selectInterfaceBox);

        ui->toolBar->addSeparator();

        m_sendAction = ui->toolBar->addAction( ActiveTheme::icon("tool-composer/send"), "Send frames" );
        m_sendAction->setCheckable(false);
        m_sendAction->setChecked(false);

        connect(m_sendAction, &QAction::triggered, this, [this]
        {
            if ( ! m_composingStarted )
            {
                startComposing();
                return;
            }

            if ( m_composingPaused )
            {
                continueComposing();
            }
            else
            {
                pauseComposing();
            }

        });

        connect(ui->selectInterfaceBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index)
        {
            if (  index == -1 )
            {
                // if no interfaces are currently available disable send action
                m_sendAction->setEnabled(false);
            }
            else
            {
                // and if at least one interface is available enable send action
                m_sendAction->setEnabled(true);
            }
        });

        // if no interfaces are available (yet) disable send action
        if ( ui->selectInterfaceBox->count() == 0)
        {
            m_sendAction->setEnabled(false);
        }

        m_stopAction = ui->toolBar->addAction( ActiveTheme::icon("tool-composer/stop"), "Stop composing" );
        m_stopAction->setCheckable(false);
        m_stopAction->setChecked(false);
        m_stopAction->setEnabled(false);

        connect(m_stopAction, &QAction::triggered, this, [this]
        {
            stopComposing();
        });

        ui->toolBar->addSeparator();

        ui->toolBar->addWidget(ui->label);
        ui->toolBar->addWidget(ui->sendInterval);

        QWidget* spacer = new QWidget();
        spacer->setMinimumWidth(10);

        ui->toolBar->addWidget(spacer);
        ui->toolBar->addWidget(ui->chkLoopEnabled);
    }

    CanFrameComposerTab::~CanFrameComposerTab()
    {
        delete ui;
    }

    void CanFrameComposerTab::startComposing()
    {
        bool toIntOk;
        m_sendInterval = ui->sendInterval->text().toInt(&toIntOk);

        if ( !toIntOk )
        {
            qCritical(LOG_TAG) << "Could not convert send interval to number: " << ui->sendInterval->text();
            return;
        }

        QString interfaceId = ui->selectInterfaceBox->currentData().toString();

        // TODO: Each tab mounts an interface with the same component name which is currently not handled well
        // by the CanInterfaceManager/CanInterfaces
        // if the same interface is currently mounted on one tab and will be unmounted on another tab it is
        // completely unmounted and could be deleted by accident.
        ICanInterfaceHandleSharedPtr interface = ICanInterfaceManager::instance()->mountInterface(interfaceId, COMPONENT_NAME);

        if ( ! interface )
        {
            qCritical(LOG_TAG) << "Count not mount interface";
            return;
        }

        if ( m_frameComposer->parseFrames( ui->composerText->toPlainText() ) )
        {
            m_frameComposer->mountCANInterface( interface );

            ui->progressBar->setValue(0);
            ui->progressBar->setMaximum(100);

            setUIComposingRunning();

            m_composingStarted = true;
            m_composingPaused = false;

            m_frameComposer->startComposing(m_sendInterval, ui->chkLoopEnabled->isChecked() );
        }
    }

    void CanFrameComposerTab::stopComposing()
    {
        m_frameComposer->stopComposing();
    }

    void CanFrameComposerTab::pauseComposing()
    {
        m_frameComposer->pauseComposing();
    }

    void CanFrameComposerTab::continueComposing()
    {
        m_frameComposer->continueComposing();
    }

    void CanFrameComposerTab::composerProgress(quint64 currentCount, quint64 totalCount)
    {
        int percent = ( (float) currentCount / totalCount ) * 100;
        ui->progressBar->setValue(percent);

        int remainingTime = (totalCount - currentCount) * m_sendInterval;

        ui->statusLabel->setText(QString::number(currentCount) + " / " + QString::number(totalCount) + " Frames | Time remaining: " + QTime::fromMSecsSinceStartOfDay(remainingTime).toString("mm:ss.zzz"));
    }

    void CanFrameComposerTab::composingFinished()
    {
        m_frameComposer->unmountCANInterface();

        m_composingStarted = false;
        m_composingPaused = false;

        setUIComposingStopped();
    }

    void CanFrameComposerTab::composingPaused()
    {
        m_composingPaused = true;
        setUIComposingPaused();
    }

    void CanFrameComposerTab::composingContinued()
    {
        m_composingPaused = false;
        setUIComposingRunning();
    }

    void CanFrameComposerTab::setUIComposingRunning()
    {
        ui->selectInterfaceBox->setEnabled(false);
        ui->sendInterval->setEnabled(false);
        ui->composerText->setEnabled(false);
        ui->chkLoopEnabled->setEnabled(false);

        m_sendAction->setIcon( ActiveTheme::icon("tool-composer/pause") );

        m_sendAction->setEnabled(true);
        m_stopAction->setEnabled(true);
    }

    void CanFrameComposerTab::setUIComposingPaused()
    {
        ui->selectInterfaceBox->setEnabled(false);
        ui->sendInterval->setEnabled(false);
        ui->composerText->setEnabled(false);
        ui->chkLoopEnabled->setEnabled(false);

        m_sendAction->setIcon( ActiveTheme::icon("tool-composer/send") );

        m_sendAction->setEnabled(true);
        m_stopAction->setEnabled(true);
    }

    void CanFrameComposerTab::setUIComposingStopped()
    {
        ui->selectInterfaceBox->setEnabled(true);
        ui->sendInterval->setEnabled(true);
        ui->composerText->setEnabled(true);
        ui->chkLoopEnabled->setEnabled(true);

        m_sendAction->setIcon( ActiveTheme::icon("tool-composer/send") );

        m_sendAction->setEnabled(true);
        m_stopAction->setEnabled(false);
    }
}
