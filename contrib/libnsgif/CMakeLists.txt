add_library(nsgif SHARED
	src/gif.c
	src/lzw.c
)

install(TARGETS nsgif DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
