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
include zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/depend.make

# Include the progress variables for this target.
include zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/progress.make

# Include the compile flags for this target's objects.
include zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/flags.make

zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/__/secret_service.c.o: zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/flags.make
zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/__/secret_service.c.o: zuluCrypt-gui/lxqt_wallet/frontend/secret_service.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kod/soft/zuluCrypt-4.7.4/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/__/secret_service.c.o"
	cd /home/kod/soft/zuluCrypt-4.7.4/zuluCrypt-gui/lxqt_wallet/frontend/secret_service && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/secretService.dir/__/secret_service.c.o   -c /home/kod/soft/zuluCrypt-4.7.4/zuluCrypt-gui/lxqt_wallet/frontend/secret_service.c

zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/__/secret_service.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/secretService.dir/__/secret_service.c.i"
	cd /home/kod/soft/zuluCrypt-4.7.4/zuluCrypt-gui/lxqt_wallet/frontend/secret_service && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/kod/soft/zuluCrypt-4.7.4/zuluCrypt-gui/lxqt_wallet/frontend/secret_service.c > CMakeFiles/secretService.dir/__/secret_service.c.i

zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/__/secret_service.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/secretService.dir/__/secret_service.c.s"
	cd /home/kod/soft/zuluCrypt-4.7.4/zuluCrypt-gui/lxqt_wallet/frontend/secret_service && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/kod/soft/zuluCrypt-4.7.4/zuluCrypt-gui/lxqt_wallet/frontend/secret_service.c -o CMakeFiles/secretService.dir/__/secret_service.c.s

zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/__/secret_service.c.o.requires:
.PHONY : zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/__/secret_service.c.o.requires

zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/__/secret_service.c.o.provides: zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/__/secret_service.c.o.requires
	$(MAKE) -f zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/build.make zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/__/secret_service.c.o.provides.build
.PHONY : zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/__/secret_service.c.o.provides

zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/__/secret_service.c.o.provides.build: zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/__/secret_service.c.o

# Object files for target secretService
secretService_OBJECTS = \
"CMakeFiles/secretService.dir/__/secret_service.c.o"

# External object files for target secretService
secretService_EXTERNAL_OBJECTS =

zuluCrypt-gui/lxqt_wallet/frontend/secret_service/libsecretService.a: zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/__/secret_service.c.o
zuluCrypt-gui/lxqt_wallet/frontend/secret_service/libsecretService.a: zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/build.make
zuluCrypt-gui/lxqt_wallet/frontend/secret_service/libsecretService.a: zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C static library libsecretService.a"
	cd /home/kod/soft/zuluCrypt-4.7.4/zuluCrypt-gui/lxqt_wallet/frontend/secret_service && $(CMAKE_COMMAND) -P CMakeFiles/secretService.dir/cmake_clean_target.cmake
	cd /home/kod/soft/zuluCrypt-4.7.4/zuluCrypt-gui/lxqt_wallet/frontend/secret_service && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/secretService.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/build: zuluCrypt-gui/lxqt_wallet/frontend/secret_service/libsecretService.a
.PHONY : zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/build

zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/requires: zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/__/secret_service.c.o.requires
.PHONY : zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/requires

zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/clean:
	cd /home/kod/soft/zuluCrypt-4.7.4/zuluCrypt-gui/lxqt_wallet/frontend/secret_service && $(CMAKE_COMMAND) -P CMakeFiles/secretService.dir/cmake_clean.cmake
.PHONY : zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/clean

zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/depend:
	cd /home/kod/soft/zuluCrypt-4.7.4 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kod/soft/zuluCrypt-4.7.4 /home/kod/soft/zuluCrypt-4.7.4/zuluCrypt-gui/lxqt_wallet/frontend/secret_service /home/kod/soft/zuluCrypt-4.7.4 /home/kod/soft/zuluCrypt-4.7.4/zuluCrypt-gui/lxqt_wallet/frontend/secret_service /home/kod/soft/zuluCrypt-4.7.4/zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : zuluCrypt-gui/lxqt_wallet/frontend/secret_service/CMakeFiles/secretService.dir/depend

