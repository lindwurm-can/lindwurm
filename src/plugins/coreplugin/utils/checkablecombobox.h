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

#ifndef CHECKABLECOMBOBOX_H
#define CHECKABLECOMBOBOX_H

#include "coreplugin_global.h"

#include <QComboBox>

class QMenu;

namespace Lindwurm::Core
{
    class COREPLUGIN_EXPORT CheckableComboBox : public QComboBox
    {
        Q_OBJECT
        public:

            explicit        CheckableComboBox(QWidget *parent = nullptr);

            void            addItem(const QString &text, bool checked = false, const QVariant &userData = QVariant() );
            void            addItems(const QStringList &texts);
            void            addItems(const QMap<QString,QVariant> &map);
            void            setModel(QAbstractItemModel *newModel);

            QVariant        currentData(int role = Qt::UserRole) const;

            void            setPlaceholderText(const QString &placeholderText);
            QString         placeholderText() const;

            void            setMultipleCheckedText(const QString &multiCheckedText);
            QString         multipleCheckedText() const;

            void            setSeparator(const QString &separator);
            QString         separator() const;

            bool            showMultipleItems() const;
            void            setShowMultipleItems(bool showItems);

            virtual bool    eventFilter(QObject *object, QEvent *event) override;
            virtual void    wheelEvent(QWheelEvent* event) override;
            virtual void    keyPressEvent(QKeyEvent *event) override;
            virtual void    resizeEvent(QResizeEvent *event) override;

        public slots:

            void            selectAllIndexes();
            void            deselectAllIndexes();

        signals:

            void            selectedIndexesChanged(int numberOfSelectedIndexes);

        protected slots:

            void            updateText();

        private:

            QString         m_placeholderText;
            QString         m_multipleCheckedText = {"Multiple items selected"};
            QString         m_separator = {", "};
            bool            m_showMultipleItems = { true };
            QMenu*          m_contextMenu;
    };

}

#endif // CHECKABLECOMBOBOX_H
