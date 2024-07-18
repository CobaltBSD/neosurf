nsutils_src = files(
  'src/base64.c',
  'src/time.c',
  'src/unistd.c',
  'src/libwapcaplet.c'
)

nsutils = library('nsutils',
  nsutils_src,
  include_directories: include_directories('include'),
	install: true
)
