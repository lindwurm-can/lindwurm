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

#include "cantracerwidget.h"
#include "ui_cantracerwidget.h"
#include "caninterface/icaninterfacemanager.h"
#include "caninterface/caninterfacehandle.h"

#include "cantracer/canframetracer.h"
#include "cantracer/linearcanframetracermodel.h"
#include "cantracer/aggregatedcanframetracermodel.h"
#include "cantracer/canframefilterproxymodel.h"
#include "dialogs/cantracerfilterbookmarksdialog.h"

#include "themes/activetheme.h"

#include <QLineEdit>
#include <QKeyEvent>
#include <QScrollBar>
#include <QSettings>
#include <QListIterator>
#include <QVariantMap>
#include <QMenu>

#include <QDebug>
#include <QLoggingCategory>
#include <QKeyEvent>

namespace
{
    const char*     COMPONENT_NAME = "CAN Tracer";
    const int       MAX_VIEW_FILTER_HISTORY = 10;
    Q_LOGGING_CATEGORY(LOG_TAG, "lindwurm.tracer")
}

using namespace Lindwurm::Lib;

namespace Lindwurm::Core
{
    CanTracerWidget::CanTracerWidget(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::CanTracerWidget)
        , m_tracer( new Lib::CanFrameTracer(this) )
    {
        ui->setupUi(this);

        setupToolBar();

        ui->viewFilterBox->lineEdit()->setPlaceholderText("Apply ID filter ... ( e.g. 1AF, 33, 50 )");

        connect(ui->viewFilterBox->lineEdit(), &QLineEdit::returnPressed, this, &CanTracerWidget::applyViewFilter);
        connect(ui->viewFilterBox->lineEdit(), &QLineEdit::textChanged, this, &CanTracerWidget::setViewFilterEditedIndication);
        connect(ui->cmbFilterType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CanTracerWidget::setViewFilterEditedIndication);

        m_applyFilterAction = ui->viewFilterBox->lineEdit()->addAction( ActiveTheme::icon("tool-tracer/apply-filter"), QLineEdit::TrailingPosition);
        m_applyFilterAction->setEnabled(false);
        connect(m_applyFilterAction, &QAction::triggered, this, &CanTracerWidget::applyViewFilter);

        m_clearFilterAction = ui->viewFilterBox->lineEdit()->addAction( ActiveTheme::icon("tool-tracer/clear-filter"), QLineEdit::TrailingPosition);
        m_clearFilterAction->setVisible(false);
        connect(m_clearFilterAction, &QAction::triggered, this, &CanTracerWidget::clearViewFilter);

        ui->bookmarksButton->setIcon( ActiveTheme::icon("tool-tracer/filter-bookmarks") );
        ui->bookmarksButton->setIconSize( QSize(20,20) );
        ui->bookmarksButton->setStyleSheet("QToolButton::menu-indicator{width:0px;}");

        loadFilterBookmarks();
        loadRecentUsedFilters();

        setModel( new Lib::AggregatedCanFrameTracerModel(m_tracer, m_tracer)  );

        ui->cmbFilterType->addItem("Pass", false);
        ui->cmbFilterType->addItem("Block", true);

        ui->selectInterfaceBox->setStyleSheet("QComboBox { padding-left: 10px; padding-right: 10px; height: 24px; }");

        // TODO: Implement me
        //setupContextMenu();

        ui->selectInterfaceBox->setModel( Lib::ICanInterfaceManager::instance()->interfaceListModel() );
    }

    CanTracerWidget::~CanTracerWidget()
    {
        delete ui;
    }

    void CanTracerWidget::startTrace()
    {
        QString interfaceId = ui->selectInterfaceBox->currentData().toString();

        ICanInterfaceHandleSharedPtr interface = ICanInterfaceManager::instance()->mountInterface(interfaceId, COMPONENT_NAME);

        if ( ! interface )
        {
            qCritical(LOG_TAG) << "Count not mount interface";
            return;
        }

        m_startAction->setEnabled(false);
        m_stopAction->setEnabled(true);
        m_openAction->setEnabled(false);
        m_saveAction->setEnabled(false);

        ui->selectInterfaceBox->setEnabled(false);

        CanFrameTracer* oldTracer = m_tracer;

        m_tracer = new CanFrameTracer(this);

        if ( m_toggleViewModeAction->isChecked() )
        {
            setModel( new Lib::LinearCanFrameTracerModel(m_tracer, m_tracer)  );
        }
        else
        {
            setModel( new Lib::AggregatedCanFrameTracerModel(m_tracer, m_tracer)  );
        }

        resizeTraceViewColumnsToContents();

        oldTracer->deleteLater();

        m_tracer->mountCANInterface(interface);

        m_tracer->start();
    }

