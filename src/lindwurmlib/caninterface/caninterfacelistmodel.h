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

#ifndef CANINTERFACELISTMODEL_H
#define CANINTERFACELISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QString>
#include <tuple>

#include "caninterface/caninterfaceinfo.h"

namespace Lindwurm::Lib
{
    class CanInterfaceManager;

    /**
     * @brief The CanInterfaceListModel class provides a data model for the available CAN interfaces.
     *
     * The CanInterfaceListModel class provides a read-only list model of the CAN interfaces that are
     * currently available by the CanInterfaceManager. It is commonly used by CAN tools to
     * provide a selection of the CAN interface to be used by the tool.
     *
     * Generally, the model is not intended to be created by the user. Instead use
     * ICanInterfaceManager::interfaceListModel() to get the common shared model.
     */
    class CanInterfaceListModel : public QAbstractListModel
    {
        Q_OBJECT
        public:

            CanInterfaceListModel(CanInterfaceManager *interfaceManager, QObject *parent = nullptr);

            virtual int         rowCount(const QModelIndex &parent) const override;
            virtual QVariant    data(const QModelIndex &index, int role) const override;

        private slots:

            void    interfaceAdded(     const Lindwurm::Lib::CanInterfaceInfo  &interfaceInfo);
            void    interfaceModified(  const Lindwurm::Lib::CanInterfaceInfo  &interfaceInfo);
            void    interfaceRemoved(   const QString           &interfaceId);

        private:

            /**
             * @brief m_interfaceList stores the available interfaces with their name and id as a tupel.
             */
            QList< std::tuple<QString, QString> >  m_interfaceList;
    };
}

#endif // CANINTERFACELISTMODEL_H
