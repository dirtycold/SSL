cmake_minimum_required(VERSION 3.8)
project(SSL)
set(CMAKE_CXX_STANDARD 14)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(SOURCES
    main.cpp
    SSLWidget.h
    SSLWidget.cpp
    SSLController.h
    SSLController.cpp)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets)
