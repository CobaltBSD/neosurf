add_library(hubbub SHARED
	src/parser.c
	src/charset/detect.c
	src/tokeniser/entities.c
	src/tokeniser/tokeniser.c
	src/treebuilder/treebuilder.c
	src/treebuilder/initial.c
	src/treebuilder/before_html.c
	src/treebuilder/before_head.c
	src/treebuilder/in_head.c
	src/treebuilder/in_head_noscript.c
	src/treebuilder/after_head.c
	src/treebuilder/in_body.c
	src/treebuilder/in_table.c
	src/treebuilder/in_caption.c
	src/treebuilder/in_column_group.c
	src/treebuilder/in_table_body.c
	src/treebuilder/in_row.c
	src/treebuilder/in_cell.c
	src/treebuilder/in_select.c
	src/treebuilder/in_select_in_table.c
	src/treebuilder/in_foreign_content.c
	src/treebuilder/after_body.c
	src/treebuilder/in_frameset.c
	src/treebuilder/after_frameset.c
	src/treebuilder/after_after_body.c
	src/treebuilder/after_after_frameset.c
	src/treebuilder/generic_rcdata.c
	src/utils/errors.c
	src/utils/string.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -D_BSD_SOURCE -D_DEFAULT_SOURCE -I${CMAKE_SOURCE_DIR}/contrib/libhubbub/include/ -I${CMAKE_SOURCE_DIR}/contrib/libhubbub/src -std=c99 -I${CMAKE_SOURCE_DIR}/contrib/libparserutils/include -Werror -pedantic")

target_link_libraries(hubbub parserutils)
