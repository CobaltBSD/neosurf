add_library(nsbmp SHARED
	src/libnsbmp.c
)
set_target_properties(nsbmp PROPERTIES SOVERSION ${NEOSURF_ABI})

install(TARGETS nsbmp DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})
