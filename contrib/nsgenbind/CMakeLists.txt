cmake_minimum_required(VERSION 3.20)
project(nsgenbind)

execute_process(COMMAND flex -P=nsgenbind_ --outfile=${CMAKE_BINARY_DIR}/nsgenbind-lexer.c --header-file=${CMAKE_BINARY_DIR}/nsgenbind-lexer.h ${CMAKE_SOURCE_DIR}/src/nsgenbind-lexer.l)
execute_process(COMMAND bison --output=${CMAKE_BINARY_DIR}/nsgenbind-parser.c --header=${CMAKE_BINARY_DIR}/nsgenbind-parser.h ${CMAKE_SOURCE_DIR}/src/nsgenbind-parser.y -p nsgenbind_)

execute_process(COMMAND flex -P=webidl_ --outfile=${CMAKE_BINARY_DIR}/webidl-lexer.c --header-file=${CMAKE_BINARY_DIR}/webidl-lexer.h ${CMAKE_SOURCE_DIR}/src/webidl-lexer.l)
execute_process(COMMAND bison --output=${CMAKE_BINARY_DIR}/webidl-parser.c --header=${CMAKE_BINARY_DIR}/webidl-parser.h ${CMAKE_SOURCE_DIR}/src/webidl-parser.y -p webidl_)

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
	${CMAKE_BINARY_DIR}/nsgenbind-parser.c
	${CMAKE_BINARY_DIR}/nsgenbind-lexer.c
	${CMAKE_BINARY_DIR}/webidl-parser.c
	${CMAKE_BINARY_DIR}/webidl-lexer.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -D_BSD_SOURCE -D_DEFAULT_SOURCE -I${CMAKE_SOURCE_DIR}/contrib/nsgenbind/include/ -I${CMAKE_SOURCE_DIR}/contrib/nsgenbind/src -std=c99 -Werror -D_POSIX_C_SOURCE=200809L -DYYENABLE_NLS=0 -Wno-void-pointer-to-enum-cast -Wno-unused-but-set-variable -I${CMAKE_SOURCE_DIR}/src")