    void CanTracerWidget::stopTrace()
    {
        m_tracer->stop();
        m_tracer->unmountCANInterface();

        ui->selectInterfaceBox->setEnabled(true);

        m_startAction->setEnabled(true);
        m_startAction->setChecked(false);
        m_stopAction->setEnabled(false);

        // TODO: Enable if actions are implemented
        //m_openAction->setEnabled(true);
        //m_saveAction->setEnabled(true);
    }

    void CanTracerWidget::applyViewFilter()
    {
        bool blockFilter = ui->cmbFilterType->currentData().toBool();
        QString filterString = ui->viewFilterBox->lineEdit()->text();

        filterString = filterString.replace('!', "");
        filterString = filterString.replace(' ', "");
        filterString = filterString.replace(',', '|');

        if ( filterString.isEmpty() )
        {
            setViewFilterInactiveIndication();
            m_filterModel->clearFilter();
            m_currentFilter.clear();
        }
        else
        {
            filterString = "(?i)^ *(" + filterString + ")$";

            if ( blockFilter )
            {
                m_filterModel->setBlockFilterRegularExpression(filterString);
                m_currentFilter = "Block: " + ui->viewFilterBox->lineEdit()->text();
            }
            else
            {
                m_filterModel->setPassFilterRegularExpression(filterString);
                m_currentFilter = "Pass: " + ui->viewFilterBox->lineEdit()->text();
            }

            setViewFilterActiveIndication();
        }

        addToRecentUsedFilters( ui->viewFilterBox->lineEdit()->text() );
    }

    void CanTracerWidget::clearViewFilter()
    {
        ui->viewFilterBox->lineEdit()->setText("");
        applyViewFilter();
    }

    void CanTracerWidget::setViewFilterActiveIndication()
    {
        m_clearFilterAction->setVisible(true);
        m_applyFilterAction->setEnabled(false);
        ui->viewFilterBox->lineEdit()->setStyleSheet("QLineEdit { background-color: #97c170; color: black;}");

        m_clearFilterAction->setIcon( ActiveTheme::icon("tool-tracer/clear-filter-active") );
        m_applyFilterAction->setIcon( ActiveTheme::icon("tool-tracer/apply-filter-active") );

        // if a filter currently is active, but the line edit is cleared, show the currently active filter as place holder text
        ui->viewFilterBox->lineEdit()->setPlaceholderText("Current filter: " + m_currentFilter);
    }

    void CanTracerWidget::setViewFilterInactiveIndication()
    {
        m_clearFilterAction->setVisible(false);
        ui->viewFilterBox->lineEdit()->setStyleSheet("");
        m_clearFilterAction->setIcon( ActiveTheme::icon("tool-tracer/clear-filter") );
        m_applyFilterAction->setIcon( ActiveTheme::icon("tool-tracer/apply-filter") );

        // set the default place holder text, if currently no filter is active
        ui->viewFilterBox->lineEdit()->setPlaceholderText("Apply ID filter ... ( e.g. 1AF, 33, 50 )");
        m_applyFilterAction->setEnabled(false);
    }

    void CanTracerWidget::setViewFilterEditedIndication()
    {
        // check if the filter line edit is empty, e.g. there is currently no filter input that can be applied
        // and enable/disable the apply/clear filter action accordingly
        if ( ui->viewFilterBox->lineEdit()->text().isEmpty() )
        {
            m_applyFilterAction->setEnabled(false);
            m_clearFilterAction->setVisible(false);
        }
        else
        {
            m_applyFilterAction->setEnabled(true);
            m_clearFilterAction->setVisible(true);
        }

        if ( ! m_currentFilter.isEmpty() )
        {
            // if there is currently a filter active, but the filter line edit was modified
            // we change the background of the filter line to indicate that the currently shown input differs from the currently set filter
            ui->viewFilterBox->lineEdit()->setStyleSheet("QLineEdit { background-color: #7097c170; color: black;}");
            m_clearFilterAction->setIcon( ActiveTheme::icon("tool-tracer/clear-filter-active") );
            m_clearFilterAction->setVisible(true);
            m_applyFilterAction->setEnabled(true);
        }
    }

    void CanTracerWidget::applyFilterBookmark(int index)
    {
        ui->viewFilterBox->lineEdit()->setText( m_filterBookmarks.at(index).second );

        // send a enter press event instead of calling applyViewFilter() directly
        // to add the filter to the recent history of the viewFilterBox
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
        ui->viewFilterBox->lineEdit()->event(&keyPress);
    }

