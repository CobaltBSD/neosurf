add_library(parserutils SHARED
	src/input/filter.c
	src/input/inputstream.c
	src/charset/aliases.c
	src/charset/encodings/utf8.c
	src/utils/buffer.c
	src/utils/stack.c
	src/utils/vector.c
)
set_target_properties(parserutils PROPERTIES SOVERSION ${NEOSURF_ABI})

install(TARGETS parserutils DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

include_directories(${CMAKE_SOURCE_DIR}/contrib/libparserutils/src)

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})
