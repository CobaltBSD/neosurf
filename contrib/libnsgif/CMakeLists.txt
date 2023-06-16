add_library(nsgif SHARED
	src/gif.c
	src/lzw.c
)
set_target_properties(nsgif PROPERTIES SOVERSION ${NEOSURF_ABI})

install(TARGETS nsgif DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES include/nsgif.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
