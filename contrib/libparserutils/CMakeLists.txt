add_library(parserutils SHARED
	src/input/filter.c
	src/input/inputstream.c
	src/charset/aliases.c
	src/charset/codec.c
	src/charset/codecs/codec_ascii.c
	src/charset/codecs/codec_8859.c
	src/charset/codecs/codec_ext8.c
	src/charset/codecs/codec_utf8.c
	src/charset/codecs/codec_utf16.c
	src/charset/encodings/utf8.c
	src/charset/encodings/utf16.c
	src/utils/buffer.c
	src/utils/errors.c
	src/utils/stack.c
	src/utils/vector.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -D_BSD_SOURCE -D_DEFAULT_SOURCE -I${CMAKE_SOURCE_DIR}/contrib/libparserutils/include/ -I${CMAKE_SOURCE_DIR}/contrib/libparserutils/src -std=c99 -Werror -pedantic")

#target_link_libraries(css)
