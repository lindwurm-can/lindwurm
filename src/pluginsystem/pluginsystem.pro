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

TEMPLATE = lib

QT      -= gui
DEFINES += PLUGINSYSTEM_LIBRARY

CONFIG  += c++17

DESTDIR  = ../../bin/lib

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    iplugin.cpp \
    pluginmanager.cpp

HEADERS += \
    pluginsystem_global.h \
    iplugin.h \
    pluginmanager.h

DISTFILES += \
    pluginsystem.pri
