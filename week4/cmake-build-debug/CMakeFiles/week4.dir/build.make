# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /home/sean/application/clion-2017.3.1/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/sean/application/clion-2017.3.1/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sean/course/CS61C-2015/week4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sean/course/CS61C-2015/week4/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/week4.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/week4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/week4.dir/flags.make

CMakeFiles/week4.dir/hw1/beargit.c.o: CMakeFiles/week4.dir/flags.make
CMakeFiles/week4.dir/hw1/beargit.c.o: ../hw1/beargit.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sean/course/CS61C-2015/week4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/week4.dir/hw1/beargit.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/week4.dir/hw1/beargit.c.o   -c /home/sean/course/CS61C-2015/week4/hw1/beargit.c

CMakeFiles/week4.dir/hw1/beargit.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/week4.dir/hw1/beargit.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sean/course/CS61C-2015/week4/hw1/beargit.c > CMakeFiles/week4.dir/hw1/beargit.c.i

CMakeFiles/week4.dir/hw1/beargit.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/week4.dir/hw1/beargit.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sean/course/CS61C-2015/week4/hw1/beargit.c -o CMakeFiles/week4.dir/hw1/beargit.c.s

CMakeFiles/week4.dir/hw1/beargit.c.o.requires:

.PHONY : CMakeFiles/week4.dir/hw1/beargit.c.o.requires

CMakeFiles/week4.dir/hw1/beargit.c.o.provides: CMakeFiles/week4.dir/hw1/beargit.c.o.requires
	$(MAKE) -f CMakeFiles/week4.dir/build.make CMakeFiles/week4.dir/hw1/beargit.c.o.provides.build
.PHONY : CMakeFiles/week4.dir/hw1/beargit.c.o.provides

CMakeFiles/week4.dir/hw1/beargit.c.o.provides.build: CMakeFiles/week4.dir/hw1/beargit.c.o


CMakeFiles/week4.dir/hw1/cunittests.c.o: CMakeFiles/week4.dir/flags.make
CMakeFiles/week4.dir/hw1/cunittests.c.o: ../hw1/cunittests.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sean/course/CS61C-2015/week4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/week4.dir/hw1/cunittests.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/week4.dir/hw1/cunittests.c.o   -c /home/sean/course/CS61C-2015/week4/hw1/cunittests.c

CMakeFiles/week4.dir/hw1/cunittests.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/week4.dir/hw1/cunittests.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sean/course/CS61C-2015/week4/hw1/cunittests.c > CMakeFiles/week4.dir/hw1/cunittests.c.i

CMakeFiles/week4.dir/hw1/cunittests.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/week4.dir/hw1/cunittests.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sean/course/CS61C-2015/week4/hw1/cunittests.c -o CMakeFiles/week4.dir/hw1/cunittests.c.s

CMakeFiles/week4.dir/hw1/cunittests.c.o.requires:

.PHONY : CMakeFiles/week4.dir/hw1/cunittests.c.o.requires

CMakeFiles/week4.dir/hw1/cunittests.c.o.provides: CMakeFiles/week4.dir/hw1/cunittests.c.o.requires
	$(MAKE) -f CMakeFiles/week4.dir/build.make CMakeFiles/week4.dir/hw1/cunittests.c.o.provides.build
.PHONY : CMakeFiles/week4.dir/hw1/cunittests.c.o.provides

CMakeFiles/week4.dir/hw1/cunittests.c.o.provides.build: CMakeFiles/week4.dir/hw1/cunittests.c.o


CMakeFiles/week4.dir/hw1/main.c.o: CMakeFiles/week4.dir/flags.make
CMakeFiles/week4.dir/hw1/main.c.o: ../hw1/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sean/course/CS61C-2015/week4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/week4.dir/hw1/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/week4.dir/hw1/main.c.o   -c /home/sean/course/CS61C-2015/week4/hw1/main.c

CMakeFiles/week4.dir/hw1/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/week4.dir/hw1/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sean/course/CS61C-2015/week4/hw1/main.c > CMakeFiles/week4.dir/hw1/main.c.i

