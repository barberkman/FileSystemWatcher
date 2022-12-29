cmake_minimum_required(VERSION 3.14)

project(imagedownloader LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)

add_library(imagedownloader SHARED
  imagedownloader_global.h
  imagedownloader.cpp
  imagedownloader.h
  imagedownloaderimpl.h imagedownloaderimpl.cpp
)

target_link_libraries(imagedownloader PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
)

target_compile_definitions(imagedownloader PRIVATE IMAGEDOWNLOADER_LIBRARY)
