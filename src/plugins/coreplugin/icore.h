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

#ifndef ICORE_H
#define ICORE_H

#include "coreplugin_global.h"

#include <QObject>
#include <QString>
#include <memory>

namespace Lindwurm::Core
{
    class MainWindow;
    using MainWindowUniquePtr   = std::unique_ptr<MainWindow>;

    /**
     * @brief The ICore class maintains all the core parts of Lindwurm.
     *
     * You should never create an instance of this class. The one and only
     * instance is created by the Core plugin and can be accessed via
     * ICore::instance().
     */
    class COREPLUGIN_EXPORT ICore : public QObject
    {
        friend class CorePlugin;

        Q_OBJECT
        public:

            /**
             * @brief Get the ICore instance.
             * @return the ICore instance.
             */
            static ICore*       instance();
                                ~ICore();

            /**
             * @brief Get the current running version of Lindwurm.
             * @return the current version of Lindwurm.
             */
            static QString      version();

        private:

            /**
             * @brief The ICore instance can only be created by the CorePlugin.
             */
            explicit            ICore(QObject *parent = nullptr);

            static ICore*       s_instance;
            MainWindowUniquePtr m_mainWindow;
    };
}

#endif // ICORE_H