    void CanTracerWidget::filterBookmarksDialogFinished(int result)
    {
        if ( result == QDialog::Accepted )
        {
            m_filterBookmarks.clear();

            m_filterBookmarks = m_filterBookmarksDialog->bookmarks();

            QSettings settings;
            settings.setValue("core/tracer.filter-bookmarks", QVariant::fromValue(m_filterBookmarks) );

            loadFilterBookmarks();
        }

        delete m_filterBookmarksDialog;
    }

    void CanTracerWidget::setupToolBar()
    {
        ui->toolBar->setStyleSheet("QToolBar { border-bottom: 1px solid gray; }");

        ui->selectInterfaceBox->setMinimumWidth(150);
        ui->toolBar->addWidget(ui->selectInterfaceBox);

        ui->toolBar->addSeparator();

        m_startAction = ui->toolBar->addAction( ActiveTheme::icon("tool-tracer/start"), "Start trace");
        m_startAction->setCheckable(true);
        m_startAction->setChecked(false);
        connect(m_startAction, &QAction::triggered, this, [this]
        {
            if ( m_startAction->isChecked() )
            {
                startTrace();
            }
        });

        connect(ui->selectInterfaceBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index)
        {
            if (  index == -1 )
            {
                // if no interfaces are currently available disable start action
                m_startAction->setEnabled(false);
            }
            else
            {
                // and if at least one interface is available enable start action
                m_startAction->setEnabled(true);
            }
        });

        // if no interfaces are available (yet) disable start action
        if ( ui->selectInterfaceBox->count() == 0)
        {
            m_startAction->setEnabled(false);
        }

        m_stopAction = ui->toolBar->addAction( ActiveTheme::icon("tool-tracer/stop"), "Stop trace");
        m_stopAction->setCheckable(false);
        m_stopAction->setEnabled(false);
        connect(m_stopAction, &QAction::triggered, this, [this]
        {
            stopTrace();
        });

        ui->toolBar->addSeparator();

        m_openAction = ui->toolBar->addAction( ActiveTheme::icon("tool-tracer/open-trace"), "Open trace file");
        // TODO: Implement me and enable action
        m_openAction->setEnabled(false);
        //connect(_openAction, &QAction::triggered, this, &CanTracerWidget::loadTraceFile);

        m_saveAction = ui->toolBar->addAction( ActiveTheme::icon("tool-tracer/save-trace"), "Save trace file");
        m_saveAction->setEnabled(false);
        // TODO: Implement me
        //connect(_saveAction, &QAction::triggered, this, &CanTracerWidget::saveActionTriggered);

        ui->toolBar->addSeparator();

        m_autoScrollAction = ui->toolBar->addAction( ActiveTheme::icon("tool-tracer/autoscroll-enable"), "Automatically scroll to the last frame during tracing");
        m_autoScrollAction->setCheckable(true);
        m_autoScrollAction->setChecked(true);

        connect(m_autoScrollAction, &QAction::triggered, this, [this]
        {
            if ( m_autoScrollAction->isChecked() )
            {
                // if the user activates auto scroll, we immediatly scroll to the bottom because for auto scroll we asure that the view always is at the bottom
                // if the view is not at the bottom anymore (e.g. the user manually scrolled) we automatically deactivate auto scroll
                ui->traceView->scrollToBottom();
            }
        });

        // Setup view model toggle action

        m_toggleViewModeAction = ui->toolBar->addAction( ActiveTheme::icon("tool-tracer/view-single"), "Switch linear/aggregated view");
        m_toggleViewModeAction->setCheckable(true);
        m_toggleViewModeAction->setChecked(false);

