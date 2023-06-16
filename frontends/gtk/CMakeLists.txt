find_package(PkgConfig)

# Second GTK_RESPATH entry is for AppImages
add_definitions(-DGTK_RESPATH="${CMAKE_INSTALL_PREFIX}/share/neosurf-gtk:./${CMAKE_INSTALL_PREFIX}/share/neosurf-gtk" -DWITH_GRESOURCE -DWITH_BUILTIN_PIXBUF -DNEOSURF_HOMEPAGE="about:welcome")

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

install(TARGETS neosurf-gtk DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY res/ DESTINATION ${CMAKE_INSTALL_DATADIR}/neosurf-gtk)
