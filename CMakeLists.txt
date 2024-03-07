cmake_minimum_required (VERSION 3.20 FATAL_ERROR)

project(QtTerminal CXX)

# C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Qt
if(NOT QT_VERSION_MAJOR)
  set(QT_VERSION_MAJOR 5 CACHE STRING "Qt version to use (5 or 6), defaults to 5")
endif()
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Targets
add_subdirectory(src)
