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

#ifndef CANFRAMECOMPOSER_H
#define CANFRAMECOMPOSER_H

#include "lindwurmlib_global.h"

#include <QObject>
#include <QCanBusFrame>
#include <QTimer>

#include "caninterface/icaninterfacehandlesharedptr.h"
#include "cancomposer/canframecomposit.h"

class QThread;

namespace Lindwurm::Lib
{
    class LINDWURMLIB_EXPORT CanFrameComposer : public QObject
    {
        Q_OBJECT
        public:

            explicit CanFrameComposer(QObject *parent = nullptr);

            void            mountCANInterface(ICanInterfaceHandleSharedPtr interface);
            void            unmountCANInterface();

            bool            parseFrames(const QString &plainTextFrames);
            bool            startComposing(int interval, bool loopComposing = false);
            void            pauseComposing();
            void            continueComposing();
            void            stopComposing();

        signals:

            void            composingStarted();
            void            progress(quint64 currentCount, quint64 totalCount);
            void            composingPaused();
            void            composingContinued();
            void            composingFinished();

        private slots:

            void            sendNextFrame();

        private:

            void            setSendInterval(int interval);

        private:

            ICanInterfaceHandleSharedPtr    m_canInterface;
            CanFrameComposit                m_frameComposit;
            QTimer                          m_sendTimer;
            unsigned int                    m_updateFrameCounter;
            unsigned int                    m_updateFrameLimit;
            bool                            m_loopComposing;
    };
}

#endif // CANFRAMECOMPOSER_H
