# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dregos/uni/grapevine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dregos/uni/grapevine/build

# Utility rule file for NightlyMemCheck.

# Include any custom commands dependencies for this target.
include libs/glm/CMakeFiles/NightlyMemCheck.dir/compiler_depend.make

# Include the progress variables for this target.
include libs/glm/CMakeFiles/NightlyMemCheck.dir/progress.make

libs/glm/CMakeFiles/NightlyMemCheck:
	cd /home/dregos/uni/grapevine/build/libs/glm && /usr/bin/ctest -D NightlyMemCheck

NightlyMemCheck: libs/glm/CMakeFiles/NightlyMemCheck
NightlyMemCheck: libs/glm/CMakeFiles/NightlyMemCheck.dir/build.make
.PHONY : NightlyMemCheck

# Rule to build all files generated by this target.
libs/glm/CMakeFiles/NightlyMemCheck.dir/build: NightlyMemCheck
.PHONY : libs/glm/CMakeFiles/NightlyMemCheck.dir/build

libs/glm/CMakeFiles/NightlyMemCheck.dir/clean:
	cd /home/dregos/uni/grapevine/build/libs/glm && $(CMAKE_COMMAND) -P CMakeFiles/NightlyMemCheck.dir/cmake_clean.cmake
.PHONY : libs/glm/CMakeFiles/NightlyMemCheck.dir/clean

libs/glm/CMakeFiles/NightlyMemCheck.dir/depend:
	cd /home/dregos/uni/grapevine/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dregos/uni/grapevine /home/dregos/uni/grapevine/libs/sol2 /home/dregos/uni/grapevine/build /home/dregos/uni/grapevine/build/libs/glm /home/dregos/uni/grapevine/build/libs/glm/CMakeFiles/NightlyMemCheck.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : libs/glm/CMakeFiles/NightlyMemCheck.dir/depend

