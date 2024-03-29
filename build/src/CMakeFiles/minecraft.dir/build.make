# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /u/hwett/Desktop/graphics/minecraft

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /u/hwett/Desktop/graphics/minecraft/build

# Include any dependencies generated for this target.
include src/CMakeFiles/minecraft.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/minecraft.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/minecraft.dir/flags.make

src/CMakeFiles/minecraft.dir/camera.cc.o: src/CMakeFiles/minecraft.dir/flags.make
src/CMakeFiles/minecraft.dir/camera.cc.o: ../src/camera.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/u/hwett/Desktop/graphics/minecraft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/minecraft.dir/camera.cc.o"
	cd /u/hwett/Desktop/graphics/minecraft/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/minecraft.dir/camera.cc.o -c /u/hwett/Desktop/graphics/minecraft/src/camera.cc

src/CMakeFiles/minecraft.dir/camera.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/minecraft.dir/camera.cc.i"
	cd /u/hwett/Desktop/graphics/minecraft/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /u/hwett/Desktop/graphics/minecraft/src/camera.cc > CMakeFiles/minecraft.dir/camera.cc.i

src/CMakeFiles/minecraft.dir/camera.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/minecraft.dir/camera.cc.s"
	cd /u/hwett/Desktop/graphics/minecraft/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /u/hwett/Desktop/graphics/minecraft/src/camera.cc -o CMakeFiles/minecraft.dir/camera.cc.s

src/CMakeFiles/minecraft.dir/camera.cc.o.requires:

.PHONY : src/CMakeFiles/minecraft.dir/camera.cc.o.requires

src/CMakeFiles/minecraft.dir/camera.cc.o.provides: src/CMakeFiles/minecraft.dir/camera.cc.o.requires
	$(MAKE) -f src/CMakeFiles/minecraft.dir/build.make src/CMakeFiles/minecraft.dir/camera.cc.o.provides.build
.PHONY : src/CMakeFiles/minecraft.dir/camera.cc.o.provides

src/CMakeFiles/minecraft.dir/camera.cc.o.provides.build: src/CMakeFiles/minecraft.dir/camera.cc.o


src/CMakeFiles/minecraft.dir/main.cc.o: src/CMakeFiles/minecraft.dir/flags.make
src/CMakeFiles/minecraft.dir/main.cc.o: ../src/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/u/hwett/Desktop/graphics/minecraft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/minecraft.dir/main.cc.o"
	cd /u/hwett/Desktop/graphics/minecraft/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/minecraft.dir/main.cc.o -c /u/hwett/Desktop/graphics/minecraft/src/main.cc

src/CMakeFiles/minecraft.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/minecraft.dir/main.cc.i"
	cd /u/hwett/Desktop/graphics/minecraft/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /u/hwett/Desktop/graphics/minecraft/src/main.cc > CMakeFiles/minecraft.dir/main.cc.i

src/CMakeFiles/minecraft.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/minecraft.dir/main.cc.s"
	cd /u/hwett/Desktop/graphics/minecraft/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /u/hwett/Desktop/graphics/minecraft/src/main.cc -o CMakeFiles/minecraft.dir/main.cc.s

src/CMakeFiles/minecraft.dir/main.cc.o.requires:

.PHONY : src/CMakeFiles/minecraft.dir/main.cc.o.requires

src/CMakeFiles/minecraft.dir/main.cc.o.provides: src/CMakeFiles/minecraft.dir/main.cc.o.requires
	$(MAKE) -f src/CMakeFiles/minecraft.dir/build.make src/CMakeFiles/minecraft.dir/main.cc.o.provides.build
.PHONY : src/CMakeFiles/minecraft.dir/main.cc.o.provides

src/CMakeFiles/minecraft.dir/main.cc.o.provides.build: src/CMakeFiles/minecraft.dir/main.cc.o


# Object files for target minecraft
minecraft_OBJECTS = \
"CMakeFiles/minecraft.dir/camera.cc.o" \
"CMakeFiles/minecraft.dir/main.cc.o"

# External object files for target minecraft
minecraft_EXTERNAL_OBJECTS =

bin/minecraft: src/CMakeFiles/minecraft.dir/camera.cc.o
bin/minecraft: src/CMakeFiles/minecraft.dir/main.cc.o
bin/minecraft: src/CMakeFiles/minecraft.dir/build.make
bin/minecraft: /usr/lib/x86_64-linux-gnu/libGLEW.so
bin/minecraft: /usr/lib/x86_64-linux-gnu/libGL.so
bin/minecraft: /usr/lib/x86_64-linux-gnu/libGLEW.so
bin/minecraft: libutgraphicsutil.a
bin/minecraft: /usr/lib/x86_64-linux-gnu/libGLEW.so
bin/minecraft: /usr/lib/x86_64-linux-gnu/libjpeg.so
bin/minecraft: src/CMakeFiles/minecraft.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/u/hwett/Desktop/graphics/minecraft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../bin/minecraft"
	cd /u/hwett/Desktop/graphics/minecraft/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/minecraft.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/minecraft.dir/build: bin/minecraft

.PHONY : src/CMakeFiles/minecraft.dir/build

src/CMakeFiles/minecraft.dir/requires: src/CMakeFiles/minecraft.dir/camera.cc.o.requires
src/CMakeFiles/minecraft.dir/requires: src/CMakeFiles/minecraft.dir/main.cc.o.requires

.PHONY : src/CMakeFiles/minecraft.dir/requires

src/CMakeFiles/minecraft.dir/clean:
	cd /u/hwett/Desktop/graphics/minecraft/build/src && $(CMAKE_COMMAND) -P CMakeFiles/minecraft.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/minecraft.dir/clean

src/CMakeFiles/minecraft.dir/depend:
	cd /u/hwett/Desktop/graphics/minecraft/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /u/hwett/Desktop/graphics/minecraft /u/hwett/Desktop/graphics/minecraft/src /u/hwett/Desktop/graphics/minecraft/build /u/hwett/Desktop/graphics/minecraft/build/src /u/hwett/Desktop/graphics/minecraft/build/src/CMakeFiles/minecraft.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/minecraft.dir/depend

