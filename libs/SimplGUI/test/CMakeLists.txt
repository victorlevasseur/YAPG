#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)
project(SimplGUITestProject)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

# Define sources and executable
set(EXECUTABLE_NAME "SimplGUITest")
file(GLOB_RECURSE source *)
file(GLOB_RECURSE source2 ../src/simplgui/*)
include_directories(../include)
add_definitions("-std=c++11")
add_executable(${EXECUTABLE_NAME} ${source} ${source2})

#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
