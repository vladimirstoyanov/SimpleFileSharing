cmake_minimum_required(VERSION 3.16)

project(simpleLanFileSharing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

qt_standard_project_setup()


qt_add_executable(simpleLanFileSharing
    main.cpp

    GUI/about_gui.cpp
    GUI/about_gui.h

    GUI/filesharing_gui.cpp
    GUI/filesharing_gui.h

    GUI/addfile_gui.cpp
    GUI/addfile_gui.h

    GUI/setdir_gui.cpp
    GUI/setdir_gui.h

    GUI/setnetwork_gui.cpp
    GUI/setnetwork_gui.h

    GUI/scanip_gui.cpp
    GUI/scanip_gui.h

    client_manager.cpp
    client_manager.h

    client_thread.cpp
    client_thread.h

    file_data.cpp
    file_data.h

    file_operations.cpp
    file_operations.h

    network_manager.cpp
    network_manager.h

    parse_file_data.cpp
    parse_file_data.h

    parse_network_message.cpp
    parse_network_message.h

    protocol_data.cpp
    protocol_data.h

    protocol_messages.cpp
    protocol_messages.h

    remote_host_file_data.cpp
    remote_host_file_data.h

    scan_network_thread.cpp
    scan_network_thread.h

    search.cpp
    search.h

    server.cpp
    server.h

    server_thread.cpp
    server_thread.h

    shared_files.cpp
    shared_files.h

    message_codes.h
)

target_include_directories(simpleLanFileSharing PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(simpleLanFileSharing PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)
