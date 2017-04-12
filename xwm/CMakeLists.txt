cmake_minimum_required(VERSION 3.1)
project(piga-xwm)

set(PIGA_XWM_MAJOR_VERSION 0)
set(PIGA_XWM_MINOR_VERSION 1)
set(PIGA_XWM_PATCH_VERSION 0)
set(PIGA_XWM_TWEAK_VERSION 0)

set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(HDR "${CMAKE_CURRENT_SOURCE_DIR}/include/piga/xwm")

set(XWM_SRCS
    ${SRC}/main.cpp
    ${SRC}/Util.cpp
    ${SRC}/WindowManager.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/include/easyloggingpp/easylogging++.cc
    )

set(XWM_HDRS
    ${HDR}/Util.hpp
    ${HDR}/WindowManager.hpp
    )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Compatability with Wt and boost
add_definitions(-DQT_NO_KEYWORDS)

configure_file(${HDR}/Config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/PigaXWMConfig.hpp)

add_executable(piga-xwm ${XWM_SRCS} ${XWM_HDRS})

target_include_directories(piga-xwm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(piga-xwm ${BOOST_LOG_LIBRARIES})

set_property(TARGET piga-xwm PROPERTY CXX_STANDARD_REQUIRED 14)

target_link_libraries(piga-xwm ${X11_LIBRARIES})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/xinitrc ${CMAKE_CURRENT_BINARY_DIR}/xinitrc COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run.sh ${CMAKE_CURRENT_BINARY_DIR}/run_in_xephyr.sh COPYONLY)
