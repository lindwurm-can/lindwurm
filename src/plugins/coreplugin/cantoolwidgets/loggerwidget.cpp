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

#include "loggerwidget.h"
#include "ui_loggerwidget.h"

#include <QTreeWidgetItem>
#include <QStringList>
#include <QIcon>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QSettings>
#include <QMetaObject>

#include "themes/activetheme.h"

Q_DECLARE_METATYPE(QtMsgType)

namespace Lindwurm::Core
{
    LoggerWidget* LoggerWidget::s_instance;

    LoggerWidget::LoggerWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::LoggerWidget)
    {
        ui->setupUi(this);

        qRegisterMetaType<QtMsgType>();

        ui->logView->setHeaderLabels(QStringList() << "Type" << "Time" << "Source" << "Message");
        ui->logView->setColumnCount(4);
        ui->logView->setColumnWidth(0, 60);
        ui->logView->setColumnWidth(1, 180);
        ui->logView->setColumnWidth(2, 170);

        s_instance = this;
        setupToolBar();

        qInstallMessageHandler(LoggerWidget::qMessageHandler);
    }

    LoggerWidget::~LoggerWidget()
    {
        qInstallMessageHandler(0);
        delete ui;
    }

    void LoggerWidget::setupToolBar()
    {
        ui->toolBar->setStyleSheet("QToolBar { border-bottom: 1px solid gray; }");

        QAction* toolBarAction;

        toolBarAction = ui->toolBar->addAction( ActiveTheme::icon("tool-logger/clear-logs"),   "Clear log");
        connect(toolBarAction, &QAction::triggered, this, &LoggerWidget::clearLog);
        ui->logView->addAction(toolBarAction);

        QWidget* spacer = new QWidget();
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        ui->toolBar->addWidget(spacer);

        m_filterErrorAction      = addFilterAction( QIcon(":/images/icons/tool-logger/error.svg"),      "Show Errors",      "E");
        m_filterWarningAction    = addFilterAction( QIcon(":/images/icons/tool-logger/warning.svg"),    "Show Warnings",    "W");
        m_filterInfoAction       = addFilterAction( QIcon(":/images/icons/tool-logger/info.svg"),       "Show Infos",       "I");
        m_filterDebugAction      = addFilterAction( ActiveTheme::icon("tool-logger/bug"),               "Show Debugs",      "D");

        QSettings settings;

        m_filterDebugAction->setChecked(    settings.value("core/showDLog", false).toBool() );
        m_filterInfoAction->setChecked(     settings.value("core/showILog", true).toBool() );
        m_filterWarningAction->setChecked(  settings.value("core/showWLog", true).toBool() );
        m_filterErrorAction->setChecked(    settings.value("core/showELog", true).toBool() );
    }

    void LoggerWidget::clearLog()
    {
        ui->logView->clear();
    }

    void LoggerWidget::hideLogs(QString type)
    {
        QList<QTreeWidgetItem*> items = ui->logView->findItems(type, Qt::MatchExactly);
        int numberOfItems = items.size();

        for(int i = 0; i < numberOfItems; i++)
        {
            items.at(i)->setHidden(true);
        }
    }

    void LoggerWidget::showLogs(QString type)
    {
        QList<QTreeWidgetItem*> items = ui->logView->findItems(type, Qt::MatchExactly);
        int numberOfItems = items.size();

        for(int i = 0; i < numberOfItems; i++)
        {
            items.at(i)->setHidden(false);
        }
    }

    void LoggerWidget::addLog(QtMsgType type, QString category, QString message)
    {
        bool        hideItem = false;
        QStringList logColumns;
        QIcon       icon;

        switch (type)
        {
            case QtDebugMsg:

                icon = ActiveTheme::icon("tool-logger/bug");
                logColumns << "D";
                hideItem = ! s_instance->m_filterDebugAction->isChecked();

                break;

            case QtInfoMsg:

                icon = QIcon(":/images/icons/tool-logger/info.svg");
                logColumns << "I";
                hideItem = ! s_instance->m_filterInfoAction->isChecked();

                break;

            case QtWarningMsg:

                icon = QIcon(":/images/icons/tool-logger/warning.svg");
                logColumns << "W";
                hideItem = ! s_instance->m_filterWarningAction->isChecked();

                break;

            case QtCriticalMsg:

                icon = QIcon(":/images/icons/tool-logger/error.svg");
                logColumns << "E";
                hideItem = ! s_instance->m_filterErrorAction->isChecked();

                break;

            case QtFatalMsg:

                icon = QIcon(":/images/icons/tool-logger/error.svg");
                logColumns << "F";
                break;
        }

        logColumns << QLocale().toString(QDate::currentDate(), QLocale::ShortFormat) + " " + QTime::currentTime().toString() << category << message;

        QTreeWidgetItem* item = new QTreeWidgetItem(logColumns);
        item->setIcon(0, icon);

        ui->logView->insertTopLevelItem(0, item);

        item->setHidden(hideItem);

        // if user hides this log type, do not show an inserted indicator
        // except for errors/critical logs => these should always be indicated
        // (altough the message is not displayed until the user activates errors)
        if ( (! hideItem) || type == QtCriticalMsg )
        {
            emit logInserted();
        }
    }

    QAction *LoggerWidget::addFilterAction(const QIcon &icon, QString text, QString type)
    {
        QAction* filterAction = ui->toolBar->addAction(icon, text);
        filterAction->setCheckable(true);
        filterAction->setChecked(true);
        ui->logView->addAction(filterAction);

        connect(filterAction, &QAction::triggered, this, [this, filterAction, type]
        {
            QSettings settings;

            if ( filterAction->isChecked() )
            {
                settings.setValue("core/show" + type + "Log", true);
                showLogs(type);
            }
            else
            {
                settings.setValue("core/show" + type + "Log", false);
                hideLogs(type);
            }
        });

        return filterAction;
    }

    void LoggerWidget::qMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        fprintf(stderr, "%s\n", msg.toLocal8Bit().constData() );

        QString category = QString(context.category);

        if ( category == "qt.qpa.xcb" )
        {
            // Ignore messages like QXcbConnection: XCB error: 3 (BadWindow)
            return;
        }

        if ( category.contains('.') )
        {
            category = category.mid( category.lastIndexOf('.') + 1 );
        }

        // qMessageHandler could be called from any thread, since we call GUI/widget methods here
        // we queue the call into the main thread's event loop
        QMetaObject::invokeMethod(s_instance, "addLog", Qt::QueuedConnection, Q_ARG(QtMsgType, type),Q_ARG(QString, category),Q_ARG(QString, msg));
    }
}
