# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/dkruger/git/ru/ECE560-Graphics/src/gen2test/common

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dkruger/git/ru/ECE560-Graphics/src/gen2test/common/build

# Include any dependencies generated for this target.
include CMakeFiles/opengl_core.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/opengl_core.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/opengl_core.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/opengl_core.dir/flags.make

CMakeFiles/opengl_core.dir/opengl_core.cpp.o: CMakeFiles/opengl_core.dir/flags.make
CMakeFiles/opengl_core.dir/opengl_core.cpp.o: ../opengl_core.cpp
CMakeFiles/opengl_core.dir/opengl_core.cpp.o: CMakeFiles/opengl_core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dkruger/git/ru/ECE560-Graphics/src/gen2test/common/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/opengl_core.dir/opengl_core.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/opengl_core.dir/opengl_core.cpp.o -MF CMakeFiles/opengl_core.dir/opengl_core.cpp.o.d -o CMakeFiles/opengl_core.dir/opengl_core.cpp.o -c /home/dkruger/git/ru/ECE560-Graphics/src/gen2test/common/opengl_core.cpp

CMakeFiles/opengl_core.dir/opengl_core.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opengl_core.dir/opengl_core.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dkruger/git/ru/ECE560-Graphics/src/gen2test/common/opengl_core.cpp > CMakeFiles/opengl_core.dir/opengl_core.cpp.i

CMakeFiles/opengl_core.dir/opengl_core.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opengl_core.dir/opengl_core.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dkruger/git/ru/ECE560-Graphics/src/gen2test/common/opengl_core.cpp -o CMakeFiles/opengl_core.dir/opengl_core.cpp.s

# Object files for target opengl_core
opengl_core_OBJECTS = \
"CMakeFiles/opengl_core.dir/opengl_core.cpp.o"

# External object files for target opengl_core
opengl_core_EXTERNAL_OBJECTS =

libopengl_core.a: CMakeFiles/opengl_core.dir/opengl_core.cpp.o
libopengl_core.a: CMakeFiles/opengl_core.dir/build.make
libopengl_core.a: CMakeFiles/opengl_core.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dkruger/git/ru/ECE560-Graphics/src/gen2test/common/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libopengl_core.a"
	$(CMAKE_COMMAND) -P CMakeFiles/opengl_core.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/opengl_core.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/opengl_core.dir/build: libopengl_core.a
.PHONY : CMakeFiles/opengl_core.dir/build

CMakeFiles/opengl_core.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/opengl_core.dir/cmake_clean.cmake
.PHONY : CMakeFiles/opengl_core.dir/clean

CMakeFiles/opengl_core.dir/depend:
	cd /home/dkruger/git/ru/ECE560-Graphics/src/gen2test/common/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dkruger/git/ru/ECE560-Graphics/src/gen2test/common /home/dkruger/git/ru/ECE560-Graphics/src/gen2test/common /home/dkruger/git/ru/ECE560-Graphics/src/gen2test/common/build /home/dkruger/git/ru/ECE560-Graphics/src/gen2test/common/build /home/dkruger/git/ru/ECE560-Graphics/src/gen2test/common/build/CMakeFiles/opengl_core.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/opengl_core.dir/depend

