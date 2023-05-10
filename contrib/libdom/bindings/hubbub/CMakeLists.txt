cmake_policy(SET CMP0079 NEW)

add_library(dom-hubbub-bindings SHARED
	parser.c
)

target_link_libraries(dom-hubbub-bindings nsutils hubbub dom)

install(TARGETS dom-hubbub-bindings DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${CMAKE_SOURCE_DIR}/contrib/libdom/src")
