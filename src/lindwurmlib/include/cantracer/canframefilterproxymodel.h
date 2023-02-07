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

#ifndef CANFRAMEFILTERPROXYMODEL_H
#define CANFRAMEFILTERPROXYMODEL_H

#include "lindwurmlib_global.h"

#include <QSortFilterProxyModel>

namespace Lindwurm::Lib
{
    /**
     * @brief The CanFrameFilterProxyModel class allows to filter CAN frames from a model by a block or a pass filter.
     */
    class LINDWURMLIB_EXPORT CanFrameFilterProxyModel : public QSortFilterProxyModel
    {
        Q_OBJECT
        public:

            /**
             * @brief Constructs a CanFrameFilterProxyModel with the provided QObject parent.
             * @param parent the QObject parent.
             */
            explicit CanFrameFilterProxyModel(QObject *parent = nullptr);

            /**
             * @brief Sets a regular expression filter to allow only frames that match the pattern.
             * @param pattern the regular expression for the filter.
             */
            void setPassFilterRegularExpression(const QString &pattern);

            /**
             * @brief Sets a regular expression filter to block frames that match the pattern.
             * @param pattern the regular expression for the filter.
             */
            void setBlockFilterRegularExpression(const QString &pattern);

            /**
             * @brief Clears the filter and disabled any filtering.
             */
            void clearFilter();

        protected:

            enum class FilterType
            {
                PassFilter,
                BlockFilter,
                None
            };

            virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;


            FilterType  _filterType = { FilterType::None };
    };
}

#endif // CANFRAMEFILTERPROXYMODEL_H
