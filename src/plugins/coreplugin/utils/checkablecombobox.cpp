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

#include "checkablecombobox.h"

#include <QLineEdit>
#include <QPalette>
#include <QApplication>
#include <QAbstractItemView>

#include <QStandardItemModel>
#include <QMouseEvent>
#include <QVariantList>
#include <QMenu>
#include <QAction>
#include <QPoint>

namespace Lindwurm::Core
{
    CheckableComboBox::CheckableComboBox(QWidget *parent)
        : QComboBox{parent}
    {
        // Make the combo editable to set a custom text, but readonly
        setEditable(true);
        lineEdit()->setReadOnly(true);

        // Make the lineedit the same color as QPushButton
        QPalette palette = qApp->palette();
        palette.setBrush(QPalette::Base, palette.button() );
        lineEdit()->setPalette(palette);

        // Update the text when an item is toggled
        connect(model(), &QAbstractItemModel::dataChanged, this, &CheckableComboBox::updateText);

        // Hide and show popup when clicking the line edit
        lineEdit()->installEventFilter( this );

        // Prevent popup from closing when clicking on an item
        view()->viewport()->installEventFilter( this );

        // prevent pressing enter to emit the itemSelected signal
        view()->installEventFilter( this );

        m_contextMenu = new QMenu(this);
        QAction* selectAllAction = m_contextMenu->addAction("Select all");
        QAction* deselectAllAction = m_contextMenu->addAction("Deselect all");

        // TODO: Shortcut currently not working, maybe some problems with focus?
        //selectAllAction->setShortcut( QKeySequence(tr("Ctrl+A")) );

        connect(selectAllAction,    &QAction::triggered, this, &CheckableComboBox::selectAllIndexes);
        connect(deselectAllAction,  &QAction::triggered, this, &CheckableComboBox::deselectAllIndexes);

        setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, &QComboBox::customContextMenuRequested, this, [=](QPoint pos)
        {
            m_contextMenu->exec( this->mapToGlobal(pos) );
        });

        view()->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(view(), &QComboBox::customContextMenuRequested, this, [=](QPoint pos)
        {
            m_contextMenu->exec( this->mapToGlobal(pos) );
        });

