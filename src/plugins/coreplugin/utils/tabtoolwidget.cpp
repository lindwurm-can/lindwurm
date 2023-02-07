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

#include "tabtoolwidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QToolButton>
#include <QSplitter>

namespace Lindwurm::Core
{
    TabToolWidget::TabToolWidget(QWidget *parent) : QWidget(parent)
    {
        m_tabBar        = new QWidget(this);

        m_tabBar->setStyleSheet("QWidget { background:  rgba(64, 66, 68, 255); color: #d0d0d0; border: none; border-right: 1px solid #1E1F20; } QToolButton { width: 100%; font-size: 9pt; padding-top: 5px; } QToolButton:hover { background-color: rgba(89, 91, 93, 255); } QToolButton:checked { background-color: rgba(38, 40, 41, 255);}");
        m_tabBar->setMinimumSize(QSize(74, 16777215));
        m_tabBar->setMaximumSize(QSize(74, 16777215));

        m_tabBarLayout = new QVBoxLayout(m_tabBar);
        m_tabBarLayout->setSpacing(0);
        m_tabBarLayout->setContentsMargins(0, 0, 0, 0);
        m_tabBarLayout->addItem( new QSpacerItem(20, 536, QSizePolicy::Minimum, QSizePolicy::Expanding) );

        m_toolWidgetSplitter = new QSplitter(Qt::Vertical, this);
        m_toolWidgetSplitter->setChildrenCollapsible(false);

        QHBoxLayout *mainLayout = new QHBoxLayout(this);

        mainLayout->setSpacing(0);
        mainLayout->setContentsMargins(0, 0, 0, 0);

        mainLayout->addWidget(m_tabBar);
        mainLayout->addWidget(m_toolWidgetSplitter);
    }

    QToolButton* TabToolWidget::insertTool(const QIcon &icon, const QString &label, QWidget *toolWidget)
    {
        QToolButton *tabButton = new QToolButton(m_tabBar);

        tabButton->setIconSize( QSize(32, 32) );
        tabButton->setCheckable(true);
        tabButton->setChecked(true);
        tabButton->setIcon(icon);
        tabButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        tabButton->setText(label);

        m_tabBarLayout->insertWidget(0, tabButton);
        m_toolWidgetSplitter->insertWidget(0, toolWidget);

        return tabButton;
    }

    QByteArray TabToolWidget::saveToolsGeometry() const
    {
        return m_toolWidgetSplitter->saveState();
    }

    bool TabToolWidget::restoreToolsGeometry(const QByteArray &geometry)
    {
        return m_toolWidgetSplitter->restoreState(geometry);
    }
}
