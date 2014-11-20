#-------------------------------------------------
#
# Project created by QtCreator 2014-06-08T14:28:22
#
#-------------------------------------------------

QT       += core gui
QT       += network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simpleLanFileSharing
TEMPLATE = app


SOURCES += main.cpp\
        filesharing_gui.cpp \
    search.cpp \
    server.cpp \
    scan_network.cpp \
    data.cpp \
    client.cpp \
    addfile_gui.cpp \
    myserver.cpp \
    setdir_gui.cpp \
    setnetwork_gui.cpp \
    scanip_gui.cpp

HEADERS  += filesharing_gui.h \
    search.h \
    server.h \
    scan_network.h \
    data.h \
    client.h \
    addfile_gui.h \
    myserver.h \
    setdir_gui.h \
    setnetwork_gui.h \
    scanip_gui.h

FORMS    += filesharing_gui.ui \
    addfile_gui.ui \
    setdir_gui.ui \
    setnetwork_gui.ui \
    scanip_gui.ui
