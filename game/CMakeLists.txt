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

#SFML lib
find_package(SFML 2.3 REQUIRED system window graphics audio)
if(SFML_FOUND)
    target_include_directories(yapg-game PUBLIC ${SFML_INCLUDE_DIR})
endif()

#Lua
find_package(Lua 5 REQUIRED)
if(LUA_FOUND)
    target_include_directories(yapg-game PUBLIC ${LUA_INCLUDE_DIR})
endif()

#Selene
set(SELENE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/Selene/include")
target_include_directories(yapg-game PUBLIC ${SELENE_INCLUDE_DIR})

#Boost filesystem
find_package(Boost COMPONENTS system filesystem REQUIRED)
target_include_directories(yapg-game PUBLIC ${BOOST_INCLUDE_DIR})

#Linking
target_link_libraries(yapg-game PUBLIC
    ${ENTITYX_STATIC_LIBRARY} ${SFML_LIBRARIES} ${LUA_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
)

#Install the app
install(TARGETS yapg-game DESTINATION "${CMAKE_SOURCE_DIR}/release")

install(DIRECTORY "${CMAKE_SOURCE_DIR}/game/luasrc/" DESTINATION "${CMAKE_SOURCE_DIR}/release/scripts/" PATTERN "*.lua")