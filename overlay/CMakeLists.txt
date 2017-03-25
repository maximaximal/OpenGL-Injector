cmake_minimum_required(VERSION 3.1)
project(piga-overlay)

set(PIGA_OVERLAY_MAJOR_VERSION 0)
set(PIGA_OVERLAY_MINOR_VERSION 3)
set(PIGA_OVERLAY_PATCH_VERSION 0)
set(PIGA_OVERLAY_TWEAK_VERSION 0)

set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(HDR "${CMAKE_CURRENT_SOURCE_DIR}/include/piga/overlay")

set(OVERLAY_SRCS
    ${SRC}/Applications.cpp
    ${SRC}/OverlayPlugin.cpp
    )
set(OVERLAY_HDRS
    ${HDR}/Applications.hpp
    ${HDR}/OverlayPlugin.hpp
    )

set(OVERLAY_RSC "${CMAKE_CURRENT_SOURCE_DIR}/overlay.qrc")

include_directories(${HDR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Compatability with Wt and boost
add_definitions(-DQT_NO_KEYWORDS)

# Qt Plugin
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)

configure_file(${HDR}/Config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/PigaOverlayConfig.hpp)

add_library(piga-overlay STATIC ${OVERLAY_SRCS} ${OVERLAY_HDRS} ${OVERLAY_RSC})

target_include_directories(piga-overlay PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

qt5_use_modules(piga-overlay Widgets Qml Quick Sql)

set_property(TARGET piga-overlay PROPERTY CXX_STANDARD_REQUIRED 14)
