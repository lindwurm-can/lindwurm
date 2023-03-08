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

#ifndef FANCYTABWIDGET_H
#define FANCYTABWIDGET_H

#include <QTabWidget>

namespace Lindwurm::Core
{
    /**
     * @brief The FancyTabWidget class extends a QTabWidget with editable tab names and an add tab button.
     *
     * The FancyTabWidget does not automatically insert a new tab when the add tab button is clicked. Instead
     * the tabAddRequested signal is emitted.
     */
    class FancyTabWidget : public QTabWidget
    {
        Q_OBJECT
        public:

            explicit    FancyTabWidget(QWidget *parent = nullptr);

        signals:

            /**
             * @brief This signal is emitted when the add tab button was clicked. A new tab is
             * not automatically added to the widget.
             */
            void        tabAddRequested();

            /**
             * @brief This signal is emitted when the caption of a tab was changed.
             * @param index the index of the tab.
             * @param text the new text of the tab at index.
             */
            void        tabTextChanged(int index, const QString &text);

        protected slots:

            void        addTabButtonClicked(bool checked);
            void        paintEvent(QPaintEvent *event) override;
            void        onTabBarDoubleClick(int tabIndex);

            void        startTabNameEditing(int tabIndex);
            void        finishTabNameEditing();
            void        adjustTabLineEdit();

        protected:

            int         m_editingTabIndex = { -1 };
    };
}

#endif // FANCYTABWIDGET_H
