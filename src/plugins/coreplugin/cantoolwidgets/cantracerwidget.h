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

#ifndef CANTRACERWIDGET_H
#define CANTRACERWIDGET_H

#include <QWidget>
#include <QPair>
#include <QString>
#include <QList>
#include <QPointer>

namespace Ui { class CanTracerWidget; }

class QAbstractItemModel;
class QMenu;

namespace Lindwurm::Lib
{
    class CanFrameTracer;
    class CanFrameFilterProxyModel;
}

namespace Lindwurm::Core
{
    using FilterBookmark = QPair<QString,QString>;
    using FilterBookmarkList = QList<FilterBookmark>;

    class CanTracerFilterBookmarksDialog;

    /**
     * @brief The CanTracerWidget class provides a widget for capturing and filtering CAN trace logs.
     */
    class CanTracerWidget : public QWidget
    {
        Q_OBJECT
        public:

            /**
             * @brief Constructs a CanTracerWidget with the provided QObject parent.
             * @param parent the QObject parent.
             */
            explicit                        CanTracerWidget(QWidget *parent = nullptr);
                                            ~CanTracerWidget();

            QSet<QString>                   selectedIds();
            QSet<QString>                   allDisplayedIds();

        private slots:

            void                            startTrace();
            void                            stopTrace();

            void                            applyViewFilter(bool addToRecentUsed);
            void                            clearViewFilter();
            void                            setViewFilterActiveIndication();
            void                            setViewFilterInactiveIndication();
            void                            setViewFilterEditedIndication();
            void                            applyFilterBookmark(int index);

            void                            filterBookmarksDialogFinished(int result);

        private:

            enum FilterType
            {
                PassFilter,
                BlockFilter
            };

            void                            setupToolBar();
            void                            setupContextMenu();
            void                            setModel(QAbstractItemModel *model);
            void                            resizeTraceViewColumnsToContents();

            void                            addToRecentUsedFilters(const QString &filter);
            void                            loadRecentUsedFilters();
            void                            loadFilterBookmarks();

            void                            setIdSetAsFilter(const QSet<QString> &idSet, CanTracerWidget::FilterType type);
            void                            appendIdSetToFilter(const QSet<QString> &idSet);

            QString                         idSetToFilterString(const QSet<QString> &ids);

        private:

            Ui::CanTracerWidget                         *ui;
            Lib::CanFrameTracer*                        m_tracer = { nullptr };

            Lib::CanFrameFilterProxyModel*              m_filterModel = { nullptr };
            QString                                     m_currentFilter = {};

            QAction*                                    m_startAction = { nullptr };
            QAction*                                    m_stopAction = { nullptr };
            QAction*                                    m_autoScrollAction = { nullptr };
            bool                                        m_tracerViewAtBottom = { false };

            QAction*                                    m_toggleViewModeAction = { nullptr };
            QAction*                                    m_saveAction = { nullptr };
            QAction*                                    m_openAction = { nullptr };
            QAction*                                    m_clearFilterAction = { nullptr };
            QAction*                                    m_applyFilterAction = { nullptr };

            QMenu*                                      m_filterBookmarksMenu = { nullptr };
            QAction*                                    m_editBookmarksAction = { nullptr };

            FilterBookmarkList                          m_filterBookmarks = {};
            QPointer<CanTracerFilterBookmarksDialog>    m_filterBookmarksDialog = {};
    };
}

#endif // CANTRACERWIDGET_H
