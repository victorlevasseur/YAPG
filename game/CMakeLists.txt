cmake_minimum_required(VERSION 3.0)
project(YAPG-game)

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

#SimplGUI library
set(SIMPLGUI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/SimplGUI/include")
set(SIMPLGUI_STATIC_LIBRARY "${CMAKE_SOURCE_DIR}/libs/SimplGUI/bin/libsimplgui.a")
target_include_directories(yapg-game PUBLIC ${SIMPLGUI_INCLUDE_DIR})

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

#Linking
target_link_libraries(yapg-game PUBLIC
    ${ENTITYX_STATIC_LIBRARY} ${SIMPLGUI_STATIC_LIBRARY} ${SFML_LIBRARIES} ${SFGUI_LIBRARY} ${LUA_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
)

#Install the app
install(TARGETS yapg-game DESTINATION "${CMAKE_SOURCE_DIR}/release")

install(DIRECTORY "${CMAKE_SOURCE_DIR}/game/luasrc/" DESTINATION "${CMAKE_SOURCE_DIR}/release/scripts/" PATTERN "*.lua")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/game/resources/" DESTINATION "${CMAKE_SOURCE_DIR}/release/")
