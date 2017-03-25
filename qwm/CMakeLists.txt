cmake_minimum_required(VERSION 3.1)
project(piga-qwm)

set(PIGA_QWM_MAJOR_VERSION 0)
set(PIGA_QWM_MINOR_VERSION 3)
set(PIGA_QWM_PATCH_VERSION 0)
set(PIGA_QWM_TWEAK_VERSION 0)

set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(HDR "${CMAKE_CURRENT_SOURCE_DIR}/include/piga/qwm")

set(QWM_SRCS
    ${SRC}/main.cpp
    )

set(QWM_RSC "${CMAKE_CURRENT_SOURCE_DIR}/qwm.qrc")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Compatability with Wt and boost
add_definitions(-DQT_NO_KEYWORDS)

configure_file(${HDR}/Config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/PigaQWMConfig.hpp)

add_executable(piga-qwm ${QWM_SRCS} ${QWM_RSC})

target_include_directories(piga-qwm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

qt5_use_modules(piga-qwm WaylandCompositor Widgets Qml Quick Sql)

set_property(TARGET piga-qwm PROPERTY CXX_STANDARD_REQUIRED 14)

target_link_libraries(piga-qwm piga-overlay)
