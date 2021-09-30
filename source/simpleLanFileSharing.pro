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
CONFIG += c++11


SOURCES += main.cpp\
    about_gui.cpp \
    file_data.cpp \
        filesharing_gui.cpp \
    network_manager.cpp \
    parse_file_data.cpp \
    parse_network_message.cpp \
    remote_host_file_data.cpp \
    search.cpp \
    server.cpp \
    scan_network.cpp \
    data.cpp \
    client.cpp \
    addfile_gui.cpp \
    setdir_gui.cpp \
    setnetwork_gui.cpp \
    scanip_gui.cpp \
    my_server.cpp \
    shared_files.cpp

HEADERS  += filesharing_gui.h \
    about_gui.h \
    file_data.h \
    message_codes.h \
    network_manager.h \
    parse_file_data.h \
    parse_network_message.h \
    remote_host_file_data.h \
    search.h \
    server.h \
    scan_network.h \
    data.h \
    client.h \
    addfile_gui.h \
    setdir_gui.h \
    setnetwork_gui.h \
    scanip_gui.h \
    my_server.h \
    shared_files.h

FORMS    += filesharing_gui.ui \
    about_gui.ui \
    addfile_gui.ui \
    setdir_gui.ui \
    setnetwork_gui.ui \
    scanip_gui.ui
