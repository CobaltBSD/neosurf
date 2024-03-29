cmake_minimum_required(VERSION 3.20)
project(nsgenbind)

add_definitions(-D_POSIX_C_SOURCE=200809L)
add_compile_options(-std=c99 -Wno-void-pointer-to-int-cast -Wno-int-conversion)

execute_process(COMMAND flex -P=nsgenbind_ --outfile=${CMAKE_BINARY_DIR}/nsgenbind-lexer.c --header-file=${CMAKE_BINARY_DIR}/nsgenbind-lexer.h ${CMAKE_SOURCE_DIR}/src/nsgenbind-lexer.l RESULT_VARIABLE RET)
if(NOT RET EQUAL 0)
	message(FATAL_ERROR "flex nsgenbind failed (error ${RET})")
endif()
if(${YACC} STREQUAL bison)
	execute_process(COMMAND bison -o ${CMAKE_BINARY_DIR}/nsgenbind-parser.c -d ${CMAKE_SOURCE_DIR}/src/nsgenbind-parser.y -p nsgenbind_ RESULT_VARIABLE RET)
else()
	execute_process(COMMAND byacc -o ${CMAKE_BINARY_DIR}/nsgenbind-parser.c -d -p nsgenbind_ ${CMAKE_SOURCE_DIR}/src/nsgenbind-parser.y RESULT_VARIABLE RET)
endif()
if(NOT RET EQUAL 0)
	message(FATAL_ERROR "yacc nsgenbind failed (error ${RET})")
endif()

execute_process(COMMAND flex -P=webidl_ --outfile=${CMAKE_BINARY_DIR}/webidl-lexer.c --header-file=${CMAKE_BINARY_DIR}/webidl-lexer.h ${CMAKE_SOURCE_DIR}/src/webidl-lexer.l RESULT_VARIABLE RET)
if(NOT RET EQUAL 0)
	message(FATAL_ERROR "flex webidl failed (error ${RET})")
endif()
if(${YACC} STREQUAL bison)
	execute_process(COMMAND bison -o ${CMAKE_BINARY_DIR}/webidl-parser.c -d ${CMAKE_SOURCE_DIR}/src/webidl-parser.y -p webidl_ RESULT_VARIABLE RET)
else()
	execute_process(COMMAND byacc -o ${CMAKE_BINARY_DIR}/webidl-parser.c -d -p webidl_ ${CMAKE_SOURCE_DIR}/src/webidl-parser.y RESULT_VARIABLE RET)
endif()
if(NOT RET EQUAL 0)
	message(FATAL_ERROR "yacc webidl failed (error ${RET})")
endif()

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

include_directories(
	${CMAKE_SOURCE_DIR}/contrib/nsgenbind/include
	${CMAKE_SOURCE_DIR}/contrib/nsgenbind/src
	${CMAKE_SOURCE_DIR}/src
)
