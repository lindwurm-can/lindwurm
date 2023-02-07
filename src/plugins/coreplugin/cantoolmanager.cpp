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

#include "cantoolmanager.h"

#include "cantool.h"
#include "utils/tabtoolwidget.h"
#include <algorithm>
#include <QToolButton>
#include <QSettings>

namespace Lindwurm::Core
{
    CanToolManager*         CanToolManager::s_instance = nullptr;
    QList<CanToolSharedPtr> CanToolManager::s_canTools;

    CanToolManager::CanToolManager(TabToolWidget *tabToolWidget, QObject *parent)
        : QObject(parent)
        , m_canToolsWidget(tabToolWidget)
    {
        s_instance = this;
    }

    CanToolManager::~CanToolManager()
    {
        s_instance = nullptr;
    }

    void CanToolManager::shutdown()
    {
        QSettings settings;

        foreach (CanToolSharedPtr tool, s_canTools)
        {
            settings.setValue("core/" + tool->id() + ".isVisible", tool->widget()->isVisible() );
        }

        settings.setValue("core/tools.widget.geometry", m_canToolsWidget->saveToolsGeometry() );
    }

    static bool canToolComparator(const CanToolSharedPtr& first, const CanToolSharedPtr& second)
    {
        return first->index() < second->index();
    }

    void CanToolManager::addCanTool(CanToolSharedPtr tool)
    {
        s_canTools.append(tool);
    }

    void CanToolManager::pluginsInitialized()
    {
        std::sort( s_canTools.begin(), s_canTools.end(), canToolComparator );

        QSettings settings;

        foreach (CanToolSharedPtr tool, s_canTools)
        {
            QToolButton* toolButton = s_instance->m_canToolsWidget->insertTool(tool->icon(), tool->caption(), tool->widget() );
            connect(toolButton, &QToolButton::clicked, tool.get(), &CanTool::toolButtonClicked );

            bool isVisible = settings.value("core/" + tool->id() + ".isVisible", isToolVisibleByDefault(tool->id()) ).toBool();

            tool->widget()->setVisible(isVisible);
            toolButton->setChecked(isVisible);
        }

        QByteArray geometry;

        if ( settings.value("core/tools.widget.geometry").isValid() )
        {
            geometry = settings.value("core/tools.widget.geometry").toByteArray();
        }
        else
        {
            geometry = QByteArrayLiteral("\x00\x00\x00\xFF\x00\x00\x00\x01\x00\x00\x00\x06\x00\x00\x03,\x00\x00\x00\xF0\x00\x00\x01&\x00\x00\x00\xED\x00\x00\x01\r\x00\x00\x01\b\x00\xFF\xFF\xFF\xFF\x01\x00\x00\x00\x02\x00");
            settings.setValue("core/tools.widget.geometry", geometry );
        }

        s_instance->m_canToolsWidget->restoreToolsGeometry( geometry );
    }

    bool CanToolManager::isToolVisibleByDefault(const QString &toolId)
    {
        if ( (toolId == "core.tool.logger") || (toolId == "core.tool.tracer") || (toolId == "core.tool.interfacemanager") )
        {
            return true;
        }

        return false;
    }
}
