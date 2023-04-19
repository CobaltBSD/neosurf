add_library(rosprite SHARED
	src/librosprite.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -D_BSD_SOURCE -D_DEFAULT_SOURCE -I${CMAKE_SOURCE_DIR}/contrib/librosprite/include/ -I${CMAKE_SOURCE_DIR}/contrib/librosprite/src -std=c99 -Werror -pedantic -Wno-format-zero-length -Wformat-security -Wstrict-aliasing=2 -Wmissing-format-attribute -Wunused -Wformat=2 -Werror-implicit-function-declaration -Wmissing-declarations -Wmissing-prototypes")

#target_link_libraries(css)
