cmake_minimum_required(VERSION 3.20)
project(neosurf LANGUAGES C)

set(NEOSURF_ABI 14)
set(NEOSURF_VERSION 15)

add_definitions(-DNDEBUG -DWITH_OPENSSL -DWITH_BMP -DWITH_GIF -DWITH_NS_SVG -D_XOPEN_SOURCE=700 -D_DEFAULT_SOURCE -DNEOSURF_VERSION=${NEOSURF_VERSION})
add_compile_options(-std=c99 -Wno-unused-parameter -Wno-deprecated-declarations -Wno-strict-prototypes -Wno-unused-but-set-variable -Wno-int-conversion)

include(GNUInstallDirs)

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
message("yacc program found: ${YACC}")

find_program(PKGCONFIG pkg-config)
find_program(PKGCONF pkgconf)
if(${PKGCONFIG} STREQUAL PKGCONFIG-NOTFOUND AND ${PKGCONF} STREQUAL PKGCONF-NOTFOUND)
	message(FATAL_ERROR "Missing `pkg-config` or `pkgconf` utility. Please refer to your distribution package manager for obtaining one or the other.")
endif()
find_program(M4 m4)
if(${M4} STREQUAL M4-NOTFOUND)
	message(FATAL_ERROR "Missing `m4` utility. Please refer to your distribution package manager for obtaining this.")
endif()

option(NEOSURF_USE_PNG "Include png support" ON)
if(${NEOSURF_USE_PNG})
	add_definitions(-DWITH_PNG)
	set(PNG_SRC content/handlers/image/png.c)
endif()
option(NEOSURF_USE_JPEG "Include jpeg support" ON)
if(${NEOSURF_USE_JPEG})
	add_definitions(-DWITH_JPEG)
	set(JPEG_SRC content/handlers/image/jpeg.c)
endif()
option(NEOSURF_USE_WEBP "Include webp support" ON)
if(${NEOSURF_USE_WEBP})
	add_definitions(-DWITH_WEBP)
	set(WEBP_SRC content/handlers/image/webp.c)
endif()

option(NEOSURF_BUILD_GTK_FRONTEND "Build and install the bundled Gtk frontend" ON)
if(NEOSURF_BUILD_GTK_FRONTEND)
	add_definitions(-Dgtk -Dnsgtk)
endif()
option(NEOSURF_BUILD_VI_FRONTEND "Build and install the bundled Visurf frontend" ON)
if(NEOSURF_BUILD_VI_FRONTEND)
	add_definitions(-Dvi -Dnsvi)
endif()
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
	"${CMAKE_SOURCE_DIR}/frontends"
)

add_subdirectory(contrib)
add_subdirectory(src)
add_subdirectory(frontends)

#TODO: quickjs/wasm3

install(DIRECTORY include/neosurf DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#FIXME: gtk frontend seems to segfault if no proper cursor theme is installed
#FIXME: will fail in mysterious ways if user does not have write access to their ~/.config/
