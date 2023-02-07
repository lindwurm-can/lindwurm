# www.lindwurm-can.org
# Copyright (C) 2023 Sascha Muenzberg <sascha@lindwurm-can.org>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

TEMPLATE  = lib

QT       += core gui widgets serialbus
DEFINES  += COREPLUGIN_LIBRARY

CONFIG   += c++17
DEFINES  += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../lindwurmplugin.pri)
include(../../lindwurmlib/lindwurmlib.pri)

SOURCES += \
    cantool.cpp \
    cantoolmanager.cpp \
    cantoolwidgets/caninterfacemanagerwidget.cpp \
    cantoolwidgets/cantracerwidget.cpp \
    cantoolwidgets/loggerwidget.cpp \
    coreplugin.cpp \
    dialogs/aboutdialog.cpp \
    dialogs/caninterfaceconfigdialog.cpp \
    dialogs/cantracerfilterbookmarksdialog.cpp \
    dialogs/settingsdialog.cpp \
    icore.cpp \
    ioptionspage.cpp \
    mainwindow.cpp \
    optionspages/generalsettings.cpp \
    optionspages/uisettings.cpp \
    settingswidgets/generalsettingswidget.cpp \
    themes/activetheme.cpp \
    themes/darktheme.cpp \
    themes/lighttheme.cpp \
    utils/checkablecombobox.cpp \
    utils/tabtoolwidget.cpp

HEADERS += \
    cantool.h \
    cantoolmanager.h \
    cantoolwidgets/caninterfacemanagerwidget.h \
    cantoolwidgets/cantracerwidget.h \
    cantoolwidgets/loggerwidget.h \
    coreconstants.h \
    coreplugin_global.h \
    coreplugin.h \
    dialogs/aboutdialog.h \
    dialogs/caninterfaceconfigdialog.h \
    dialogs/cantracerfilterbookmarksdialog.h \
    dialogs/settingsdialog.h \
    icore.h \
    ioptionspage.h \
    mainwindow.h \
    optionspages/generalsettings.h \
    optionspages/uisettings.h \
    settingswidgets/generalsettingswidget.h \
    themes/activetheme.h \
    themes/darktheme.h \
    themes/itheme.h \
    themes/lighttheme.h \
    utils/checkablecombobox.h \
    utils/tabtoolwidget.h

FORMS += \
    cantoolwidgets/caninterfacemanagerwidget.ui \
    cantoolwidgets/cantracerwidget.ui \
    cantoolwidgets/loggerwidget.ui \
    dialogs/aboutdialog.ui \
    dialogs/caninterfaceconfigdialog.ui \
    dialogs/cantracerfilterbookmarksdialog.ui \
    dialogs/settingsdialog.ui \
    mainwindow.ui \
    settingswidgets/generalsettingswidget.ui

RESOURCES += \
    resources/resources.qrc

DISTFILES += \
    coreplugin.pri
