cmake_minimum_required(VERSION 3.8)
project(SSL)
set(CMAKE_CXX_STANDARD 14)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SRC
    main.cpp
    SSLWidget.h
    SSLWidget.cpp
    SSLController.h
    SSLController.cpp
    SSLPathHelper.h
    SSL.qrc
    icon/SSL.icns)

add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SRC})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets)
