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

#ifndef CANTRACERFILTERBOOKMARKSDIALOG_H
#define CANTRACERFILTERBOOKMARKSDIALOG_H

#include <QDialog>
#include <QPair>
#include <QString>

namespace Ui { class CanTracerFilterBookmarksDialog; }

namespace Lindwurm::Core
{
    using FilterBookmark = QPair<QString,QString>;
    using FilterBookmarkList = QList<FilterBookmark>;

    class CanTracerFilterBookmarksDialog : public QDialog
    {
        Q_OBJECT
        public:

            explicit CanTracerFilterBookmarksDialog(QWidget *parent = nullptr);
            ~CanTracerFilterBookmarksDialog();

            /**
             * @brief Shows the dialog with the provided filter bookmarks to edit them.
             * @param bookmarks to edit
             */
            void                editBookmarks(const FilterBookmarkList& bookmarks);

            /**
             * @brief Returns the current (edited) bookmark list
             * @return bookmark list
             */
            FilterBookmarkList  bookmarks() const;

        private:

            Ui::CanTracerFilterBookmarksDialog *ui;
    };
}

#endif // CANTRACERFILTERBOOKMARKSDIALOG_H
