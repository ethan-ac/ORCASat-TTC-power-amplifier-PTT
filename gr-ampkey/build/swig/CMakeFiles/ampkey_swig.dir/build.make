# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/ethan/install/gr-ampkey

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ethan/install/gr-ampkey/build

# Include any dependencies generated for this target.
include swig/CMakeFiles/ampkey_swig.dir/depend.make

# Include the progress variables for this target.
include swig/CMakeFiles/ampkey_swig.dir/progress.make

# Include the compile flags for this target's objects.
include swig/CMakeFiles/ampkey_swig.dir/flags.make

swig/CMakeFiles/ampkey_swig.dir/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx.o: swig/CMakeFiles/ampkey_swig.dir/flags.make
swig/CMakeFiles/ampkey_swig.dir/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx.o: swig/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ethan/install/gr-ampkey/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object swig/CMakeFiles/ampkey_swig.dir/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx.o"
	cd /home/ethan/install/gr-ampkey/build/swig && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ampkey_swig.dir/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx.o -c /home/ethan/install/gr-ampkey/build/swig/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx

swig/CMakeFiles/ampkey_swig.dir/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ampkey_swig.dir/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx.i"
	cd /home/ethan/install/gr-ampkey/build/swig && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ethan/install/gr-ampkey/build/swig/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx > CMakeFiles/ampkey_swig.dir/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx.i

swig/CMakeFiles/ampkey_swig.dir/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ampkey_swig.dir/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx.s"
	cd /home/ethan/install/gr-ampkey/build/swig && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ethan/install/gr-ampkey/build/swig/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx -o CMakeFiles/ampkey_swig.dir/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx.s

# Object files for target ampkey_swig
ampkey_swig_OBJECTS = \
"CMakeFiles/ampkey_swig.dir/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx.o"

# External object files for target ampkey_swig
ampkey_swig_EXTERNAL_OBJECTS =

swig/_ampkey_swig.so: swig/CMakeFiles/ampkey_swig.dir/CMakeFiles/ampkey_swig.dir/ampkey_swigPYTHON_wrap.cxx.o
swig/_ampkey_swig.so: swig/CMakeFiles/ampkey_swig.dir/build.make
swig/_ampkey_swig.so: lib/libgnuradio-ampkey.so.v1.0-compat-xxx-xunknown
swig/_ampkey_swig.so: /usr/lib/x86_64-linux-gnu/libpython3.8.so
swig/_ampkey_swig.so: /usr/lib/x86_64-linux-gnu/libgnuradio-runtime.so.3.8.2.0
swig/_ampkey_swig.so: /usr/lib/x86_64-linux-gnu/libgnuradio-pmt.so.3.8.2.0
swig/_ampkey_swig.so: /usr/lib/x86_64-linux-gnu/libvolk.so.2.2
swig/_ampkey_swig.so: /usr/lib/x86_64-linux-gnu/liborc-0.4.so
swig/_ampkey_swig.so: /usr/lib/x86_64-linux-gnu/libboost_program_options.so.1.71.0
swig/_ampkey_swig.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.71.0
swig/_ampkey_swig.so: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
swig/_ampkey_swig.so: /usr/lib/x86_64-linux-gnu/libboost_regex.so.1.71.0
swig/_ampkey_swig.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so.1.71.0
swig/_ampkey_swig.so: /usr/lib/x86_64-linux-gnu/libboost_atomic.so.1.71.0
swig/_ampkey_swig.so: /usr/lib/x86_64-linux-gnu/liblog4cpp.so
swig/_ampkey_swig.so: /usr/lib/x86_64-linux-gnu/libgmpxx.so
swig/_ampkey_swig.so: /usr/lib/x86_64-linux-gnu/libgmp.so
swig/_ampkey_swig.so: swig/CMakeFiles/ampkey_swig.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ethan/install/gr-ampkey/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared module _ampkey_swig.so"
	cd /home/ethan/install/gr-ampkey/build/swig && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ampkey_swig.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
swig/CMakeFiles/ampkey_swig.dir/build: swig/_ampkey_swig.so

.PHONY : swig/CMakeFiles/ampkey_swig.dir/build

swig/CMakeFiles/ampkey_swig.dir/clean:
	cd /home/ethan/install/gr-ampkey/build/swig && $(CMAKE_COMMAND) -P CMakeFiles/ampkey_swig.dir/cmake_clean.cmake
.PHONY : swig/CMakeFiles/ampkey_swig.dir/clean

swig/CMakeFiles/ampkey_swig.dir/depend:
	cd /home/ethan/install/gr-ampkey/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ethan/install/gr-ampkey /home/ethan/install/gr-ampkey/swig /home/ethan/install/gr-ampkey/build /home/ethan/install/gr-ampkey/build/swig /home/ethan/install/gr-ampkey/build/swig/CMakeFiles/ampkey_swig.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : swig/CMakeFiles/ampkey_swig.dir/depend

