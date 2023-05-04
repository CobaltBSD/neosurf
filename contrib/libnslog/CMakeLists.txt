set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -D_BSD_SOURCE -D_DEFAULT_SOURCE -I${CMAKE_SOURCE_DIR}/contrib/libnslog/include/ -I${CMAKE_SOURCE_DIR}/contrib/libnslog/src -std=c99 -Werror -D_POSIX_C_SOURCE=200809L -g")

add_custom_target(
	yacc_target
	COMMAND yacc -d -t --output=${CMAKE_SOURCE_DIR}/filter-parser.c --defines=${CMAKE_SOURCE_DIR}/filter-parser.h ${CMAKE_SOURCE_DIR}/src/filter-parser.y
	COMMENT "Generating parser"
)

add_custom_target(
	lex_target
	COMMAND lex --outfile=${CMAKE_SOURCE_DIR}/filter-lexer.c --header-file=${CMAKE_SOURCE_DIR}/filter-lexer.h ${CMAKE_SOURCE_DIR}/src/filter-lexer.l
	COMMENT "Generating lexer"
)

add_library(nslog SHARED
	src/core.c
	src/filter.c
	src/filter-parser.c
	src/filter-lexer.c
)

add_dependencies(nslog
	yacc_target
	lex_target
)

#target_link_libraries(css)

install(TARGETS nslog DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
