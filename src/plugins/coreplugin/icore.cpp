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

#include "icore.h"
#include "mainwindow.h"

#include "cantool.h"
#include "cantoolwidgets/cantracerwidget.h"

namespace Lindwurm::Core
{
    ICore* ICore::s_instance = nullptr;

    ICore::ICore(QObject *parent)
        : QObject(parent)
        , m_mainWindow( std::make_unique<MainWindow>(nullptr) )
    {
        s_instance = this;
        m_mainWindow->show();
    }

    ICore *ICore::instance()
    {
        return s_instance;
    }

    ICore::~ICore()
    {
        s_instance = nullptr;
    }

    QString ICore::version()
    {
        return "0.21";
    }
}
