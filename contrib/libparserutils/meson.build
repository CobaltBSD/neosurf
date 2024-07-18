parserutils_src = files(
  'src/input/filter.c',
  'src/input/inputstream.c',
  'src/charset/aliases.c',
  'src/charset/encodings/utf8.c',
  'src/utils/buffer.c',
  'src/utils/stack.c',
  'src/utils/vector.c'
)

parserutils = library('parserutils',
  parserutils_src,
  include_directories: include_directories('src', 'include'),
	install: true
)
