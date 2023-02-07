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

#include "cancomposer/canframecomposer.h"

#include "caninterface/icaninterfacehandle.h"

#include <QRegularExpression>
#include <QThread>

namespace Lindwurm::Lib
{
    CanFrameComposer::CanFrameComposer(QObject *parent)
        : QObject{parent}
        , m_canInterface()
        , m_frameComposit()
        , m_sendTimer()
        , m_updateFrameCounter(0)
        , m_updateFrameLimit(0)
        , m_loopComposing(false)
    {
        m_sendTimer.setSingleShot(false);
        connect(&m_sendTimer, &QTimer::timeout, this, &CanFrameComposer::sendNextFrame);
    }

    void CanFrameComposer::mountCANInterface(ICanInterfaceHandleSharedPtr interface)
    {
        unmountCANInterface();

        m_canInterface = interface;
    }

    void CanFrameComposer::unmountCANInterface()
    {
        if ( m_canInterface )
        {
            m_canInterface->unmount();
            m_canInterface.reset();
        }
    }

    bool CanFrameComposer::parseFrames(const QString &plainTextFrames)
    {
        return m_frameComposit.parseFrameComposit(plainTextFrames);
    }

    bool CanFrameComposer::startComposing(int interval, bool loopComposing)
    {
        if ( ! ( m_canInterface && m_canInterface->isMounted() ) )
        {
            return false;
        }

        setSendInterval(interval);
        m_loopComposing = loopComposing;

        emit composingStarted();

        sendNextFrame();

        m_sendTimer.start();

        return true;
    }

    void CanFrameComposer::pauseComposing()
    {
        m_sendTimer.stop();
        emit composingPaused();
    }

    void CanFrameComposer::continueComposing()
    {
        m_sendTimer.start();
        emit composingContinued();
    }

    void CanFrameComposer::stopComposing()
    {
        m_sendTimer.stop();
        emit composingFinished();
    }

    void CanFrameComposer::sendNextFrame()
    {
        if ( m_frameComposit.hasNext() )
        {
            QCanBusFrame frame = m_frameComposit.next();

            if ( frame.isValid() )
            {
                m_canInterface->sendFrame(frame);

                m_updateFrameCounter++;

                if ( m_updateFrameCounter >= m_updateFrameLimit)
                {
                    emit progress( m_frameComposit.currentCount(), m_frameComposit.frameCount() );
                    m_updateFrameCounter = 0;
                }
            }
        }

        // looks like a redundant check, but if we had to send a frame above, it was potentially the last frame
        // so we check it here again to stop composing immediatly to avoid waiting to the next timeout
        if ( ! m_frameComposit.hasNext() )
        {
            if ( m_loopComposing )
            {
                m_frameComposit.reset();
                emit progress( m_frameComposit.currentCount(), m_frameComposit.frameCount() );
            }
            else
            {
                emit progress( m_frameComposit.currentCount(), m_frameComposit.frameCount() );
                stopComposing();
            }
        }
    }

    void CanFrameComposer::setSendInterval(int interval)
    {
        m_sendTimer.setInterval(interval);

        if ( interval < 150 )
        {
            // if the send interval is below 150 ms we want to send an update signal only every few frames
            // to avoid flooding the GUI thread

            m_updateFrameLimit = 150.0f / interval;

            if ( m_frameComposit.frameCount() < m_updateFrameLimit )
            {
                m_updateFrameLimit = 1;
            }
        }
        else
        {
            // if the send interval is at least 100 ms it's fine to send an update signal every frame
            m_updateFrameLimit = 1;
        }
    }
}
