add_library(parserutils SHARED
	src/input/filter.c
	src/input/inputstream.c
	src/charset/aliases.c
	src/charset/encodings/utf8.c
	src/utils/buffer.c
	src/utils/stack.c
	src/utils/vector.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -D_BSD_SOURCE -D_DEFAULT_SOURCE -I${CMAKE_SOURCE_DIR}/contrib/libparserutils/include/ -I${CMAKE_SOURCE_DIR}/contrib/libparserutils/src -std=c99 -Werror -pedantic -DNDEBUG")

install(TARGETS parserutils DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
