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

#include "coreplugin.h"
#include <QDebug>
#include <QLoggingCategory>

#include <QPair>
#include <QString>
#include <QList>

#include "icore.h"
#include "coreconstants.h"
#include "optionspages/generalsettings.h"
#include "optionspages/uisettings.h"

#include "dialogs/settingsdialog.h"

#include "cantool.h"
#include "cantoolwidgets/cantracerwidget.h"
#include "cantoolmanager.h"
#include "cantoolwidgets/caninterfacemanagerwidget.h"
#include "cantoolwidgets/loggerwidget.h"

namespace
{
    Q_LOGGING_CATEGORY(LOG_TAG, "lindwurm.core")
}

namespace Lindwurm::Core
{
    using StringPair = QPair<QString,QString>;
    using StringPairList = QList<StringPair>;

    QDataStream &operator<<(QDataStream &out, const StringPairList &list)
    {
        int s = list.size();
        out << s;

        if (s)
        {
            for (int i = 0; i < s; ++i)
            {
                out << list[i].first << list[i].second;
            }
        }

        return out;
    }

    QDataStream &operator>>(QDataStream &in, StringPairList &list)
    {
        if ( !list.empty() )
        {
            list.clear();
        }

        int s;
        in >> s;

        if (s)
        {
            list.reserve(s);
            for (int i = 0; i < s; ++i)
            {
                QString first, second;
                in >> first >> second;
                list.append( QPair<QString, QString>(first, second) );
            }
        }

        return in;
    }

    CorePlugin::CorePlugin()
    {
    }

    CorePlugin::~CorePlugin()
    {
    }

    bool CorePlugin::initialize(const QStringList &arguments)
    {
        Q_UNUSED(arguments)

        qRegisterMetaTypeStreamOperators<StringPairList>("StringPairList");

        m_core = new ICore(this);

        CanToolManager::addCanTool( std::make_shared<CanTool>(Constants::TOOL_ID_LOGGER,      Constants::TOOL_INDEX_LOGGER,       QIcon(":/images/icons/tool-logger/logger.png"), new LoggerWidget, "Log") );
        CanToolManager::addCanTool( std::make_shared<CanTool>(Constants::TOOL_ID_INTERFACES,  Constants::TOOL_INDEX_INTERFACES,   QIcon(":/images/icons/tool-interfaces/interfaces.png"), new CanInterfaceManagerWidget, "Interfaces") );
        CanToolManager::addCanTool( std::make_shared<CanTool>(Constants::TOOL_ID_TRACER,      Constants::TOOL_INDEX_TRACER,       QIcon(":/images/icons/tool-tracer/tracer.png"), new CanTracerWidget, "Tracer") );

        SettingsDialog::addOptionsPage( std::make_unique<GeneralSettings>() );

        qDebug(LOG_TAG) << "CorePlugin is initialized!";

        return true;
    }

    void CorePlugin::pluginsInitialized()
    {
        CanToolManager::pluginsInitialized();
    }

    bool CorePlugin::shutdown()
    {
        qDebug(LOG_TAG) << "CorePlugin is shutting down!";
        delete m_core;
        m_core = nullptr;

        return true;
    }

    QString CorePlugin::errorString()
    {
        return "";
    }

    QString CorePlugin::pluginName()
    {
        return "Lindwurm::Core";
    }
}
