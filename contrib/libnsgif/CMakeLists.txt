add_library(nsgif SHARED
	src/gif.c
	src/lzw.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -I${CMAKE_SOURCE_DIR}/contrib/libnsgif/include/ -I${CMAKE_SOURCE_DIR}/contrib/libnsgif/src -std=c99 -Werror -pedantic")

install(TARGETS nsgif DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
