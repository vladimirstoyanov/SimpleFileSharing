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
    GUI/about_gui.cpp \
    client_thread.cpp \
    file_data.cpp \
    file_operations.cpp \
    GUI/filesharing_gui.cpp \
    network_manager.cpp \
    parse_file_data.cpp \
    parse_network_message.cpp \
    protocol_data.cpp \
    protocol_messages.cpp \
    remote_host_file_data.cpp \
    scan_network_thread.cpp \
    search.cpp \
    GUI/addfile_gui.cpp \
    server.cpp \
    server_thread.cpp \
    GUI/setdir_gui.cpp \
    GUI/setnetwork_gui.cpp \
    GUI/scanip_gui.cpp \
    shared_files.cpp

HEADERS  += GUI/filesharing_gui.h \
    GUI/about_gui.h \
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
    scan_network_thread.h \
    search.h \
    GUI/addfile_gui.h \
    server.h \
    server_thread.h \
    GUI/setdir_gui.h \
    GUI/setnetwork_gui.h \
    GUI/scanip_gui.h \
    shared_files.h

FORMS    += GUI/filesharing_gui.ui \
    GUI/about_gui.ui \
    GUI/addfile_gui.ui \
    GUI/setdir_gui.ui \
    GUI/setnetwork_gui.ui \
    GUI/scanip_gui.ui
