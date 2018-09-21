cmake_minimum_required(VERSION 3.10)
project(SuperBrowser)

# 一般当前目录设置为直接包含目录

SET(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR on)
set(CMAKE_AUTOMOC on)
set(CMAKE_AUTOUIC on)
set(CMAKE_AUTORCC on)
set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES
        main.cpp
        encoding.cpp
        memorycookiejar.cpp
        receivethread.cpp
        superbrowser.cpp
        terminal.cpp
        webpage.cpp)
# 添加qt界面基本模块

find_package(Qt5Network)
find_package(Qt5WebKitWidgets)

add_executable(SuperBrowser ${SOURCE_FILES})

target_link_libraries (SuperBrowser Qt5::Network Qt5::WebKitWidgets)