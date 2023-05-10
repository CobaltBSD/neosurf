cmake_minimum_required(VERSION 3.20)
project(neosurf)

set(CMAKE_INSTALL_LIBDIR lib CACHE STRING "Directory to install to inside of CMAKE_INSTALL_PREFIX")

# Abort if source or build directory has white space
if(${CMAKE_SOURCE_DIR} MATCHES "^(.* +.*)+$")
	message(FATAL_ERROR "neosurf's source cannot be in a directory containing white space. Please move to a different location and try again.")
endif()
if(${CMAKE_BINARY_DIR} MATCHES "^(.* +.*)+$")
	message(FATAL_ERROR "neosurf cannot be built in a directory containing white space. Please move to a different location and try again.")
endif()

# Check for required programs
find_program(GPERF gperf)
if(${GPERF} STREQUAL GPERF-NOTFOUND)
	message(FATAL_ERROR "Missing `gperf` utility. Please refer to your distribution package manager for obtaining this.")
endif()
find_program(PYTHON3 python3)
if(${PYTHON3} STREQUAL PYTHON3-NOTFOUND)
	message(FATAL_ERROR "Missing Python3 executable. Please refer to your distribution package manager for obtaining this.")
endif()
find_program(FLEX flex)
if(${FLEX} STREQUAL FLEX-NOTFOUND)
	message(FATAL_ERROR "Missing `flex` utility. Please refer to your distribution package manager for obtaining this.")
endif()
find_program(BISON bison)
if(${BISON} STREQUAL BISON-NOTFOUND)
	message(FATAL_ERROR "Missing `bison` utility. Please refer to your distribution package manager for obtaining this.")
endif()

#TODO: appimage building support

option(NEOSURF_BUILD_GTK3_FRONTEND "Build and install the bundled Gtk3 frontend" ON)
option(NEOSURF_BUILD_VI_FRONTEND "Build and install the bundled Visurf frontend" ON)
option(NEOSURF_INSTALL_NSGENBIND "Installs the nsgenbind utility with neosurf" OFF)
option(NEOSURF_INSTALL_GEN_PARSER "Installs the libcss gen_parser utility with neosurf" OFF)
option(NEOSURF_BUILD_XXD "Build and install a complimentary xxd implementation" OFF)

add_subdirectory(contrib)
add_subdirectory(src)
