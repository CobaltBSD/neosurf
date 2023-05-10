add_library(nsutils SHARED
	src/base64.c
	src/time.c
	src/unistd.c
	src/libwapcaplet.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -D_BSD_SOURCE -D_DEFAULT_SOURCE -I${CMAKE_SOURCE_DIR}/contrib/libnsutils/include -I${CMAKE_SOURCE_DIR}/contrib/libnsutils/src -std=c99 -Werror -D_POSIX_C_SOURCE=200809L -DNDEBUG")

install(TARGETS nsutils DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
