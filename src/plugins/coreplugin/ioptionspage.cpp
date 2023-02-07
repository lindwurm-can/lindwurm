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

#include "ioptionspage.h"

namespace Lindwurm::Core
{
    IOptionsPage::IOptionsPage()
    {
    }

    IOptionsPage::~IOptionsPage()
    {
    }

    void IOptionsPage::setWidgetCreator(const WidgetCreator &widgetCreator)
    {
        m_widgetCreator = widgetCreator;
    }

    bool IOptionsPage::apply()
    {
        if ( m_optionsWidget )
        {
            return m_optionsWidget->apply();
        }

        return false;
    }

    QWidget *IOptionsPage::widget()
    {
        Q_ASSERT_X(m_widgetCreator, "IOptionsPage", (QString("Options page '") + m_caption + QString("' did not provide a WidgetCreator!")).toLatin1() );

        if ( ! m_optionsWidget )
        {
            m_optionsWidget = m_widgetCreator();
        }

        return m_optionsWidget;
    }
}
