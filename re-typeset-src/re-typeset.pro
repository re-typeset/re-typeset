# Re-Typeset program for converting scanned documents to smaller size of page
# without changing text size.
#
# Copyright (C) 2013-2015 Piotr Mika (piotr.mt.mika@gmail.com).
#
# This  program is free software:  you can  redistribute it and/or  modify it
# under the terms of the  GNU General Public License as published by the Free
# Software Foundation,  either version 3 of the License,  or (at your option)
# any later version.
#
# This program is distributed in the hope that it will beuseful,  but WITHOUT
# ANY  WARRANTY;  without  even the implied  warranty  of MERCHANTABILITY  or
# FITNESS FOR  A PARTICULAR  PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the  GNU  General  Public  License along
# with this program.If not, see <http://www.gnu.org/licenses/>.

#-------------------------------------------------
#
# Project created by QtCreator 2013-11-27T11:05:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = re-typeset
TEMPLATE = app

SOURCES  += main.cpp \
    mainwindow.cpp \
    consts.cpp \
    scanneddocument.cpp \
    scannedpage.cpp \
    scannedline.cpp \
    stats.cpp \
    printedpage.cpp \
    printedline.cpp \
    printedpagestat.cpp \
    printedpixelfont.cpp \
    license.cpp \
    processingthread.cpp \
    calibratewindow.cpp \
    variableparameter.cpp \
    calibrateimagewidget.cpp \
    extendedimage.cpp \
    hsvimage.cpp

HEADERS  += version.hpp \
    mainwindow.hpp \
    consts.hpp \
    scanneddocument.hpp \
    scannedpage.hpp \
    scannedline.hpp \
    stats.hpp \
    printedpage.hpp \
    printedline.hpp \
    printedpagestat.hpp \
    printedpixelfont.hpp \
    license.hpp \
    processingthread.hpp \
    calibratewindow.hpp \
    variableparameter.hpp \
    calibrateimagewidget.hpp \
    extendedimage.hpp \
    hsvimage.hpp    

FORMS  += mainwindow.ui \
    calibratewindow.ui

TRANSLATIONS  += lang/re-typeset_pl.ts \
    lang/re-typeset_en.ts

QMAKE_CXXFLAGS += -Wall


CONFIG += staticlib \
    c++11

#QTPLUGIN  += qtiff

OTHER_FILES +=
