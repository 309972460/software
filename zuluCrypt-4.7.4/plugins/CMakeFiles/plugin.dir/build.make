# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/kod/soft/zuluCrypt-4.7.4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kod/soft/zuluCrypt-4.7.4

# Include any dependencies generated for this target.
include plugins/CMakeFiles/plugin.dir/depend.make

# Include the progress variables for this target.
include plugins/CMakeFiles/plugin.dir/progress.make

# Include the compile flags for this target's objects.
include plugins/CMakeFiles/plugin.dir/flags.make

plugins/moc_mainwindow.cxx: plugins/mainwindow.h
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kod/soft/zuluCrypt-4.7.4/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating moc_mainwindow.cxx"
	cd /home/kod/soft/zuluCrypt-4.7.4/plugins && /usr/lib/x86_64-linux-gnu/qt4/bin/moc @/home/kod/soft/zuluCrypt-4.7.4/plugins/moc_mainwindow.cxx_parameters

plugins/ui_mainwindow.h: plugins/mainwindow.ui
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kod/soft/zuluCrypt-4.7.4/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ui_mainwindow.h"
	cd /home/kod/soft/zuluCrypt-4.7.4/plugins && /usr/lib/x86_64-linux-gnu/qt4/bin/uic -o /home/kod/soft/zuluCrypt-4.7.4/plugins/ui_mainwindow.h /home/kod/soft/zuluCrypt-4.7.4/plugins/mainwindow.ui

plugins/CMakeFiles/plugin.dir/moc_mainwindow.cxx.o: plugins/CMakeFiles/plugin.dir/flags.make
plugins/CMakeFiles/plugin.dir/moc_mainwindow.cxx.o: plugins/moc_mainwindow.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kod/soft/zuluCrypt-4.7.4/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object plugins/CMakeFiles/plugin.dir/moc_mainwindow.cxx.o"
	cd /home/kod/soft/zuluCrypt-4.7.4/plugins && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/plugin.dir/moc_mainwindow.cxx.o -c /home/kod/soft/zuluCrypt-4.7.4/plugins/moc_mainwindow.cxx

plugins/CMakeFiles/plugin.dir/moc_mainwindow.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/plugin.dir/moc_mainwindow.cxx.i"
	cd /home/kod/soft/zuluCrypt-4.7.4/plugins && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/kod/soft/zuluCrypt-4.7.4/plugins/moc_mainwindow.cxx > CMakeFiles/plugin.dir/moc_mainwindow.cxx.i

plugins/CMakeFiles/plugin.dir/moc_mainwindow.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/plugin.dir/moc_mainwindow.cxx.s"
	cd /home/kod/soft/zuluCrypt-4.7.4/plugins && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/kod/soft/zuluCrypt-4.7.4/plugins/moc_mainwindow.cxx -o CMakeFiles/plugin.dir/moc_mainwindow.cxx.s

plugins/CMakeFiles/plugin.dir/moc_mainwindow.cxx.o.requires:
.PHONY : plugins/CMakeFiles/plugin.dir/moc_mainwindow.cxx.o.requires

plugins/CMakeFiles/plugin.dir/moc_mainwindow.cxx.o.provides: plugins/CMakeFiles/plugin.dir/moc_mainwindow.cxx.o.requires
	$(MAKE) -f plugins/CMakeFiles/plugin.dir/build.make plugins/CMakeFiles/plugin.dir/moc_mainwindow.cxx.o.provides.build
.PHONY : plugins/CMakeFiles/plugin.dir/moc_mainwindow.cxx.o.provides

plugins/CMakeFiles/plugin.dir/moc_mainwindow.cxx.o.provides.build: plugins/CMakeFiles/plugin.dir/moc_mainwindow.cxx.o

plugins/CMakeFiles/plugin.dir/mainwindow.cpp.o: plugins/CMakeFiles/plugin.dir/flags.make
plugins/CMakeFiles/plugin.dir/mainwindow.cpp.o: plugins/mainwindow.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kod/soft/zuluCrypt-4.7.4/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object plugins/CMakeFiles/plugin.dir/mainwindow.cpp.o"
	cd /home/kod/soft/zuluCrypt-4.7.4/plugins && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/plugin.dir/mainwindow.cpp.o -c /home/kod/soft/zuluCrypt-4.7.4/plugins/mainwindow.cpp

