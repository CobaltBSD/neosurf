add_library(nsbmp SHARED
	src/libnsbmp.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -D_BSD_SOURCE -D_DEFAULT_SOURCE -I${CMAKE_SOURCE_DIR}/contrib/libnsbmp/include/ -I${CMAKE_SOURCE_DIR}/contrib/libnsbmp/src -std=c99 -Werror -pedantic")

#target_link_libraries(css)
