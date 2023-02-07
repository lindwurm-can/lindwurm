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

#include "darktheme.h"
#include <QFile>
#include <QApplication>
#include <QStyle>
#include <QIconEngine>
#include <QString>
#include <QPainter>

namespace
{
    class DarkThemeIconEngine : public QIconEngine
    {
        public:

            DarkThemeIconEngine(const QString &fileName) : m_fileName(fileName)
            {

            }

            void paint(QPainter* painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override
            {
                Q_UNUSED(state)

                if (mode == QIcon::Disabled)
                {
                    QColor gray(128, 128, 128);

                    painter->fillRect( rect, gray.darker(120) );
                }
                else
                {
                    painter->fillRect( rect, Qt::white );
                }
            }

            virtual QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) override
            {
                QPixmap pixmap = QIcon(m_fileName).pixmap( size );

                QPainter painter;

                painter.begin( &pixmap );
                painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                paint(&painter, pixmap.rect(), mode, state);
                painter.end();

                return pixmap;
            }

            QIconEngine* clone() const override
            {
                return new DarkThemeIconEngine(m_fileName);
            }

        private:

            QString m_fileName;
    };
}

namespace Lindwurm::Core
{
    DarkTheme::DarkTheme()
    {
        setActiveTheme(this);
    }

    QString DarkTheme::themeName() const
    {
        return "DarkTheme";
    }

    QString DarkTheme::themeStylesheet() const
    {
        QFile file(":/stylesheet-dark.css");

        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String( file.readAll() );
        file.close();

        return styleSheet;
    }

    QIcon DarkTheme::themeIcon(const QString &iconName) const
    {
        if ( iconName == "tool-logger/bug" )
        {
            // Workaround: QTreeWidgetItem in LoggerWidget seems not working correctly with custom icon engine
            // we select a pre-prepared dark theme icon for this case here
            return QIcon(":/images/icons/tool-logger/bug-dark.svg");
        }

        if ( iconName == "tool-tracer/clear-filter-active" )
        {
            return QIcon(":/images/icons/tool-tracer/clear-filter.svg");
        }

        if ( iconName == "tool-tracer/apply-filter-active" )
        {
            return QIcon(":/images/icons/tool-tracer/apply-filter.svg");
        }

        QIconEngine* iconEngine = new DarkThemeIconEngine(":/images/icons/" + iconName + ".svg");

        return QIcon(iconEngine);
    }

    QPalette DarkTheme::themePalette() const
    {
        QColor darkGray(0x2E, 0x2F, 0x30);
        QColor gray(128, 128, 128);
        QColor lightGray = QColor(0x38, 0x3a, 0x3a).darker(110);
        QColor blue(0x34, 0x67, 0x92);

        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, darkGray);           // QComboBox Drop Down background, affects also the border of QComboBox drop down or QMenus
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, lightGray);
        darkPalette.setColor(QPalette::AlternateBase, darkGray);
        darkPalette.setColor(QPalette::ToolTipBase, blue);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);

        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, darkGray);           // QAction Checked, Background QComboBox, ListView Headers
        darkPalette.setColor(QPalette::ButtonText, Qt::white);      // QMenuBar Text

        darkPalette.setColor(QPalette::Link, blue);
        darkPalette.setColor(QPalette::Highlight, blue);            // Selected widget, but also selected combobox/listview item
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);

        darkPalette.setColor(QPalette::Active, QPalette::Button, lightGray );


        darkPalette.setColor(QPalette::Disabled, QPalette::Base, darkGray);

        darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, gray.darker(120) );   // QComboBox Drop Down Arrow
        darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, gray);
        darkPalette.setColor(QPalette::Disabled, QPalette::Text, gray.darker(120) );         // Disabled Drop Down text
        darkPalette.setColor(QPalette::Disabled, QPalette::Light, darkGray);

        return darkPalette;
    }
}