        connect(m_toggleViewModeAction, &QAction::triggered, this, [this]
        {
            if ( m_toggleViewModeAction->isChecked() )
            {
                m_toggleViewModeAction->setIcon( ActiveTheme::icon("tool-tracer/view-aggregate") );
                setModel( new Lib::LinearCanFrameTracerModel(m_tracer, m_tracer)  );
            }
            else
            {
                m_toggleViewModeAction->setIcon( ActiveTheme::icon("tool-tracer/view-single") );
                setModel( new Lib::AggregatedCanFrameTracerModel(m_tracer, m_tracer)  );
            }
        });
    }

    void CanTracerWidget::setModel(QAbstractItemModel *model)
    {
        if ( m_filterModel != nullptr )
        {
            delete m_filterModel;
        }

        m_filterModel = new Lib::CanFrameFilterProxyModel();
        model->setParent(m_filterModel);

        m_filterModel->setSourceModel(model);
        m_filterModel->setFilterKeyColumn(2);

        applyViewFilter();
        ui->traceView->setModel(m_filterModel);

        if ( m_autoScrollAction->isChecked() )
        {
            ui->traceView->scrollToBottom();
        }

        ui->traceView->sortByColumn(0, Qt::AscendingOrder);

        // setup auto scroll handling

        connect(m_filterModel, &QAbstractItemModel::rowsAboutToBeInserted, ui->traceView, [&]
        {
            QScrollBar* bar = ui->traceView->verticalScrollBar();
            m_tracerViewAtBottom = bar ? (bar->value() == bar->maximum()) : false;

            // if the user manually scrolled in the view (e.g. the view is not at the bottom anymore), disable auto scroll
            if ( ! m_tracerViewAtBottom )
            {
                m_autoScrollAction->setChecked(false);
            }
        });

        connect(m_filterModel, &QAbstractItemModel::rowsInserted, ui->traceView, [&]
        {
            if ( m_tracerViewAtBottom && m_autoScrollAction->isChecked() )
            {
                ui->traceView->scrollToBottom();
            }

            if ( m_tracerViewAtBottom && ! m_autoScrollAction->isChecked() )
            {
                m_autoScrollAction->setChecked(true);
            }

        });

        resizeTraceViewColumnsToContents();
    }

    void CanTracerWidget::resizeTraceViewColumnsToContents()
    {
        int columnCount = m_filterModel->columnCount();

        for (int i = 0; i < columnCount; i++)
        {
            ui->traceView->resizeColumnToContents(i);
        }
    }

    void CanTracerWidget::addToRecentUsedFilters(const QString &filter)
    {
        if ( filter.isEmpty() )
        {
            return;
        }

        QSettings settings;
        QVariantList recentFilters = settings.value("core/tracer.recent-filters").toList();

        if ( ! recentFilters.contains(filter)  )
        {
            recentFilters.append(filter);
        }

        if ( recentFilters.size() > MAX_VIEW_FILTER_HISTORY )
        {
            recentFilters.pop_front();
            ui->viewFilterBox->removeItem( MAX_VIEW_FILTER_HISTORY );
        }

        settings.setValue("core/tracer.recent-filters", recentFilters);
    }

    void CanTracerWidget::loadRecentUsedFilters()
    {
        QSettings settings;
        QVariantList recentFilters = settings.value("core/tracer.recent-filters").toList();

        ui->viewFilterBox->clear();

        QListIterator<QVariant> filter(recentFilters);
        filter.toBack();
        while ( filter.hasPrevious() )
        {
            ui->viewFilterBox->addItem( filter.previous().toString() );
        }

        // we want to have the recent filters only in the drop down box, but not activated
        ui->viewFilterBox->lineEdit()->clear();
    }

    void CanTracerWidget::loadFilterBookmarks()
    {
        if ( m_filterBookmarksMenu )
        {
            delete m_filterBookmarksMenu;
        }

        QSettings settings;

        m_filterBookmarks = settings.value("core/tracer.filter-bookmarks").value<FilterBookmarkList>();

        m_filterBookmarksMenu = new QMenu(this);

        m_filterBookmarksMenu->addAction("Bookmark this filter");
        m_editBookmarksAction = m_filterBookmarksMenu->addAction("Edit bookmarks");
        connect(m_editBookmarksAction, &QAction::triggered, this, [this]
        {
            if ( ! m_filterBookmarksDialog )
            {
                m_filterBookmarksDialog = new CanTracerFilterBookmarksDialog(this);
                connect(m_filterBookmarksDialog, &QDialog::finished, this, &CanTracerWidget::filterBookmarksDialogFinished);
            }

            m_filterBookmarksDialog->editBookmarks(m_filterBookmarks);
        });


        m_filterBookmarksMenu->addSeparator();

        int numberOfBookmarks = m_filterBookmarks.size();

        for (int i = 0; i < numberOfBookmarks; i++)
        {
            const FilterBookmark &bookmark = m_filterBookmarks.at(i);

            QAction* bookmarkAction = m_filterBookmarksMenu->addAction( bookmark.first + ": " + bookmark.second );
            connect(bookmarkAction, &QAction::triggered, this, [this,i]
            {
                this->applyFilterBookmark(i);
            });
        }

        ui->bookmarksButton->setMenu(m_filterBookmarksMenu);
    }
}
