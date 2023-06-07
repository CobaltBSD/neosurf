find_package(PkgConfig)

include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/src/content/handlers"
	"${CMAKE_SOURCE_DIR}/src/include"
	"${CMAKE_SOURCE_DIR}/src/frontends"
	"${CMAKE_BINARY_DIR}"
)

pkg_check_modules(LIBJPEG REQUIRED libjpeg)
include_directories(${LIBJPEG_INCLUDE_DIRS})

pkg_check_modules(LIBPNG REQUIRED libpng)
include_directories(${LIBPNG_INCLUDE_DIRS})

pkg_check_modules(LIBCURL REQUIRED libcurl)
include_directories(${LIBCURL_INCLUDE_DIRS})

pkg_check_modules(LIBCRYPTO REQUIRED libcrypto)
include_directories(${LIBCRYPTO_INCLUDE_DIRS})

pkg_check_modules(LIBPSL REQUIRED libpsl)
include_directories(${LIBPSL_INCLUDE_DIRS})

pkg_check_modules(LIBWEBP REQUIRED libwebp)
include_directories(${LIBWEBP_INCLUDE_DIRS})

pkg_check_modules(ZLIB REQUIRED zlib)
include_directories(${ZLIB_INCLUDE_DIRS})

pkg_check_modules(LIBSSL REQUIRED libssl)
include_directories(${LIBSSL_INCLUDE_DIRS})
string(REPLACE "." ";" LIBSSL_VERSION_STRING ${LIBSSL_VERSION})
list(GET LIBSSL_VERSION_STRING 0 LIBSSL_VERSION_MAJOR)
if(LIBSSL_VERSION_MAJOR EQUAL 1)
	message(FATAL_ERROR "NeoSurf does not support OpenSSL 1.x.x")
endif()

if(NEOSURF_BUILD_GTK3_FRONTEND)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
	include_directories(${GTK3_INCLUDE_DIRS})
endif()

if(NEOSURF_BUILD_VI_FRONTEND)
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
endif()

# Build nsgenbind at configure time
execute_process(COMMAND cmake -B ${CMAKE_BINARY_DIR}/contrib/nsgenbind ../contrib/nsgenbind -G ${CMAKE_GENERATOR} -DYACC=${YACC})
execute_process(COMMAND ${CMAKE_MAKE_PROGRAM} -C ${CMAKE_BINARY_DIR}/contrib/nsgenbind)

execute_process(COMMAND mkdir -pv ${CMAKE_BINARY_DIR}/duktape)
execute_process(COMMAND ${CMAKE_BINARY_DIR}/contrib/nsgenbind/nsgenbind -I ${CMAKE_SOURCE_DIR}/src/content/handlers/javascript/WebIDL ${CMAKE_SOURCE_DIR}/src/content/handlers/javascript/duktape/neosurf.bnd ${CMAKE_BINARY_DIR}/duktape)

aux_source_directory(${CMAKE_BINARY_DIR}/duktape DSRC)

