add_library(nsutils SHARED
	src/base64.c
	src/time.c
	src/unistd.c
	src/libwapcaplet.c
)

install(TARGETS nsutils DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
