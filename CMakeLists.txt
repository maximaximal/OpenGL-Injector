cmake_minimum_required(VERSION 3.1)
project(OpenGL-Injector)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(LUA_USE_LUAJIT "Use LuaJIT instead of default Lua" OFF)
unset(Lua_FOUND CACHE)
unset(LUA_INCLUDE_DIR CACHE)
unset(LUA_LIBRARY CACHE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Compatability with Wt and boost
add_definitions(-DQT_NO_KEYWORDS)

# Find Boost libraries.
find_package(Boost COMPONENTS log)

# Find libx11 and xlib.
find_package(X11)

# GNU default dirs.
include(GNUInstallDirs)

add_subdirectory(injector)