add_library(neosurf SHARED
	content/content.c
	content/content_factory.c
	content/fetch.c
	content/hlcache.c
	content/llcache.c
	content/mimesniff.c
	content/textsearch.c
	content/urldb.c
	content/no_backing_store.c
	content/fs_backing_store.c
	content/fetchers/data.c
	content/fetchers/resource.c
	content/fetchers/curl.c
	content/fetchers/about/about.c
	content/fetchers/about/blank.c
	content/fetchers/about/certificate.c
	content/fetchers/about/chart.c
	content/fetchers/about/config.c
	content/fetchers/about/imagecache.c
	content/fetchers/about/nscolours.c
	content/fetchers/about/query.c
	content/fetchers/about/query_auth.c
	content/fetchers/about/query_fetcherror.c
	content/fetchers/about/query_privacy.c
	content/fetchers/about/query_timeout.c
	content/fetchers/file/dirlist.c
	content/fetchers/file/file.c
	content/handlers/css/css.c
	content/handlers/css/dump.c
	content/handlers/css/internal.c
	content/handlers/css/hints.c
	content/handlers/css/select.c
	content/handlers/html/box_construct.c
	content/handlers/html/box_inspect.c
	content/handlers/html/box_manipulate.c
	content/handlers/html/box_normalise.c
	content/handlers/html/box_special.c
	content/handlers/html/box_textarea.c
	content/handlers/html/css.c
	content/handlers/html/css_fetcher.c
	content/handlers/html/dom_event.c
	content/handlers/html/font.c
	content/handlers/html/form.c
	content/handlers/html/forms.c
	content/handlers/html/html.c
	content/handlers/html/imagemap.c
	content/handlers/html/interaction.c
	content/handlers/html/layout.c
	content/handlers/html/layout_flex.c
	content/handlers/html/object.c
	content/handlers/html/redraw.c
	content/handlers/html/redraw_border.c
	content/handlers/html/script.c
	content/handlers/html/table.c
	content/handlers/html/textselection.c
	content/handlers/image/image.c
	content/handlers/image/image_cache.c
	content/handlers/image/bmp.c
	content/handlers/image/gif.c
	content/handlers/image/ico.c
	content/handlers/image/jpeg.c
	content/handlers/image/png.c
	content/handlers/image/svg.c
	#content/handlers/image/video.c # requires gstreamer
	content/handlers/image/webp.c # requires libwebp
	content/handlers/javascript/fetcher.c
	content/handlers/javascript/content.c
	content/handlers/javascript/duktape/dukky.c
	content/handlers/javascript/duktape/duktape.c
	content/handlers/text/textplain.c
	desktop/cookie_manager.c
	desktop/knockout.c
	desktop/hotlist.c
	desktop/plot_style.c
	desktop/print.c
	desktop/search.c
	desktop/searchweb.c
	desktop/scrollbar.c
	desktop/textarea.c
	desktop/version.c
	desktop/system_colour.c
	desktop/local_history.c
	desktop/global_history.c
	desktop/treeview.c
	desktop/page-info.c
	desktop/browser.c
	desktop/browser_window.c
	desktop/browser_history.c
	desktop/download.c
	desktop/frames.c
	desktop/neosurf.c
	desktop/cw_helper.c
	desktop/save_complete.c
	desktop/save_text.c
	desktop/selection.c
	desktop/textinput.c
	desktop/gui_factory.c
	desktop/save_pdf.c
	desktop/bitmap.c
	utils/bloom.c
	utils/corestrings.c
	utils/file.c
	utils/filepath.c
	utils/hashmap.c
	utils/hashtable.c
	utils/idna.c
	utils/libdom.c
	utils/log.c
	utils/messages.c
	utils/nscolour.c
	utils/nsoption.c
	utils/punycode.c
	utils/ssl_certs.c
	utils/talloc.c
	utils/time.c
	utils/url.c
	utils/useragent.c
	utils/utf8.c
	utils/utils.c
	utils/http/generics.c
	utils/http/primitives.c
	utils/http/parameter.c
	utils/http/cache-control.c
	utils/http/content-disposition.c
	utils/http/content-type.c
	utils/http/strict-transport-security.c
	utils/nsurl/nsurl.c
	utils/nsurl/parse.c
	${DSRC}
)

if(NEOSURF_BUILD_VI_FRONTEND)
	add_executable(neosurf-vi
		frontends/visurf/xdg-shell.c
		frontends/visurf/xdg-decoration.c
		frontends/visurf/main.c
		frontends/visurf/bitmap.c
		frontends/visurf/commands.c
		frontends/visurf/fetch.c
		frontends/visurf/getopt.c
		frontends/visurf/keybindings.c
		frontends/visurf/layout.c
		frontends/visurf/pool-buffer.c
		frontends/visurf/plotters.c
		frontends/visurf/window.c
		frontends/visurf/settings.c
		frontends/visurf/undo.c
	)
endif()

if(NEOSURF_BUILD_GTK3_FRONTEND)
	add_executable(neosurf-gtk3
		frontends/gtk/gui.c
		frontends/gtk/misc.c
		frontends/gtk/schedule.c
		frontends/gtk/layout_pango.c
		frontends/gtk/bitmap.c
		frontends/gtk/plotters.c
		frontends/gtk/scaffolding.c
		frontends/gtk/gdk.c
		frontends/gtk/completion.c
		frontends/gtk/throbber.c
		frontends/gtk/accelerator.c
		frontends/gtk/selection.c
		frontends/gtk/window.c
		frontends/gtk/fetch.c
		frontends/gtk/download.c
		frontends/gtk/menu.c
		frontends/gtk/print.c
		frontends/gtk/search.c
		frontends/gtk/tabs.c
		frontends/gtk/toolbar.c
		frontends/gtk/compat.c
		frontends/gtk/viewdata.c
		frontends/gtk/viewsource.c
		frontends/gtk/preferences.c
		frontends/gtk/about.c
		frontends/gtk/resources.c
		frontends/gtk/corewindow.c
		frontends/gtk/local_history.c
		frontends/gtk/global_history.c
		frontends/gtk/cookies.c
		frontends/gtk/hotlist.c
		frontends/gtk/page_info.c
	)
