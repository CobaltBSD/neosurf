add_library(nsbmp SHARED
	src/libnsbmp.c
)

install(TARGETS nsbmp DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
