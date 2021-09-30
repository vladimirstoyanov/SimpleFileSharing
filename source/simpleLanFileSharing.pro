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
    client_thread.cpp \
    file_data.cpp \
    file_operations.cpp \
        filesharing_gui.cpp \
    network_manager.cpp \
    parse_file_data.cpp \
    parse_network_message.cpp \
    protocol_data.cpp \
    protocol_messages.cpp \
    remote_host_file_data.cpp \
    search.cpp \
    scan_network.cpp \
    addfile_gui.cpp \
    server.cpp \
    server_thread.cpp \
    setdir_gui.cpp \
    setnetwork_gui.cpp \
    scanip_gui.cpp \
    shared_files.cpp

HEADERS  += filesharing_gui.h \
    about_gui.h \
    client_thread.h \
    file_data.h \
    file_operations.h \
    message_codes.h \
    network_manager.h \
    parse_file_data.h \
    parse_network_message.h \
    protocol_data.h \
    protocol_messages.h \
    remote_host_file_data.h \
    search.h \
    scan_network.h \
    addfile_gui.h \
    server.h \
    server_thread.h \
    setdir_gui.h \
    setnetwork_gui.h \
    scanip_gui.h \
    shared_files.h

FORMS    += filesharing_gui.ui \
    about_gui.ui \
    addfile_gui.ui \
    setdir_gui.ui \
    setnetwork_gui.ui \
    scanip_gui.ui
