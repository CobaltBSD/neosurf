find_package(PkgConfig)

add_definitions(-DVISURF_RESPATH="${CMAKE_INSTALL_PREFIX}/share/neosurf-gtk")

pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)
include_directories(${XKBCOMMON_INCLUDE_DIRS})

pkg_check_modules(CAIRO REQUIRED cairo)
include_directories(${CAIRO_INCLUDE_DIRS})

pkg_check_modules(WAYLAND_CURSOR REQUIRED wayland-cursor)
include_directories(${WAYLAND_CURSOR_INCLUDE_DIRS})

pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)
include_directories(${WAYLAND_CLIENT_INCLUDE_DIRS})

pkg_check_modules(PANGO REQUIRED pango)
include_directories(${PANGO_INCLUDE_DIRS})

pkg_check_modules(PANGOCAIRO REQUIRED pangocairo)
include_directories(${PANGOCAIRO_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(neosurf-vi
	xdg-shell.c
	xdg-decoration.c
	main.c
	bitmap.c
	commands.c
	fetch.c
	getopt.c
	keybindings.c
	layout.c
	pool-buffer.c
	plotters.c
	window.c
	settings.c
	undo.c
)

target_link_libraries(neosurf-vi neosurf ${NEOSURF_COMMON_LIBS} ${XKBCOMMON_LIBRARIES} ${CAIRO_LIBRARIES} ${WAYLAND_CURSOR_LIBRARIES} ${WAYLAND_CLIENT_LIBRARIES} ${PANGO_LIBRARIES} ${PANGOCAIRO_LIBRARIES})

install(TARGETS neosurf-vi DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ../gtk/res/ DESTINATION ${CMAKE_INSTALL_DATADIR}/neosurf-gtk) # Visurf uses the same resources as the Gtk frontend
