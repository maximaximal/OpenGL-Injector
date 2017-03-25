cmake_minimum_required(VERSION 3.1)
project(Pigaco-Overlay)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/CMake")

# Find Qt libraries.
find_package(Qt5Widgets)
find_package(Qt5Qml)
find_package(Qt5Quick)
find_package(Qt5Sql)
find_package(Qt5WaylandCompositor)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Compatability with Wt and boost
add_definitions(-DQT_NO_KEYWORDS)

# The overlay subdirectory contains everything needed for the piga-overlay. It contains required
# C++ classes to be built and makes them available as a static library. This library is then
# linked in the piga-qwm binary, which also loads the QRCs from the overlay.
add_subdirectory(overlay)

# Add the qwm subdirectory, which contains the window manager.
add_subdirectory(qwm)