endif()

if(NEOSURF_BUILD_XXD)
	add_executable(xxd
		xxd.c
	)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -DNDEBUG -Dvi -Dnsvi -DVISURF_RESPATH=\\\"${CMAKE_INSTALL_PREFIX}/share/neosurf-gtk3\\\" -DWITH_OPENSSL -DWITH_BMP -DWITH_GIF -DWITH_JPEG -DWITH_PNG -DWITH_WEBP -DWITH_NS_SVG -Dgtk -Dnsgtk -DGTK_RESPATH=\\\"${CMAKE_INSTALL_PREFIX}/share/neosurf-gtk3\\\" -DNEOSURF_HOMEPAGE=\\\"about:welcome\\\" -DWITH_GRESOURCE -DWITH_BUILTIN_PIXBUF -Wno-unused-parameter -Wno-deprecated-declarations -Wno-strict-prototypes -Wno-unused-but-set-variable -Wno-int-conversion -D_XOPEN_SOURCE=700 -D_DEFAULT_SOURCE")

# Note: libpsl used for detecting supercookies
set(NEOSURF_COMMON_LIBS css dom nsutils parserutils nsgif nsbmp svgtiny dom-hubbub-bindings ${LIBCRYPTO_LIBRARIES} ${LIBPNG_LIBRARIES} ${LIBCURL_LIBRARIES} ${LIBJPEG_LIBRARIES} ${LIBWEBP_LIBRARIES} ${LIBPSL_LIBRARIES} ${ZLIB_LIBRARIES} ${LIBSSL_LIBRARIES})
target_link_libraries(neosurf ${NEOSURF_COMMON_LIBS})
if(NEOSURF_BUILD_VI_FRONTEND)
	target_link_libraries(neosurf-vi neosurf ${NEOSURF_COMMON_LIBS} ${XKBCOMMON_LIBRARIES} ${CAIRO_LIBRARIES} ${WAYLAND_CURSOR_LIBRARIES} ${WAYLAND_CLIENT_LIBRARIES} ${PANGO_LIBRARIES} ${PANGOCAIRO_LIBRARIES})
endif()
if(NEOSURF_BUILD_GTK3_FRONTEND)
	target_link_libraries(neosurf-gtk3 neosurf m ${NEOSURF_COMMON_LIBS} ${GTK3_LIBRARIES})
endif()

install(TARGETS neosurf DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY resources/ DESTINATION ${CMAKE_INSTALL_PREFIX}/share/neosurf)
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

if(NEOSURF_BUILD_VI_FRONTEND)
	install(TARGETS neosurf-vi DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif()
if(NEOSURF_BUILD_GTK3_FRONTEND)
	install(TARGETS neosurf-gtk3 DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif()

if(NEOSURF_BUILD_VI_FRONTEND OR NEOSURF_BUILD_GTK3_FRONTEND) # Visurf uses the same resources as the Gtk3 frontend
	install(DIRECTORY frontends/gtk/res/ DESTINATION ${CMAKE_INSTALL_PREFIX}/share/neosurf-gtk3)
endif()

if(NEOSURF_INSTALL_NSGENBIND)
	install(FILES ${CMAKE_BINARY_DIR}/contrib/nsgenbind/nsgenbind DESTINATION ${CMAKE_INSTALL_PREFIX}/bin PERMISSIONS WORLD_EXECUTE)
endif()

if(NEOSURF_INSTALL_GEN_PARSER)
	install(FILES ${CMAKE_BINARY_DIR}/contrib/libcss/gen_parser/gen_parser DESTINATION ${CMAKE_INSTALL_PREFIX}/bin PERMISSIONS WORLD_EXECUTE)
endif()

if(NEOSURF_BUILD_XXD)
	install(TARGETS xxd DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif()

#TODO: support QuickJS as an alternative to Duktape