        // see setModel() for explanation
        // setCurrentIndex(-1);
    }

    void CheckableComboBox::updateText()
    {
        QVariant data = currentData(Qt::DisplayRole);

        if ( ! data.toString().isEmpty() )
        {
            // if there is no item with a valid CheckStateRole, data returns only a string - the current index display role
            // this happens when the model does not support CheckStateRole, so as there is only a single index selected
            // we set the text of the current index
            setCurrentText( data.toString() );
            return;
        }

        QVariantList dataList = data.toList();

        if ( dataList.size() == 0 )
        {
            // currently no item is selected
            setCurrentText("");
            emit selectedIndexesChanged(0);
            return;
        }

        if ( dataList.size() == 1 )
        {
            // currently only one item is selected, so we show the item's text
            setCurrentText( dataList.at(0).toString() );
            emit selectedIndexesChanged(1);
            return;
        }

        if ( ! m_showMultipleItems )
        {
            setCurrentText(m_multipleCheckedText);
            emit selectedIndexesChanged(dataList.size());
            return;
        }

        QStringList selectedItemTexts;

        for (const QVariant &selectedItem : qAsConst(dataList) )
        {
            selectedItemTexts.append( selectedItem.toString() );
        }

        QFontMetrics metrics = QFontMetrics( lineEdit()->font() );
        QString elidedText = metrics.elidedText(selectedItemTexts.join(m_separator), Qt::ElideRight, lineEdit()->width() );
        lineEdit()->setText(elidedText);

        emit selectedIndexesChanged(dataList.size());
    }

    void CheckableComboBox::wheelEvent(QWheelEvent* event)
    {
        Q_UNUSED(event)
    }

    void CheckableComboBox::keyPressEvent(QKeyEvent *event)
    {
        if ( event->key() == Qt::Key_Space )
        {
            showPopup();
            return;
        }

        if ( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
        {
            // prevent accidently adding new items (e.g. when current text shows multiple selected items)
            return;
        }

        QComboBox::keyPressEvent(event);
    }

    void CheckableComboBox::resizeEvent(QResizeEvent *event)
    {
        updateText();
        QComboBox::resizeEvent(event);
    }

    void CheckableComboBox::selectAllIndexes()
    {
        model()->blockSignals(true);

            QAbstractItemModel* itemModel = model();
            int rowCount = itemModel->rowCount();

            for (int i = 0; i < rowCount; i++)
            {
                itemModel->setData(itemModel->index(i, 0), Qt::Checked, Qt::CheckStateRole);
            }

        model()->blockSignals(false);

        view()->viewport()->repaint();
        updateText();
    }

    void CheckableComboBox::deselectAllIndexes()
    {
        model()->blockSignals(true);

            QAbstractItemModel* itemModel = model();
            int rowCount = itemModel->rowCount();

            for (int i = 0; i < rowCount; i++)
            {
                itemModel->setData(itemModel->index(i, 0), Qt::Unchecked, Qt::CheckStateRole);
            }

        model()->blockSignals(false);

        view()->viewport()->repaint();
        updateText();
    }

    void CheckableComboBox::addItem(const QString &text, bool checked, const QVariant &userData)
    {
        QStandardItemModel* itemModel = qobject_cast<QStandardItemModel*>( model() );

        if ( ! itemModel )
        {
            // if the used model is not item based we call the base implementation
            // which handles adding the item via the QAbstractItemModel interface
            // handling the Checkable flag and CheckStateRole is up to the concrete model implementation (or simply ignored)
            QComboBox::addItem(text, userData);
            return;
        }

        QStandardItem* item = new QStandardItem();
        item->setText(text);

        if ( userData.isValid() )
        {
            item->setData(userData, Qt::UserRole);
        }
        else
        {
            item->setData(text, Qt::UserRole);
        }

        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

        if ( checked )
        {
            item->setData(Qt::Checked, Qt::CheckStateRole);
        }
        else
        {
            item->setData(Qt::Unchecked, Qt::CheckStateRole);
        }

        itemModel->appendRow(item);
    }

    void CheckableComboBox::addItems(const QStringList &texts)
    {
        foreach (const QString& text, qAsConst(texts) )
        {
            addItem(text);
        }
    }

    void CheckableComboBox::addItems(const QMap<QString, QVariant> &map)
    {
        foreach (const QString& text, map.keys() )
        {
            addItem(text, false, map[text]);
        }
    }

    void CheckableComboBox::setModel(QAbstractItemModel *newModel)
    {
        QAbstractItemModel* oldModel = model();

        if (oldModel)
        {
            disconnect(oldModel, &QAbstractItemModel::dataChanged, this, &CheckableComboBox::updateText);
        }

        connect(newModel, &QAbstractItemModel::dataChanged, this, &CheckableComboBox::updateText);

        QComboBox::setModel(newModel);

        // the dataChanged signal is connected to the QComboBox _q_dataChanged slot which would override the
        // line edit's text and could cause to set it to an item's text altough no item is currently checked

        // to prevent this we could either disconnect the slot (which maybe could have some side effects) or
        // we set an invalid index as current

        // these solutions do not fully work, so we don't use this workaround for now as this only affects custom models

        // here we use an invalid index because current index has no meaningful sense in a checked combo box anyway
        // setCurrentIndex(-1);
        // disconnect(newModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(_q_dataChanged(QModelIndex,QModelIndex)));
    }

    bool CheckableComboBox::eventFilter(QObject *object, QEvent *event)
    {
        if ( object == lineEdit() )
        {
            if ( event->type() == QEvent::MouseButtonRelease )
            {
                showPopup();
                return true;
            }

            return false;
        }

        if ( object == view() )
        {
            if (event->type() == QEvent::ContextMenu)
            {
                QContextMenuEvent* contextEvent = dynamic_cast<QContextMenuEvent*>(event);
                Q_ASSERT_X(contextEvent, "CheckableComboBox", "Failed to cast event to QContextMenuEvent.");

                m_contextMenu->exec( contextEvent->globalPos() );
                return true;
            }

            if ( event->type() == QEvent::ShortcutOverride)
            {
                QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
                Q_ASSERT_X(keyEvent, "CheckableComboBox", "Failed to cast event to QKeyEvent.");

                switch ( keyEvent->key() )
                {
                    case Qt::Key_Enter:
                    case Qt::Key_Return:
                    case Qt::Key_Select:

                        hidePopup();

                        // if the item is not checkable we allow to pass the event, to behave like a normal QComboBox
                        if ( !( model()->flags( view()->currentIndex() ) & Qt::ItemIsUserCheckable ) )
                        {
                            return false;
                        }

                        // we filter this event here to prevent QComboBox from sending the itemSelected signal
                        // which would override the lineEdit() text with the selected item text
                        return true;

                    default:
                        break;
                }
            }
        }

        if ( object == view()->viewport() )
        {
            if ( event->type() == QEvent::MouseButtonRelease )
            {
                QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);

                Q_ASSERT_X(mouseEvent, "CheckableComboBox", "Failed to cast event to QMouseEvent.");

                if ( mouseEvent->button() == Qt::RightButton )
                {
                    m_contextMenu->exec( mouseEvent->globalPos() );
                    return true;
                }

                QAbstractItemModel* model = this->model();

                QModelIndex index = this->view()->indexAt( mouseEvent->pos()  );
                QVariant checkedState =  model->data(index, Qt::CheckStateRole);

                bool setDataSuccess;

                if ( checkedState.toInt() == Qt::Checked )
                {
                    setDataSuccess = model->setData(index, Qt::Unchecked, Qt::CheckStateRole);
                }
                else
                {
                    setDataSuccess = model->setData(index, Qt::Checked, Qt::CheckStateRole);
                }

                // if data was successfully set we want to filter this event (e.g. to avoid closing the popup)
                // if data was not set successfully (e.g. the model does not support the CheckStateRole), we let
                // this event pass so the CheckableComboBox behaves like a regular QComboBox
                return setDataSuccess;
            }
        }

        return false;
    }

    QVariant CheckableComboBox::currentData(int role) const
    {
        QAbstractItemModel* itemModel = model();
        int rowCount = itemModel->rowCount();

        QVariantList selectedItemsData;
        bool noItemWithCheckedStateRole = true;

        for (int i = 0; i < rowCount; i++)
        {
            QVariant checked = itemModel->data( itemModel->index(i, 0), Qt::CheckStateRole);

            if ( checked.isValid() )
            {
                noItemWithCheckedStateRole = false;

                if ( Qt::CheckState(qvariant_cast<int>(checked)) == Qt::Checked )
                {
                    selectedItemsData.append( itemModel->data( itemModel->index(i, 0), role) );
                }
            }
        }

        if ( noItemWithCheckedStateRole )
        {
            // if there was no item with a valid CheckStateRole (e.g. the model does not support the CheckStateRole)
            // we call the base implementation
            return QComboBox::currentData(role);
        }

        return selectedItemsData;
    }

    void CheckableComboBox::setPlaceholderText(const QString &placeholderText)
    {
        m_placeholderText = placeholderText;
        lineEdit()->setPlaceholderText(m_placeholderText);
    }

    QString CheckableComboBox::placeholderText() const
    {
        return m_placeholderText;
    }

    void CheckableComboBox::setMultipleCheckedText(const QString &multiCheckedText)
    {
        m_multipleCheckedText = multiCheckedText;
        if ( ! m_showMultipleItems )
        {
            updateText();
        }
    }

    QString CheckableComboBox::multipleCheckedText() const
    {
        return m_multipleCheckedText;
    }

    void CheckableComboBox::setSeparator(const QString &separator)
    {
        m_separator = separator;
        if ( m_showMultipleItems )
        {
            updateText();
        }
    }

    QString CheckableComboBox::separator() const
    {
        return m_separator;
    }

    bool CheckableComboBox::showMultipleItems() const
    {
        return m_showMultipleItems;
    }

    void CheckableComboBox::setShowMultipleItems(bool showItems)
    {
        m_showMultipleItems = showItems;
        updateText();
    }
}
