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

#include "pluginmanager.h"
#include <QDebug>
#include <QLoggingCategory>

#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>
#include <QPluginLoader>
#include "iplugin.h"

namespace
{
    Q_LOGGING_CATEGORY(LOG_TAG, "lindwurm.plugin-system")
}

namespace Lindwurm::PluginSystem
{
    static PluginManager *_instance = nullptr;

    PluginManager::PluginManager() : QObject(nullptr)
    {
        _instance = this;
    }

    PluginManager::~PluginManager()
    {
        _instance = nullptr;
    }

    PluginManager *PluginManager::instance()
    {
        return _instance;
    }

    bool PluginManager::loadPlugins(const QStringList &arguments)
    {
        bool result = false;

        Q_UNUSED(arguments)

        QDir pluginsDir = QDir( QCoreApplication::instance()->applicationDirPath() );

        if ( ! pluginsDir.cd("plugins") )
        {
            qDebug(LOG_TAG) << "Failed finding the plugins directory!";
            return false;
        }

        qDebug(LOG_TAG) << "Load plugins from " << pluginsDir.absolutePath();

        QStringList loadedPlugins;

        foreach (QString fileName, pluginsDir.entryList(QDir::Files) )
        {
            QString absolutFileName = pluginsDir.absoluteFilePath(fileName);

            QFileInfo fileInfo( absolutFileName );

            // on Linux we're building the plugins with soname which creates necessary symlinks for each plugin
            // to load each plugin only once we skip these symlinks here
            if ( fileInfo.isSymLink() )
            {
                continue;
            }

            qDebug(LOG_TAG) << "Found plugin " << absolutFileName;
            QPluginLoader loader( absolutFileName );

            IPlugin *iplugin = qobject_cast<IPlugin*>( loader.instance() );

            if ( iplugin )
            {
                qInfo(LOG_TAG) << "Successfuly loaded plugin " << iplugin->pluginName();

                iplugin->setParent(this);
                iplugin->initialize(arguments);

                // if at least one plugin loaded loadPlugins returns true
                result = true;

                _plugins.insert(iplugin->pluginName(), iplugin);

                // we insert at the front because we want to call pluginsInitialized later in reverse order
                loadedPlugins.push_front( iplugin->pluginName() );
            }
            else
            {
                qWarning(LOG_TAG) << "Failed to load plugin: " << loader.errorString();
            }
        }

        foreach(const QString &pluginName, loadedPlugins)
        {
            IPlugin *plugin = _plugins[pluginName];

            if ( plugin )
            {
                plugin->pluginsInitialized();
            }
        }

        return result;
    }

    void PluginManager::shutdown()
    {
        qDebug(LOG_TAG) << "PluginManager::shutdown!";

        for (IPlugin *plugin : qAsConst(_plugins) )
        {
            plugin->shutdown();
        }
    }
}
