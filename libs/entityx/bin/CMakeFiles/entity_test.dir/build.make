# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/victor/Developpement/YAPG/libs/entityx

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/victor/Developpement/YAPG/libs/entityx/bin

# Include any dependencies generated for this target.
include CMakeFiles/entity_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/entity_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/entity_test.dir/flags.make

CMakeFiles/entity_test.dir/entityx/Entity_test.cc.o: CMakeFiles/entity_test.dir/flags.make
CMakeFiles/entity_test.dir/entityx/Entity_test.cc.o: ../entityx/Entity_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victor/Developpement/YAPG/libs/entityx/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/entity_test.dir/entityx/Entity_test.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/entity_test.dir/entityx/Entity_test.cc.o -c /home/victor/Developpement/YAPG/libs/entityx/entityx/Entity_test.cc

CMakeFiles/entity_test.dir/entityx/Entity_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/entity_test.dir/entityx/Entity_test.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victor/Developpement/YAPG/libs/entityx/entityx/Entity_test.cc > CMakeFiles/entity_test.dir/entityx/Entity_test.cc.i

CMakeFiles/entity_test.dir/entityx/Entity_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/entity_test.dir/entityx/Entity_test.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victor/Developpement/YAPG/libs/entityx/entityx/Entity_test.cc -o CMakeFiles/entity_test.dir/entityx/Entity_test.cc.s

CMakeFiles/entity_test.dir/entityx/Entity_test.cc.o.requires:

.PHONY : CMakeFiles/entity_test.dir/entityx/Entity_test.cc.o.requires

CMakeFiles/entity_test.dir/entityx/Entity_test.cc.o.provides: CMakeFiles/entity_test.dir/entityx/Entity_test.cc.o.requires
	$(MAKE) -f CMakeFiles/entity_test.dir/build.make CMakeFiles/entity_test.dir/entityx/Entity_test.cc.o.provides.build
.PHONY : CMakeFiles/entity_test.dir/entityx/Entity_test.cc.o.provides

CMakeFiles/entity_test.dir/entityx/Entity_test.cc.o.provides.build: CMakeFiles/entity_test.dir/entityx/Entity_test.cc.o


# Object files for target entity_test
entity_test_OBJECTS = \
"CMakeFiles/entity_test.dir/entityx/Entity_test.cc.o"

# External object files for target entity_test
entity_test_EXTERNAL_OBJECTS =

entity_test: CMakeFiles/entity_test.dir/entityx/Entity_test.cc.o
entity_test: CMakeFiles/entity_test.dir/build.make
entity_test: libentityx.a
entity_test: CMakeFiles/entity_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/victor/Developpement/YAPG/libs/entityx/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable entity_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/entity_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/entity_test.dir/build: entity_test

.PHONY : CMakeFiles/entity_test.dir/build

CMakeFiles/entity_test.dir/requires: CMakeFiles/entity_test.dir/entityx/Entity_test.cc.o.requires

.PHONY : CMakeFiles/entity_test.dir/requires

CMakeFiles/entity_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/entity_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/entity_test.dir/clean

CMakeFiles/entity_test.dir/depend:
	cd /home/victor/Developpement/YAPG/libs/entityx/bin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/victor/Developpement/YAPG/libs/entityx /home/victor/Developpement/YAPG/libs/entityx /home/victor/Developpement/YAPG/libs/entityx/bin /home/victor/Developpement/YAPG/libs/entityx/bin /home/victor/Developpement/YAPG/libs/entityx/bin/CMakeFiles/entity_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/entity_test.dir/depend

