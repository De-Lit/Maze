cmake_minimum_required(VERSION 3.5)

project(Maze VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGL)

include_directories("${PROJECT_SOURCE_DIR}/view")
include_directories("${PROJECT_SOURCE_DIR}/controller")
include_directories("${PROJECT_SOURCE_DIR}/model")

set(PROJECT_SOURCES
    main.cc

    controller/controller.h

    view/mainwindow.ui
    view/mainwindow.cc
    view/mainwindow.h

    model/maze.h
    model/maze.cc
    model/cave.h
    model/cave.cc

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Maze
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${C_SOURCES}
        ${MATRIX_SOURCES}
    )

# Define target properties for Android with Qt 6 as:
# set_property(TARGET Maze APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Maze SHARED
            ${PROJECT_SOURCES}
            ${C_SOURCES}
            ${MATRIX_SOURCES}
        )

    # Define properties for Android with Qt 5 after find_package() calls as:
    # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Maze
            ${PROJECT_SOURCES}
            ${C_SOURCES}
            ${MATRIX_SOURCES}
        )
    endif()
endif()

target_link_libraries(Maze PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Maze PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
target_link_libraries(Maze PRIVATE Qt${QT_VERSION_MAJOR}::OpenGL)

set_target_properties(Maze PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Maze
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Maze)
endif()
