cmake_minimum_required(VERSION 3.14)

project(filesystemwatcherlib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(filesystemwatcherlib SHARED
  filesystemwatcherlib_global.h
  fswatcherinterface.h fswatcherinterface.cpp
  fswatcherimpl.h fswatcherimpl.cpp
  components/fsnode.h
  components/fsfile.h components/fsfile.cpp
  components/fsfolder.h components/fsfolder.cpp
  helpers/fshelper.h helpers/fshelper.cpp
  managers/fswatchermanager.h managers/fswatchermanager.cpp
  models/fswatchermodel.h models/fswatchermodel.cpp
  models/fseventwatchermodel.h models/fseventwatchermodel.cpp
  entities/fsevent.h entities/fsevent.cpp
)

target_link_libraries(filesystemwatcherlib PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(filesystemwatcherlib PRIVATE FILESYSTEMWATCHERLIB_LIBRARY)
