add_executable(nsgenbind
	src/nsgenbind.c
	src/utils.c
	src/output.c
	src/webidl-ast.c
	src/nsgenbind-ast.c
	src/ir.c
	src/duk-libdom.c
	src/duk-libdom-interface.c
	src/duk-libdom-dictionary.c
	src/duk-libdom-common.c
	src/duk-libdom-generated.c

	# Generated dynamically
	src/nsgenbind-parser.c
	src/nsgenbind-lexer.c
	src/webidl-parser.c
	src/webidl-lexer.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -D_BSD_SOURCE -D_DEFAULT_SOURCE -I${CMAKE_SOURCE_DIR}/contrib/nsgenbind/include/ -I${CMAKE_SOURCE_DIR}/contrib/nsgenbind/src -std=c99 -Werror -D_POSIX_C_SOURCE=200809L -DYYENABLE_NLS=0 -Wno-void-pointer-to-enum-cast -Wno-unused-but-set-variable")

#target_link_libraries(css)