plugins/CMakeFiles/plugin.dir/mainwindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/plugin.dir/mainwindow.cpp.i"
	cd /home/kod/soft/zuluCrypt-4.7.4/plugins && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/kod/soft/zuluCrypt-4.7.4/plugins/mainwindow.cpp > CMakeFiles/plugin.dir/mainwindow.cpp.i

plugins/CMakeFiles/plugin.dir/mainwindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/plugin.dir/mainwindow.cpp.s"
	cd /home/kod/soft/zuluCrypt-4.7.4/plugins && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/kod/soft/zuluCrypt-4.7.4/plugins/mainwindow.cpp -o CMakeFiles/plugin.dir/mainwindow.cpp.s

plugins/CMakeFiles/plugin.dir/mainwindow.cpp.o.requires:
.PHONY : plugins/CMakeFiles/plugin.dir/mainwindow.cpp.o.requires

plugins/CMakeFiles/plugin.dir/mainwindow.cpp.o.provides: plugins/CMakeFiles/plugin.dir/mainwindow.cpp.o.requires
	$(MAKE) -f plugins/CMakeFiles/plugin.dir/build.make plugins/CMakeFiles/plugin.dir/mainwindow.cpp.o.provides.build
.PHONY : plugins/CMakeFiles/plugin.dir/mainwindow.cpp.o.provides

plugins/CMakeFiles/plugin.dir/mainwindow.cpp.o.provides.build: plugins/CMakeFiles/plugin.dir/mainwindow.cpp.o

# Object files for target plugin
plugin_OBJECTS = \
"CMakeFiles/plugin.dir/moc_mainwindow.cxx.o" \
"CMakeFiles/plugin.dir/mainwindow.cpp.o"

# External object files for target plugin
plugin_EXTERNAL_OBJECTS =

plugins/libplugin.a: plugins/CMakeFiles/plugin.dir/moc_mainwindow.cxx.o
plugins/libplugin.a: plugins/CMakeFiles/plugin.dir/mainwindow.cpp.o
plugins/libplugin.a: plugins/CMakeFiles/plugin.dir/build.make
plugins/libplugin.a: plugins/CMakeFiles/plugin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libplugin.a"
	cd /home/kod/soft/zuluCrypt-4.7.4/plugins && $(CMAKE_COMMAND) -P CMakeFiles/plugin.dir/cmake_clean_target.cmake
	cd /home/kod/soft/zuluCrypt-4.7.4/plugins && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/plugin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
plugins/CMakeFiles/plugin.dir/build: plugins/libplugin.a
.PHONY : plugins/CMakeFiles/plugin.dir/build

plugins/CMakeFiles/plugin.dir/requires: plugins/CMakeFiles/plugin.dir/moc_mainwindow.cxx.o.requires
plugins/CMakeFiles/plugin.dir/requires: plugins/CMakeFiles/plugin.dir/mainwindow.cpp.o.requires
.PHONY : plugins/CMakeFiles/plugin.dir/requires

plugins/CMakeFiles/plugin.dir/clean:
	cd /home/kod/soft/zuluCrypt-4.7.4/plugins && $(CMAKE_COMMAND) -P CMakeFiles/plugin.dir/cmake_clean.cmake
.PHONY : plugins/CMakeFiles/plugin.dir/clean

plugins/CMakeFiles/plugin.dir/depend: plugins/moc_mainwindow.cxx
plugins/CMakeFiles/plugin.dir/depend: plugins/ui_mainwindow.h
	cd /home/kod/soft/zuluCrypt-4.7.4 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kod/soft/zuluCrypt-4.7.4 /home/kod/soft/zuluCrypt-4.7.4/plugins /home/kod/soft/zuluCrypt-4.7.4 /home/kod/soft/zuluCrypt-4.7.4/plugins /home/kod/soft/zuluCrypt-4.7.4/plugins/CMakeFiles/plugin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : plugins/CMakeFiles/plugin.dir/depend

