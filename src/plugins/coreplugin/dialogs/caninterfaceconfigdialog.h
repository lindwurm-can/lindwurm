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

#ifndef CANINTERFACECONFIGDIALOG_H
#define CANINTERFACECONFIGDIALOG_H

#include <QDialog>
#include <QString>
#include "caninterface/caninterfaceconfig.h"
#include "caninterface/icaninterfacesharedptr.h"

namespace Ui { class CanInterfaceConfigDialog; }

namespace Lindwurm::Lib
{
    class CanInterfaceManager;
}

namespace Lindwurm::Core
{
    /**
     * @brief The CanInterfaceConfigDialog is a dialog for configuring the settings of a CAN interface.
     *
     * The dialog is shown by the CanInterfaceManagerWidget while adding or editing interfaces. It shows
     * a list of available interface types and devices and allows to set the bit rates of the interfaces.
     */
    class CanInterfaceConfigDialog : public QDialog
    {
        Q_OBJECT
        public:

            explicit                        CanInterfaceConfigDialog(Lib::CanInterfaceManager* manager, QWidget *parent = nullptr);
                                            ~CanInterfaceConfigDialog();

            /**
             * @brief Shows the dialog to add a new CAN interface.
             */
            void                            addInterface(void);

            /**
             * @brief Shows the dialog to edit the settings of an already existing interface.
             * @param interfaceId the id of the interface to be edited.
             */
            void                            editInterface(QString interfaceId);

        private slots:

            void                            setupBitRates(void);
            void                            setupBridgeableDevices();
            void                            setupBridgedDevices(const Lib::ICanInterfaceSharedPtr &interface);

            void                            selectedInterfaceTypeChanged(const QString &interfaceType);

            void                            addButtonClicked(void);
            void                            applyButtonClicked(void);

        private:

            Lib::CanInterfaceConfig         getSelectedConfig() const;

        private:

            Ui::CanInterfaceConfigDialog    *ui;
            Lib::CanInterfaceManager*       m_interfaceManager;
            int                             m_interfaceAddCounter = { 1 };
            QString                         m_interfaceIdToEdit = { };
    };
}

#endif // CANINTERFACECONFIGDIALOG_H
