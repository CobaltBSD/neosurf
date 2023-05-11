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

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -D_BSD_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -Wno-unused-variable -DNDEBUG -Wno-unused-function -D_POSIX_C_SOURCE=200809L")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${CMAKE_SOURCE_DIR}/contrib/libsvgtiny/include -I${CMAKE_SOURCE_DIR}/contrib/libdom/include -I${CMAKE_SOURCE_DIR}/contrib/libnsutils/include -I${CMAKE_SOURCE_DIR}/contrib/libnsgif/include -I${CMAKE_BINARY_DIR}/contrib/libsvgtiny -I${CMAKE_SOURCE_DIR}/contrib/libnsbmp/include -I${CMAKE_SOURCE_DIR}/contrib/libhubbub/include -I${CMAKE_SOURCE_DIR}/contrib/libparserutils/include -I${CMAKE_SOURCE_DIR}/contrib/libcss/include -I${CMAKE_BINARY_DIR}/contrib/libcss -I${CMAKE_SOURCE_DIR}/contrib/libsvgtiny/src -I${CMAKE_BINARY_DIR}/contrib/libhubbub/src")

add_subdirectory(contrib)
add_subdirectory(src)

#TODO: install headers for libcss, libdom, etc
#TODO: ensure frontends do not use any headers that aren't installed (e.g. they can be built separately)
