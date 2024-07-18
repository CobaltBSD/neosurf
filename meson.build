project('neosurf',
   'c',
  license: ['GPL-2.0-only'],
  version: files('neosurf_version')
)

add_project_arguments(
  '-DNEOSURF_VERSION=' + meson.project_version(),
  '-DWITH_OPENSSL',
  '-DWITH_BMP',
  '-DWITH_GIF',
  '-DWITH_NS_SVG',
  '-DWITH_PNG',
  '-DWITH_JPEG',
  '-DWITH_WEBP',
  language: 'c')

if get_option('buildtype') != 'debug'
  add_project_arguments(
    '-DNDEBUG',
    language: 'c'
  )
endif

subdir('contrib')
subdir('src')
subdir('frontends')
