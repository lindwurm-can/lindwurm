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

#include "canframecomposerwidget.h"
#include "ui_canframecomposerwidget.h"
#include "canframecomposertab.h"

#include <QToolButton>
#include <QTabBar>
#include <QAction>

namespace Lindwurm::Core
{
    CanFrameComposerWidget::CanFrameComposerWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CanFrameComposerWidget)
    {
        ui->setupUi(this);

        addNewTab();

        connect(ui->composerTabs, &FancyTabWidget::tabAddRequested,     this, &CanFrameComposerWidget::addNewTab);
        connect(ui->composerTabs, &FancyTabWidget::tabCloseRequested,   this, &CanFrameComposerWidget::closeTab);
    }

    CanFrameComposerWidget::~CanFrameComposerWidget()
    {
        delete ui;
    }

    void CanFrameComposerWidget::addNewTab()
    {
        int newTabIndex = ui->composerTabs->addTab( new CanFrameComposerTab, "Frames " + QString::number(tabCounter++) );
        ui->composerTabs->setCurrentIndex(newTabIndex);
    }

    void CanFrameComposerWidget::closeTab(int index)
    {
        ui->composerTabs->widget(index)->deleteLater();
        ui->composerTabs->removeTab(index);

        // ensure at least one tab will be open or tab bar will disappear otherwise
        if ( ui->composerTabs->count() == 0 )
        {
            addNewTab();
        }
    }
}
