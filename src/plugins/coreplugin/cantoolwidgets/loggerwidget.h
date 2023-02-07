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

#ifndef LOGGERWIDGET_H
#define LOGGERWIDGET_H

#include <QWidget>

namespace Ui { class LoggerWidget; }

class QAction;

namespace Lindwurm::Core
{
    /**
     * @brief The LoggerWidget class logs all QDebug log messages and allows to view and filter them.
     *
     * The class hooks into Qt's debug message system to catch all QDebug messages and displays them
     * in a table with timestamp, source (QLoggingCategory) and message type. The widget also allows to
     * filter the messages by their QtMsgType (debug, information, warning and error).
     *
     * To indicate which Lindwurm component was the originator of the message you can use a
     * `QLoggingCategory` with the `Q_LOGGING_CATEGORY` macro. Please refere to the `QLoggingCategory`
     * documentation.
     */
    class LoggerWidget : public QWidget
    {
        Q_OBJECT
        public:

            explicit        LoggerWidget(QWidget *parent = nullptr);
                            ~LoggerWidget();

        public slots:

            /**
             * @brief Clear all previous log messages.
             */
            void            clearLog();

            /**
             * @brief Hide all logs of a specific type.
             * @param type log type to hide ("E", "W", "I" or "D").
             */
            void            hideLogs(QString type);

            /**
             * @brief Enable all logs of a specific type.
             * @param type log type to hide ("E", "W", "I" or "D").
             */
            void            showLogs(QString type);

        signals:

            /**
             * @brief This signal is emitted whenever a new log message was inserted.
             */
            void            logInserted();

        private slots:

            void            addLog(QtMsgType type, QString category, QString message);

        private:

            void            setupToolBar();
            QAction*        addFilterAction(const QIcon &icon, QString text, QString type);
            static void     qMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

        private:

            Ui::LoggerWidget        *ui;

            static LoggerWidget*    s_instance;

            QAction*                m_filterErrorAction = { nullptr };
            QAction*                m_filterWarningAction = { nullptr };
            QAction*                m_filterInfoAction = { nullptr };
            QAction*                m_filterDebugAction = { nullptr };
    };
}
#endif // LOGGERWIDGET_H
