add_library(nsutils SHARED
	src/base64.c
	src/time.c
	src/unistd.c
	src/libwapcaplet.c
)
set_target_properties(nsutils PROPERTIES SOVERSION ${NEOSURF_ABI})

install(TARGETS nsutils DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/libwapcaplet DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/nsutils DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
