cmake_minimum_required(VERSION 3.10)

project(SIGBUILD LANGUAGES CXX VERSION 0.4.3)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)

find_package(QtCreator REQUIRED COMPONENTS Core)
find_package(QtCreator REQUIRED COMPONENTS ProjectExplorer)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)

# Add a CMake option that enables building your plugin with tests.
# You don't want your released plugin binaries to contain tests,
# so make that default to 'NO'.
# Enable tests by passing -DWITH_TESTS=ON to CMake.
option(WITH_TESTS "Builds with tests" NO)

if(WITH_TESTS)
  # Look for QtTest
  find_package(Qt6 REQUIRED COMPONENTS Test)

  # Tell CMake functions like add_qtc_plugin about the QtTest component.
  set(IMPLICIT_DEPENDS Qt::Test)

  # Enable ctest for auto tests.
  enable_testing()
endif()

add_qtc_plugin(SIGBUILD
  PLUGIN_DEPENDS
    QtCreator::Core
    QtCreator::ProjectExplorer
  DEPENDS
    Qt6::Multimedia
    Qt6::Widgets
  SOURCES
    res/resources.qrc
    src/BuildData.h
    src/BuildData.cpp
    src/DialogLastBuild.h
    src/DialogLastBuild.cpp
    src/DialogSessionBuilds.h
    src/DialogSessionBuilds.cpp
    src/OptionsPageMain.h
    src/OptionsPageMain.cpp
    src/OptionsPageMainWidget.h
    src/OptionsPageMainWidget.cpp
    src/Settings.h
    src/Settings.cpp
    src/SigbuildDefines.h
    src/SigbuildPlugin.h
    src/SigbuildPlugin.cpp
)
