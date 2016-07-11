cmake_minimum_required(VERSION 3.0)
project(YAPG-game)

message(STATUS ${CMAKE_SYSTEM_NAME})

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(OS_WINDOWS TRUE)
    add_definitions(-DOS_WINDOWS)
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
    add_definitions(-DWIN32)
    add_definitions(-D_WINDOWS)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(OS_LINUX TRUE)
    add_definitions(-DOS_LINUX)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(OS_MACOSX TRUE)
    add_definitions(-DOS_MACOSX)
endif()

#Source code
file(GLOB_RECURSE sources Sources/*.cpp Sources/*.h)

#Executable target
add_executable(yapg-game ${sources})

# just for example add some compiler flags
target_compile_options(yapg-game PUBLIC -std=c++14 -Wall)

# this lets me include files relative to the root src dir with a <> pair
target_include_directories(yapg-game PUBLIC Sources/)

#Entityx library
set(ENTITYX_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/entityx")
set(ENTITYX_STATIC_LIBRARY "${CMAKE_SOURCE_DIR}/libs/entityx/bin/libentityx.a")
target_include_directories(yapg-game PUBLIC ${ENTITYX_INCLUDE_DIR})

#SFML lib
find_package(SFML 2.3 REQUIRED system window graphics audio)
target_include_directories(yapg-game PUBLIC ${SFML_INCLUDE_DIR})

#OpenGL
find_package(OpenGL REQUIRED)
target_include_directories(yapg-game PUBLIC ${OPENGL_INCLUDE_DIRS})

#Lua
find_package(Lua 5 REQUIRED)
target_include_directories(yapg-game PUBLIC ${LUA_INCLUDE_DIR})

#Boost filesystem
find_package(Boost COMPONENTS system filesystem REQUIRED)
target_include_directories(yapg-game PUBLIC ${Boost_INCLUDE_DIR})

if(DEFINED OS_LINUX)
    # GTK3 on Linux (for NativeGui classes)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    target_include_directories(yapg-game PUBLIC ${GTK3_INCLUDE_DIRS})
    link_directories(${GTK3_LIBRARY_DIRS})
    add_definitions(${GTK3_CFLAGS_OTHER})
endif()

#Linking
target_link_libraries(yapg-game PUBLIC
    ${OPENGL_LIBRARIES} ${ENTITYX_STATIC_LIBRARY} ${SFML_LIBRARIES} ${LUA_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${GTK3_LIBRARIES}
)

#Install the app
install(TARGETS yapg-game DESTINATION "${CMAKE_SOURCE_DIR}/release")

install(DIRECTORY "${CMAKE_SOURCE_DIR}/yapg/LuaSources/" DESTINATION "${CMAKE_SOURCE_DIR}/release/scripts/" PATTERN "*.lua")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/yapg/Resources/" DESTINATION "${CMAKE_SOURCE_DIR}/release/")
