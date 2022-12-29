cmake_minimum_required(VERSION 3.14)

set(PROJECT "FileSystemWatcher")

project(${PROJECT} VERSION 1.0 LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
    Core
    Quick
)

find_package(QT${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Core
    Quick
)

add_subdirectory(filesystemwatcherlib)
add_subdirectory(imagedownloader)
add_subdirectory(application)
