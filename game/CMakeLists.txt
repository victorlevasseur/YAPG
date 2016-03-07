cmake_minimum_required(VERSION 3.0)
project(YAPG-game)

message(STATUS ${CMAKE_SYSTEM_NAME})

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(OS_WINDOWS TRUE)
    add_definitions(-DOS_WINDOWS)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(OS_LINUX TRUE)
    add_definitions(-DOS_LINUX)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(OS_MACOSX TRUE)
    add_definitions(-DOS_MACOSX)
endif()

#Source code
file(GLOB_RECURSE sources src/*.cpp include/*.h)

#Executable target
add_executable(yapg-game ${sources})

# just for example add some compiler flags
target_compile_options(yapg-game PUBLIC -std=c++14 -Wall)

# this lets me include files relative to the root src dir with a <> pair
target_include_directories(yapg-game PUBLIC include/)

#Entityx library
set(ENTITYX_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/entityx")
set(ENTITYX_STATIC_LIBRARY "${CMAKE_SOURCE_DIR}/libs/entityx/bin/libentityx.a")
target_include_directories(yapg-game PUBLIC ${ENTITYX_INCLUDE_DIR})

#SFML lib
find_package(SFML 2.3 REQUIRED system window graphics audio)
if(SFML_FOUND)
    target_include_directories(yapg-game PUBLIC ${SFML_INCLUDE_DIR})
endif()

#SFGUI lib
find_package(SFGUI 0.3 REQUIRED)
if(SFGUI_FOUND)
    target_include_directories(yapg-game PUBLIC ${SFGUI_INCLUDE_DIR})
endif()

#Lua
find_package(Lua 5 REQUIRED)
if(LUA_FOUND)
    target_include_directories(yapg-game PUBLIC ${LUA_INCLUDE_DIR})
endif()

#Boost filesystem
find_package(Boost COMPONENTS system filesystem REQUIRED)
if(Boost_FOUND)
    target_include_directories(yapg-game PUBLIC ${Boost_INCLUDE_DIR})
endif()

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
    ${ENTITYX_STATIC_LIBRARY} ${SFML_LIBRARIES} ${SFGUI_LIBRARY} ${LUA_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${GTK3_LIBRARIES}
)

#Install the app
install(TARGETS yapg-game DESTINATION "${CMAKE_SOURCE_DIR}/release")

install(DIRECTORY "${CMAKE_SOURCE_DIR}/game/luasrc/" DESTINATION "${CMAKE_SOURCE_DIR}/release/scripts/" PATTERN "*.lua")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/game/resources/" DESTINATION "${CMAKE_SOURCE_DIR}/release/")
