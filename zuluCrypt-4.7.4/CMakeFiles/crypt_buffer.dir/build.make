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
include CMakeFiles/crypt_buffer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/crypt_buffer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/crypt_buffer.dir/flags.make

CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.o: CMakeFiles/crypt_buffer.dir/flags.make
CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.o: plugins/network_key/crypt_buffer.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kod/soft/zuluCrypt-4.7.4/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.o   -c /home/kod/soft/zuluCrypt-4.7.4/plugins/network_key/crypt_buffer.c

CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/kod/soft/zuluCrypt-4.7.4/plugins/network_key/crypt_buffer.c > CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.i

CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/kod/soft/zuluCrypt-4.7.4/plugins/network_key/crypt_buffer.c -o CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.s

CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.o.requires:
.PHONY : CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.o.requires

CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.o.provides: CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.o.requires
	$(MAKE) -f CMakeFiles/crypt_buffer.dir/build.make CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.o.provides.build
.PHONY : CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.o.provides

CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.o.provides.build: CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.o

CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.o: CMakeFiles/crypt_buffer.dir/flags.make
CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.o: zuluCrypt-cli/utility/socket/socket.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/kod/soft/zuluCrypt-4.7.4/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.o   -c /home/kod/soft/zuluCrypt-4.7.4/zuluCrypt-cli/utility/socket/socket.c

CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/kod/soft/zuluCrypt-4.7.4/zuluCrypt-cli/utility/socket/socket.c > CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.i

CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/kod/soft/zuluCrypt-4.7.4/zuluCrypt-cli/utility/socket/socket.c -o CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.s

CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.o.requires:
.PHONY : CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.o.requires

CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.o.provides: CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.o.requires
	$(MAKE) -f CMakeFiles/crypt_buffer.dir/build.make CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.o.provides.build
.PHONY : CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.o.provides

CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.o.provides.build: CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.o

# Object files for target crypt_buffer
crypt_buffer_OBJECTS = \
"CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.o" \
"CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.o"

# External object files for target crypt_buffer
crypt_buffer_EXTERNAL_OBJECTS =

libcrypt_buffer.a: CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.o
libcrypt_buffer.a: CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.o
libcrypt_buffer.a: CMakeFiles/crypt_buffer.dir/build.make
libcrypt_buffer.a: CMakeFiles/crypt_buffer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C static library libcrypt_buffer.a"
	$(CMAKE_COMMAND) -P CMakeFiles/crypt_buffer.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/crypt_buffer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/crypt_buffer.dir/build: libcrypt_buffer.a
.PHONY : CMakeFiles/crypt_buffer.dir/build

CMakeFiles/crypt_buffer.dir/requires: CMakeFiles/crypt_buffer.dir/plugins/network_key/crypt_buffer.c.o.requires
CMakeFiles/crypt_buffer.dir/requires: CMakeFiles/crypt_buffer.dir/zuluCrypt-cli/utility/socket/socket.c.o.requires
.PHONY : CMakeFiles/crypt_buffer.dir/requires

CMakeFiles/crypt_buffer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/crypt_buffer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/crypt_buffer.dir/clean

CMakeFiles/crypt_buffer.dir/depend:
	cd /home/kod/soft/zuluCrypt-4.7.4 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kod/soft/zuluCrypt-4.7.4 /home/kod/soft/zuluCrypt-4.7.4 /home/kod/soft/zuluCrypt-4.7.4 /home/kod/soft/zuluCrypt-4.7.4 /home/kod/soft/zuluCrypt-4.7.4/CMakeFiles/crypt_buffer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/crypt_buffer.dir/depend

