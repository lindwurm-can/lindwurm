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

#include "cantool.h"

namespace Lindwurm::Core
{
    CanTool::CanTool()
    {

    }

    CanTool::CanTool(const QString &id, int index, const QIcon &icon, QWidget *widget, const QString &caption)
    {
        m_ID        = id;
        m_Index     = index;
        m_Icon      = icon;
        m_Widget    = widget;
        m_Caption   = caption;
    }

    void CanTool::setID(const QString &id)
    {
        m_ID = id;
    }

    QString CanTool::id() const
    {
        return m_ID;
    }

    int CanTool::index() const
    {
        return m_Index;
    }

    void CanTool::setCaption(const QString &caption)
    {
        m_Caption = caption;
    }

    QString CanTool::caption() const
    {
        return m_Caption;
    }

    void CanTool::setWidget(QWidget *widget)
    {
        m_Widget = widget;
    }

    QWidget *CanTool::widget() const
    {
        return m_Widget;
    }

    void CanTool::setIcon(const QIcon &icon)
    {
        m_Icon = icon;
    }

    QIcon CanTool::icon() const
    {
        return m_Icon;
    }

    void CanTool::toolButtonClicked(bool checked)
    {
        m_Widget->setVisible(checked);
    }
}
