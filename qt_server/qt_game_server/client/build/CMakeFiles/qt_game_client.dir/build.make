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
CMAKE_SOURCE_DIR = /home/ivan/codes/qt_server/qt_game_server/client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ivan/codes/qt_server/qt_game_server/client/build

# Include any dependencies generated for this target.
include CMakeFiles/qt_game_client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/qt_game_client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/qt_game_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/qt_game_client.dir/flags.make

CMakeFiles/qt_game_client.dir/qt_game_client_autogen/mocs_compilation.cpp.o: CMakeFiles/qt_game_client.dir/flags.make
CMakeFiles/qt_game_client.dir/qt_game_client_autogen/mocs_compilation.cpp.o: qt_game_client_autogen/mocs_compilation.cpp
CMakeFiles/qt_game_client.dir/qt_game_client_autogen/mocs_compilation.cpp.o: CMakeFiles/qt_game_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ivan/codes/qt_server/qt_game_server/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/qt_game_client.dir/qt_game_client_autogen/mocs_compilation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/qt_game_client.dir/qt_game_client_autogen/mocs_compilation.cpp.o -MF CMakeFiles/qt_game_client.dir/qt_game_client_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/qt_game_client.dir/qt_game_client_autogen/mocs_compilation.cpp.o -c /home/ivan/codes/qt_server/qt_game_server/client/build/qt_game_client_autogen/mocs_compilation.cpp

CMakeFiles/qt_game_client.dir/qt_game_client_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/qt_game_client.dir/qt_game_client_autogen/mocs_compilation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ivan/codes/qt_server/qt_game_server/client/build/qt_game_client_autogen/mocs_compilation.cpp > CMakeFiles/qt_game_client.dir/qt_game_client_autogen/mocs_compilation.cpp.i

CMakeFiles/qt_game_client.dir/qt_game_client_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/qt_game_client.dir/qt_game_client_autogen/mocs_compilation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ivan/codes/qt_server/qt_game_server/client/build/qt_game_client_autogen/mocs_compilation.cpp -o CMakeFiles/qt_game_client.dir/qt_game_client_autogen/mocs_compilation.cpp.s

CMakeFiles/qt_game_client.dir/client.cpp.o: CMakeFiles/qt_game_client.dir/flags.make
CMakeFiles/qt_game_client.dir/client.cpp.o: ../client.cpp
CMakeFiles/qt_game_client.dir/client.cpp.o: CMakeFiles/qt_game_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ivan/codes/qt_server/qt_game_server/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/qt_game_client.dir/client.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/qt_game_client.dir/client.cpp.o -MF CMakeFiles/qt_game_client.dir/client.cpp.o.d -o CMakeFiles/qt_game_client.dir/client.cpp.o -c /home/ivan/codes/qt_server/qt_game_server/client/client.cpp

CMakeFiles/qt_game_client.dir/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/qt_game_client.dir/client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ivan/codes/qt_server/qt_game_server/client/client.cpp > CMakeFiles/qt_game_client.dir/client.cpp.i

CMakeFiles/qt_game_client.dir/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/qt_game_client.dir/client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ivan/codes/qt_server/qt_game_server/client/client.cpp -o CMakeFiles/qt_game_client.dir/client.cpp.s

CMakeFiles/qt_game_client.dir/main.cpp.o: CMakeFiles/qt_game_client.dir/flags.make
CMakeFiles/qt_game_client.dir/main.cpp.o: ../main.cpp
CMakeFiles/qt_game_client.dir/main.cpp.o: CMakeFiles/qt_game_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ivan/codes/qt_server/qt_game_server/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/qt_game_client.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/qt_game_client.dir/main.cpp.o -MF CMakeFiles/qt_game_client.dir/main.cpp.o.d -o CMakeFiles/qt_game_client.dir/main.cpp.o -c /home/ivan/codes/qt_server/qt_game_server/client/main.cpp

CMakeFiles/qt_game_client.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/qt_game_client.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ivan/codes/qt_server/qt_game_server/client/main.cpp > CMakeFiles/qt_game_client.dir/main.cpp.i

CMakeFiles/qt_game_client.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/qt_game_client.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ivan/codes/qt_server/qt_game_server/client/main.cpp -o CMakeFiles/qt_game_client.dir/main.cpp.s

# Object files for target qt_game_client
qt_game_client_OBJECTS = \
"CMakeFiles/qt_game_client.dir/qt_game_client_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/qt_game_client.dir/client.cpp.o" \
"CMakeFiles/qt_game_client.dir/main.cpp.o"

# External object files for target qt_game_client
qt_game_client_EXTERNAL_OBJECTS =

qt_game_client: CMakeFiles/qt_game_client.dir/qt_game_client_autogen/mocs_compilation.cpp.o
qt_game_client: CMakeFiles/qt_game_client.dir/client.cpp.o
qt_game_client: CMakeFiles/qt_game_client.dir/main.cpp.o
qt_game_client: CMakeFiles/qt_game_client.dir/build.make
qt_game_client: /usr/lib/x86_64-linux-gnu/libQt6Network.so.6.2.4
qt_game_client: /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.2.4
qt_game_client: CMakeFiles/qt_game_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ivan/codes/qt_server/qt_game_server/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable qt_game_client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/qt_game_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/qt_game_client.dir/build: qt_game_client
.PHONY : CMakeFiles/qt_game_client.dir/build

CMakeFiles/qt_game_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/qt_game_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/qt_game_client.dir/clean

CMakeFiles/qt_game_client.dir/depend:
	cd /home/ivan/codes/qt_server/qt_game_server/client/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ivan/codes/qt_server/qt_game_server/client /home/ivan/codes/qt_server/qt_game_server/client /home/ivan/codes/qt_server/qt_game_server/client/build /home/ivan/codes/qt_server/qt_game_server/client/build /home/ivan/codes/qt_server/qt_game_server/client/build/CMakeFiles/qt_game_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/qt_game_client.dir/depend

