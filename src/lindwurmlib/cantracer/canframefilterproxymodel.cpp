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

#include "cantracer/canframefilterproxymodel.h"

namespace Lindwurm::Lib
{
    CanFrameFilterProxyModel::CanFrameFilterProxyModel(QObject *parent)
        : QSortFilterProxyModel(parent)
    {

    }

    void CanFrameFilterProxyModel::setPassFilterRegularExpression(const QString &pattern)
    {
        _filterType = FilterType::PassFilter;
        setFilterRegularExpression(pattern);
    }

    void CanFrameFilterProxyModel::setBlockFilterRegularExpression(const QString &pattern)
    {
        _filterType = FilterType::BlockFilter;
        setFilterRegularExpression(pattern);
    }

    void CanFrameFilterProxyModel::clearFilter()
    {
        _filterType = FilterType::None;
        setFilterRegularExpression("");
    }

    bool CanFrameFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
    {
        bool superResult = QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);

        if ( _filterType == FilterType::BlockFilter )
        {
            superResult = ! superResult;
        }

        return superResult;
    }
}
