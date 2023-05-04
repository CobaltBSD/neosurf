cmake_minimum_required(VERSION 3.20)
project(gen_parser)

add_executable(gen_parser # Add install option/rule TODO
	css_property_parser_gen.c
)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -D_BSD_SOURCE -D_DEFAULT_SOURCE -I${CMAKE_SOURCE_DIR}/contrib/nsgenbind/include/ -I${CMAKE_SOURCE_DIR}/contrib/nsgenbind/src -std=c99 -Werror -D_POSIX_C_SOURCE=200809L -DYYENABLE_NLS=0 -Wno-void-pointer-to-enum-cast -Wno-unused-but-set-variable")

#target_link_libraries(css)
