add_library(wapcaplet SHARED
	src/libwapcaplet.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -D_BSD_SOURCE -D_DEFAULT_SOURCE -I${CMAKE_SOURCE_DIR}/contrib/libwapcaplet/include/ -I${CMAKE_SOURCE_DIR}/contrib/libwapcaplet/src -std=c99 -Werror")

#target_link_libraries(css)

install(TARGETS wapcaplet DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
