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

QT += widgets \
    serialbus

TEMPLATE = lib
DEFINES += LINDWURMLIB_LIBRARY

CONFIG += c++17
CONFIG += console

DESTDIR = ../../bin/lib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include/

SOURCES += \
    cancomposer/canframecomposer.cpp \
    cancomposer/canframeenumerator.cpp \
    cancomposer/canframecomposit.cpp \
    caninterface/abstractcaninterface.cpp \
    caninterface/canbridge.cpp \
    caninterface/candevice.cpp \
    caninterface/caninterfaceinfo.cpp \
    caninterface/caninterfacelistmodel.cpp \
    caninterface/caninterfacemanagermodel.cpp \
    caninterface/icaninterfacemanager.cpp \
    caninterface/caninterfacehandle.cpp \
    caninterface/caninterfacemanager.cpp \
    cantracer/abstractcanframetracermodel.cpp \
    cantracer/canframeaggregator.cpp \
    cantracer/canframetracerrecord.cpp \
    cantracer/canframetracer.cpp \
    cantracer/linearcanframetracermodel.cpp \
    cantracer/aggregatedcanframetracermodel.cpp \
    cantracer/canframefilterproxymodel.cpp \
    diagnostic/udsecudiscoveryscanner.cpp \
    diagnostic/udsecudiscoveryscanworker.cpp \
    diagnostic/udsdataid.cpp \
    cantransport/isotransportprotocolframe.cpp \
    cantransport/isotransportprotocol.cpp \
    diagnostic/readdatabyidentifiermapper.cpp \
    utils/bytearrayenumerator.cpp

HEADERS += \
    lindwurmlib_global.h \
    diagnostic/udsecudiscoveryscanworker.h \
    include/cancomposer/canframecomposer.h \
    include/cancomposer/canframecomposit.h \
    include/cancomposer/canframeenumerator.h \
    caninterface/caninterfacelistmodel.h \
    include/cantracer/abstractcanframetracermodel.h \
    include/caninterface/abstractcaninterface.h \
    include/caninterface/canbridge.h \
    include/caninterface/candevice.h \
    include/caninterface/caninterfaceconfig.h \
    include/caninterface/caninterfaceinfo.h \
    include/caninterface/icaninterface.h \
    include/caninterface/caninterfacehandle.h \
    include/caninterface/icaninterfacehandle.h \
    include/caninterface/icaninterfacehandlesharedptr.h \
    include/caninterface/icaninterfacemanager.h \
    include/caninterface/caninterfacemanager.h \
    include/caninterface/icaninterfacesharedptr.h \
    include/caninterface/caninterfacemanagermodel.h \
    cantracer/canframeaggregator.h \
    cantracer/canframetracerrecord.h \
    include/cantracer/canframetracer.h \
    include/cantracer/linearcanframetracermodel.h \
    include/cantracer/aggregatedcanframetracermodel.h \
    include/cantracer/canframefilterproxymodel.h \
    include/diagnostic/readdatabyidentifiermapper.h \
    include/diagnostic/udsdataid.h \
    include/cantransport/isotransportprotocol.h \
    include/cantransport/isotransportprotocolframe.h \
    include/diagnostic/udsecudiscoveryscanner.h \
    include/utils/bytearrayenumerator.h \
    include/utils/range.h \
    include/utils/rangeenumerator.h

DISTFILES += \
    lindwurmlib.pri
