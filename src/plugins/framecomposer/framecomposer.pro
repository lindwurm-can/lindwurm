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
DEFINES  += FRAMECOMPOSER_LIBRARY

CONFIG   += c++17

# TODO: QLayout::setMargin is deprecated, replace it and activate disablement again
#DEFINES  += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../lindwurmplugin.pri)
include(../coreplugin/coreplugin.pri)
include(../../lindwurmlib/lindwurmlib.pri)

SOURCES += \
    canframecomposertab.cpp \
    canframecomposerwidget.cpp \
    framecomposerplugin.cpp

HEADERS += \
    canframecomposertab.h \
    canframecomposerwidget.h \
    framecomposer_global.h \
    framecomposerplugin.h

FORMS += \
    canframecomposertab.ui \
    canframecomposerwidget.ui
