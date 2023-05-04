add_library(nspsl SHARED
	src/nspsl.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -D_BSD_SOURCE -D_DEFAULT_SOURCE -I${CMAKE_SOURCE_DIR}/contrib/libnspsl/include/ -I${CMAKE_SOURCE_DIR}/contrib/libnspsl/src -std=c99 -Werror -D_POSIX_C_SOURCE=200809L")

#target_link_libraries(css)

install(TARGETS nspsl DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
