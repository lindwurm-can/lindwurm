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

#include <QApplication>
#include <QDebug>
#include <QStringList>
#include <QSettings>

#include <pluginsystem/pluginmanager.h>

using Lindwurm::PluginSystem::PluginManager;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSettings::setDefaultFormat( QSettings::IniFormat );
    QCoreApplication::setOrganizationName("lindwurm");
    QCoreApplication::setApplicationName("lindwurm");

    PluginManager pluginManager;

    if ( ! pluginManager.loadPlugins( QStringList() ) )
    {
        qCritical() << "Failed to load plugins: Abort";
        return 1;
    }

    QObject::connect(&app, &QCoreApplication::aboutToQuit, &pluginManager, &PluginManager::shutdown);

    return app.exec();
}
