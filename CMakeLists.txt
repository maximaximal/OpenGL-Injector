cmake_minimum_required(VERSION 3.1)
project(Pigaco-Overlay)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/CMake")

# Find Qt libraries.
find_package(Qt5Widgets)
find_package(Qt5Qml)
find_package(Qt5Quick)
find_package(Qt5WaylandCompositor)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Compatability with Wt and boost
add_definitions(-DQT_NO_KEYWORDS)

# Find Boost libraries.
find_package(Boost COMPONENTS log)

# Find libx11 and xlib.
find_package(X11)

# The overlay subdirectory contains everything needed for the piga-overlay. It contains required
# C++ classes to be built and makes them available as a static library. This library is then
# linked in the piga-qwm binary, which also loads the QRCs from the overlay.
add_subdirectory(overlay)

# Add the qwm subdirectory, which contains a window manager based on Qt and Wayland.
add_subdirectory(qwm)

# Add the xwm subdirectory, which contains a window manager based on basic_wm and using X11.
add_subdirectory(xwm)
