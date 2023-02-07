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

#ifndef CANFRAMECOMPOSERTAB_H
#define CANFRAMECOMPOSERTAB_H

#include <QWidget>

class QAction;

namespace Ui { class CanFrameComposerTab; }

namespace Lindwurm::Lib
{
    class CanFrameComposer;
}

namespace Lindwurm::Core
{
    /**
     * @brief The CanFrameComposerTab class implements the widget for a single tab in the CanFrameComposerWidget.
     *
     * It implements the controls for composing CAN frames with the composer.
     */
    class CanFrameComposerTab : public QWidget
    {
        Q_OBJECT
        public:

            /**
             * @brief Constructs a CanFrameComposerTab with the provided QObject parent.
             * @param parent the QObject parent.
             */
            explicit    CanFrameComposerTab(QWidget *parent = nullptr);
                        ~CanFrameComposerTab();

        private slots:

            void        startComposing();
            void        stopComposing();
            void        pauseComposing();
            void        continueComposing();

            void        composerProgress(quint64 currentCount, quint64 totalCount);

            void        composingFinished();
            void        composingPaused();
            void        composingContinued();

        private:

            void        setUIComposingRunning();
            void        setUIComposingPaused();
            void        setUIComposingStopped();

        private:

            Ui::CanFrameComposerTab *ui;
            QAction*                m_sendAction = { nullptr };
            QAction*                m_stopAction = { nullptr };
            Lib::CanFrameComposer*  m_frameComposer;
            int                     m_sendInterval = { 0 };
            bool                    m_composingStarted = { false };
            bool                    m_composingPaused = { false };
    };
}

#endif // CANFRAMECOMPOSERTAB_H
