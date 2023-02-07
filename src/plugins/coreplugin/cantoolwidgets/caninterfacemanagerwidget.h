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

#ifndef CANINTERFACEMANAGERWIDGET_H
#define CANINTERFACEMANAGERWIDGET_H

#include <QWidget>

namespace Ui { class CanInterfaceManagerWidget; }

namespace Lindwurm::Lib
{
    class CanInterfaceManager;
    class CanInterfaceManagerModel;
}

class QAction;
class QAbstractItemModel;

namespace Lindwurm::Core
{
    class CanInterfaceConfigDialog;

    /**
     * @brief The CanInterfaceManagerWidget class provides a widget for adding, editing and removing CAN interfaces.
     *
     * It provides the central controls to show the current available CAN interfaces and to add, edit or remove them.
     */
    class CanInterfaceManagerWidget : public QWidget
    {
        Q_OBJECT
        public:

            /**
             * @brief Constructs a CanInterfaceManagerWidget with the provided QObject parent.
             * @param parent    the QObject parent of the widget.
             */
            explicit                        CanInterfaceManagerWidget(QWidget *parent = nullptr);
                                            ~CanInterfaceManagerWidget();

        private:

            void                            setupToolBar();
            void                            addAvailableSocketCANInterfaces();
            void                            setInterfaceModel(QAbstractItemModel* model);

        private slots:

            void                            removeSelectedInterface();
            void                            editInteface(const QModelIndex &index);
            void                            editSelectedInterface();

        private:

            Ui::CanInterfaceManagerWidget*  ui;
            Lib::CanInterfaceManager*       m_interfaceManager = { nullptr };
            Lib::CanInterfaceManagerModel*  m_interfaceManagerModel = { nullptr };
            QAction*                        m_addInterfaceAction = { nullptr };
            QAction*                        m_removeInterfaceAction = { nullptr };
            QAction*                        m_editInterfaceAction = { nullptr };
            CanInterfaceConfigDialog*       m_interfaceConfigDialog = { nullptr };
    };
}

#endif // CANINTERFACEMANAGERWIDGET_H
