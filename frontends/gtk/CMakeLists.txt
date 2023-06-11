find_package(PkgConfig)

pkg_check_modules(GTK REQUIRED gtk+-3.0)
include_directories(${GTK_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(neosurf-gtk
	gui.c
	misc.c
	schedule.c
	layout_pango.c
	bitmap.c
	plotters.c
	scaffolding.c
	gdk.c
	completion.c
	throbber.c
	accelerator.c
	selection.c
	window.c
	fetch.c
	download.c
	menu.c
	print.c
	search.c
	tabs.c
	toolbar.c
	compat.c
	viewdata.c
	viewsource.c
	preferences.c
	about.c
	resources.c
	corewindow.c
	local_history.c
	global_history.c
	cookies.c
	hotlist.c
	page_info.c
)

target_link_libraries(neosurf-gtk neosurf m ${NEOSURF_COMMON_LIBS} ${GTK_LIBRARIES})

install(TARGETS neosurf-gtk DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY res/ DESTINATION ${CMAKE_INSTALL_PREFIX}/share/neosurf-gtk)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -DNDEBUG -DWITH_OPENSSL -DWITH_BMP -DWITH_GIF -DWITH_JPEG -DWITH_PNG -DWITH_WEBP -DWITH_NS_SVG -Dgtk -Dnsgtk -DGTK_RESPATH=\\\"${CMAKE_INSTALL_PREFIX}/share/neosurf-gtk\\\" -DNEOSURF_HOMEPAGE=\\\"about:welcome\\\" -DWITH_GRESOURCE -DWITH_BUILTIN_PIXBUF -Wno-unused-parameter -Wno-deprecated-declarations -Wno-strict-prototypes -Wno-unused-but-set-variable -Wno-int-conversion -D_XOPEN_SOURCE=700 -D_DEFAULT_SOURCE")