CMakeFiles/week4.dir/hw1/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/week4.dir/hw1/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sean/course/CS61C-2015/week4/hw1/main.c -o CMakeFiles/week4.dir/hw1/main.c.s

CMakeFiles/week4.dir/hw1/main.c.o.requires:

.PHONY : CMakeFiles/week4.dir/hw1/main.c.o.requires

CMakeFiles/week4.dir/hw1/main.c.o.provides: CMakeFiles/week4.dir/hw1/main.c.o.requires
	$(MAKE) -f CMakeFiles/week4.dir/build.make CMakeFiles/week4.dir/hw1/main.c.o.provides.build
.PHONY : CMakeFiles/week4.dir/hw1/main.c.o.provides

CMakeFiles/week4.dir/hw1/main.c.o.provides.build: CMakeFiles/week4.dir/hw1/main.c.o


CMakeFiles/week4.dir/hw1/util.c.o: CMakeFiles/week4.dir/flags.make
CMakeFiles/week4.dir/hw1/util.c.o: ../hw1/util.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sean/course/CS61C-2015/week4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/week4.dir/hw1/util.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/week4.dir/hw1/util.c.o   -c /home/sean/course/CS61C-2015/week4/hw1/util.c

CMakeFiles/week4.dir/hw1/util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/week4.dir/hw1/util.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sean/course/CS61C-2015/week4/hw1/util.c > CMakeFiles/week4.dir/hw1/util.c.i

CMakeFiles/week4.dir/hw1/util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/week4.dir/hw1/util.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sean/course/CS61C-2015/week4/hw1/util.c -o CMakeFiles/week4.dir/hw1/util.c.s

CMakeFiles/week4.dir/hw1/util.c.o.requires:

.PHONY : CMakeFiles/week4.dir/hw1/util.c.o.requires

CMakeFiles/week4.dir/hw1/util.c.o.provides: CMakeFiles/week4.dir/hw1/util.c.o.requires
	$(MAKE) -f CMakeFiles/week4.dir/build.make CMakeFiles/week4.dir/hw1/util.c.o.provides.build
.PHONY : CMakeFiles/week4.dir/hw1/util.c.o.provides

CMakeFiles/week4.dir/hw1/util.c.o.provides.build: CMakeFiles/week4.dir/hw1/util.c.o


# Object files for target week4
week4_OBJECTS = \
"CMakeFiles/week4.dir/hw1/beargit.c.o" \
"CMakeFiles/week4.dir/hw1/cunittests.c.o" \
"CMakeFiles/week4.dir/hw1/main.c.o" \
"CMakeFiles/week4.dir/hw1/util.c.o"

# External object files for target week4
week4_EXTERNAL_OBJECTS =

week4: CMakeFiles/week4.dir/hw1/beargit.c.o
week4: CMakeFiles/week4.dir/hw1/cunittests.c.o
week4: CMakeFiles/week4.dir/hw1/main.c.o
week4: CMakeFiles/week4.dir/hw1/util.c.o
week4: CMakeFiles/week4.dir/build.make
week4: CMakeFiles/week4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sean/course/CS61C-2015/week4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable week4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/week4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/week4.dir/build: week4

.PHONY : CMakeFiles/week4.dir/build

CMakeFiles/week4.dir/requires: CMakeFiles/week4.dir/hw1/beargit.c.o.requires
CMakeFiles/week4.dir/requires: CMakeFiles/week4.dir/hw1/cunittests.c.o.requires
CMakeFiles/week4.dir/requires: CMakeFiles/week4.dir/hw1/main.c.o.requires
CMakeFiles/week4.dir/requires: CMakeFiles/week4.dir/hw1/util.c.o.requires

.PHONY : CMakeFiles/week4.dir/requires

CMakeFiles/week4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/week4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/week4.dir/clean

CMakeFiles/week4.dir/depend:
	cd /home/sean/course/CS61C-2015/week4/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sean/course/CS61C-2015/week4 /home/sean/course/CS61C-2015/week4 /home/sean/course/CS61C-2015/week4/cmake-build-debug /home/sean/course/CS61C-2015/week4/cmake-build-debug /home/sean/course/CS61C-2015/week4/cmake-build-debug/CMakeFiles/week4.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/week4.dir/depend
