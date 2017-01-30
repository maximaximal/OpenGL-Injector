cmake_minimum_required(VERSION 3.1)
project(Pigaco-Overlay)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/CMake")

set(SRC "${PROJECT_SOURCE_DIR}/src")
set(HDR "${PROJECT_SOURCE_DIR}/include")
set(RSC "${PROJECT_SOURCE_DIR}/ressources")

set(SRCS
    ${SRC}/main.cpp
    ${SRC}/Overlay.cpp
    ${SRC}/Notification.cpp
)

set(HDRS
    ${HDR}/overlay/Overlay.hpp
    ${HDR}/overlay/Notification.hpp
)

set(RSCS
    ${RSC}/overlay.qml
    ${RSC}/shared/NotificationItem.qml
    ${RSC}/shared/Card.qml
    ${RSC}/shared/CardDrawer.qml
)

set(FORS

)


include_directories(${HDR})

# Find Qt
find_package(Qt5Widgets)
find_package(Qt5Qml)
find_package(Qt5Quick)
find_package(Qt5DCore)
find_package(Qt5DRender)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Compatability with Wt and boost
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

add_executable(pigaoverlay ${SRCS} ${HDRS} ${RSCS} ${FORS})

qt5_use_modules(pigaoverlay Widgets Qml Quick 3DCore 3DRender 3DExtras 3DInput)

set_property(TARGET pigaoverlay PROPERTY CXX_STANDARD_REQUIRED 14)

# Installation
install(TARGETS pigaoverlay
    EXPORT PigaOverlayConfig
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION /usr/lib/piga/apps/pigaoverlay/)
install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY ressources/ DESTINATION /usr/lib/piga/apps/pigaoverlay/)
install(EXPORT PigaOverlayConfig DESTINATION lib/cmake)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/app_config.cfg DESTINATION /usr/lib/piga/apps/pigaoverlay/)
