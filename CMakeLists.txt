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
find_program(BYACC byacc)
if(${BISON} STREQUAL BISON-NOTFOUND AND ${BYACC} STREQUAL BYACC-NOTFOUND)
	message(FATAL_ERROR "Missing `bison` or `byacc` utility. Please refer to your distribution package manager for obtaining one or the other.")
else()
	if(${BISON} STREQUAL BISON-NOTFOUND)
		set(YACC byacc)
	else()
		set(YACC bison)
	endif()
endif()
find_program(PKGCONFIG pkg-config)
find_program(PKGCONF pkgconf)
if(${PKGCONFIG} STREQUAL PKGCONFIG-NOTFOUND AND ${PKGCONF} STREQUAL PKGCONF-NOTFOUND)
	message(FATAL_ERROR "Missing `pkg-config` or `pkgconf` utility. Please refer to your distribution package manager for obtaining one or the other.")
endif()
find_program(M4 m4)
if(${M4} STREQUAL M4-NOTFOUND)
	message(FATAL_ERROR "Missing `m4` utility. Please refer to your distribution package manager for obtaining this.")
endif()

#TODO: appimage building support

option(NEOSURF_BUILD_GTK3_FRONTEND "Build and install the bundled Gtk3 frontend" ON)
option(NEOSURF_BUILD_VI_FRONTEND "Build and install the bundled Visurf frontend" ON)
option(NEOSURF_INSTALL_NSGENBIND "Installs the nsgenbind utility with neosurf" OFF)
option(NEOSURF_INSTALL_GEN_PARSER "Installs the libcss gen_parser utility with neosurf" OFF)
option(NEOSURF_BUILD_XXD "Build and install a complimentary xxd implementation" OFF)

include_directories(
	"${CMAKE_SOURCE_DIR}/contrib/libsvgtiny/include"
	"${CMAKE_SOURCE_DIR}/contrib/libdom/include"
	"${CMAKE_SOURCE_DIR}/contrib/libnsutils/include"
	"${CMAKE_SOURCE_DIR}/contrib/libnsgif/include"
	"${CMAKE_BINARY_DIR}/contrib/libsvgtiny"
	"${CMAKE_SOURCE_DIR}/contrib/libnsbmp/include"
	"${CMAKE_SOURCE_DIR}/contrib/libhubbub/include"
	"${CMAKE_SOURCE_DIR}/contrib/libparserutils/include"
	"${CMAKE_SOURCE_DIR}/contrib/libcss/include"
	"${CMAKE_BINARY_DIR}/contrib/libcss"
	"${CMAKE_SOURCE_DIR}/contrib/libsvgtiny/src"
	"${CMAKE_BINARY_DIR}/contrib/libhubbub/src"
)

add_subdirectory(contrib)
add_subdirectory(src)

#TODO: install headers for libcss, libdom, etc
#TODO: ensure frontends do not use any headers that aren't installed (e.g. they can be built separately)

#TODO: ensure posix compliant shell scripts
