project(piga-injector C)

set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

add_subdirectory(injectors)

file(GLOB_RECURSE LUA_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/lua/*.lua)

include(GenerateExportHeader)

add_library(piga-injector SHARED ${INJECTORS_SRCS})

target_include_directories(piga-injector PRIVATE ${FONTCONFIG_INCLUDE_DIR})

set_target_properties(piga-injector PROPERTIES LINK_SEARCH_START_STATIC 1)
set_target_properties(piga-injector PROPERTIES LINK_SEARCH_END_STATIC 1)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

# Find Lua
find_package(Lua REQUIRED)

set(CMAKE_EXE_LINKER_FLAGS " -static")
target_link_libraries(piga-injector -static-libgcc -static-libstdc++ m)

generate_export_header(piga-injector)
set_property(TARGET piga-injector PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET piga-injector PROPERTY C_STANDARD_REQUIRED 11)

target_include_directories(piga-injector PRIVATE ${X11_INCLUDE_DIR})

target_include_directories(piga-injector PRIVATE ${PIGA_INCLUDE_DIRS})

add_custom_command(
    TARGET piga-injector
    DEPENDS ${LUA_SCRIPTS}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/lua ${CMAKE_CURRENT_BINARY_DIR}/piga-injector/lua/)

# External Projects. Make compiling as fast as possible using processor count.
include(ProcessorCount)
ProcessorCount(N)

include(ExternalProject)

ExternalProject_Add(project_luajit
    URL https://luajit.org/download/LuaJIT-2.0.4.tar.gz
    URL_HASH MD5=dd9c38307f2223a504cbfb96e477eca0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND make install CFLAGS=-fpic PREFIX=${CMAKE_CURRENT_BINARY_DIR}/luajit -j${N}
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/project_luajit
    INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/luajit
)

ExternalProject_Get_Property(project_luajit install_dir)
add_library(luajit STATIC IMPORTED)
add_dependencies(luajit project_luajit)
set_property(TARGET luajit PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libluajit-5.1.a)
target_link_libraries(piga-injector luajit)
target_include_directories(piga-injector PRIVATE ${install_dir}/include/luajit-2.0)

ExternalProject_Add(project_pixman
    URL ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/pixman-0.34.0.tar.gz
    URL_HASH SHA1=a1b1683c1a55acce9d928fea1ab6ceb79142ddc7
    CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/configure_pixman.sh ${CMAKE_CURRENT_BINARY_DIR}/project_pixman/src/project_pixman/ ${CMAKE_CURRENT_BINARY_DIR}/cairo
    BUILD_COMMAND make -j${N} CFLAGS=-fPIC 
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND make install CFLAGS=-fpic 
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/project_pixman
    INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/cairo
)

ExternalProject_Get_Property(project_pixman install_dir)
add_library(pixman STATIC IMPORTED)
set_property(TARGET pixman PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libpixman-1.a)
target_include_directories(piga-injector PRIVATE ${install_dir}/include/pixman)

ExternalProject_Add(project_cairo
    URL ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/cairo-1.14.8.tar.xz
    URL_HASH SHA1=c6f7b99986f93c9df78653c3e6a3b5043f65145e
    CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/configure_cairo.sh ${CMAKE_CURRENT_BINARY_DIR}/project_cairo/src/project_cairo/ ${CMAKE_CURRENT_BINARY_DIR}/cairo
    BUILD_COMMAND make -j${N} CFLAGS=-fPIC 
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND make install CFLAGS=-fpic 
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/project_cairo
    INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/cairo
)

ExternalProject_Get_Property(project_cairo install_dir)
add_library(cairo STATIC IMPORTED)
set_property(TARGET cairo PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libcairo.a)
target_link_libraries(piga-injector cairo)
target_link_libraries(piga-injector pixman)
target_include_directories(piga-injector PRIVATE ${install_dir}/include)

add_dependencies(project_cairo project_pixman)
add_dependencies(piga-injector project_cairo)
