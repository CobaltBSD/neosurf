add_library(nsbmp SHARED
	src/libnsbmp.c
)
set_target_properties(nsbmp PROPERTIES SOVERSION ${NEOSURF_ABI})

install(TARGETS nsbmp DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES include/libnsbmp.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
