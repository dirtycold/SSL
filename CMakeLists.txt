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

if (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/MacOSXBundleInfo.plist.in)

    # Identify MacOS bundle
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
    set(MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
    set(MACOSX_BUNDLE_COPYRIGHT ${COPYRIGHT})
    set(MACOSX_BUNDLE_GUI_IDENTIFIER ${IDENTIFIER})
    set(MACOSX_BUNDLE_ICON_FILE ${PROJECT_NAME}.icns)

    # cmake: set icon for a mac osx app
    # https://stackoverflow.com/a/24313061
    set(ICON ${CMAKE_CURRENT_SOURCE_DIR}/icon/${PROJECT_NAME}.icns)
    set_source_files_properties(${ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()
