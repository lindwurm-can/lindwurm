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

#ifndef CANFRAMECOMPOSERWIDGET_H
#define CANFRAMECOMPOSERWIDGET_H

#include <QWidget>

namespace Ui { class CanFrameComposerWidget; }

namespace Lindwurm::Core
{
    /**
     * @brief The CanFrameComposerWidget class implements the basic widget for the CAN frame composer tool.
     */
    class CanFrameComposerWidget : public QWidget
    {
        Q_OBJECT

        public:

            /**
             * @brief Constructs a CanFrameComposerWidget with the provided QWidget parent
             * @param parent the QWidget parent for this widget.
             */
            explicit CanFrameComposerWidget(QWidget *parent = nullptr);
            ~CanFrameComposerWidget();

        private slots:

            void    addNewTab();
            void    closeTab(int index);

        private:

            Ui::CanFrameComposerWidget *ui;
            int tabCounter = { 1 };
    };
}

#endif // CANFRAMECOMPOSERWIDGET_H
