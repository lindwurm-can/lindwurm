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

#include "cantracerfilterbookmarksdialog.h"
#include "ui_cantracerfilterbookmarksdialog.h"
#include "themes/activetheme.h"

namespace
{
    const char*     NEW_FILTER_NAME = "New filter";
    const char*     NEW_FILTER_VALUE = "123, 456";
}

namespace Lindwurm::Core
{
    CanTracerFilterBookmarksDialog::CanTracerFilterBookmarksDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::CanTracerFilterBookmarksDialog)
    {
        ui->setupUi(this);

        ui->bookmarksView->setHeaderLabels( QStringList( {"Filter name", "Filter expression"}) );
        ui->bookmarksView->setColumnWidth(0, 350);

        connect( ui->toolBar->addAction( ActiveTheme::icon("tool-interfaces/add"), "Add filter"), &QAction::triggered, this, [this]
        {
            QTreeWidgetItem*    item = new QTreeWidgetItem( QStringList({NEW_FILTER_NAME, NEW_FILTER_VALUE}) );
            item->setFlags(item->flags() | Qt::ItemIsEditable);

            ui->bookmarksView->addTopLevelItem(item);

            ui->bookmarksView->setCurrentItem(item);
            ui->bookmarksView->editItem(item, 0);
        });

        connect( ui->toolBar->addAction( ActiveTheme::icon("tool-interfaces/remove"), "Delete filter"), &QAction::triggered, this, [this]
        {
            delete ( ui->bookmarksView->takeTopLevelItem( ui->bookmarksView->currentIndex().row() ) );
        });
    }

    CanTracerFilterBookmarksDialog::~CanTracerFilterBookmarksDialog()
    {
        delete ui;
    }

    void CanTracerFilterBookmarksDialog::editBookmarks(const FilterBookmarkList &bookmarks)
    {
        ui->bookmarksView->clear();

        int numberOfBookmarks = bookmarks.size();

        for (int i = 0; i < numberOfBookmarks; i++)
        {
            const FilterBookmark &bookmark = bookmarks.at(i);
            QTreeWidgetItem*    item = new QTreeWidgetItem( QStringList({bookmark.first, bookmark.second}) );
            item->setFlags(item->flags() | Qt::ItemIsEditable);

            ui->bookmarksView->addTopLevelItem(item);
        }

        show();
    }

    FilterBookmarkList CanTracerFilterBookmarksDialog::bookmarks() const
    {
        FilterBookmarkList  bookmarks;

        int bookmarksCount = ui->bookmarksView->topLevelItemCount();

        for (int i = 0; i < bookmarksCount; i++)
        {
            QString bookmarkName = ui->bookmarksView->topLevelItem(i)->data(0, Qt::DisplayRole).toString();
            QString bookmarkFilter = ui->bookmarksView->topLevelItem(i)->data(1, Qt::DisplayRole).toString();

            bookmarks.append( FilterBookmark(bookmarkName, bookmarkFilter) );
        }

        return bookmarks;
    }
}
