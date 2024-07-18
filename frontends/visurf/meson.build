visurf_src = files(
  'xdg-shell.c',
  'xdg-decoration.c',
  'main.c',
  'bitmap.c',
  'commands.c',
  'fetch.c',
  'getopt.c',
  'keybindings.c',
  'layout.c',
  'pool-buffer.c',
  'plotters.c',
  'window.c',
  'settings.c',
  'undo.c'
)

include_directories = include_directories(
  '..',
  '../../include',
  '../../contrib/libcss/include',
  '../../contrib/libdom/include',
  '../../contrib/libhubbub/include',
  '../../contrib/libnsbmp/include',
  '../../contrib/libnsgif/include',
  '../../contrib/libnsutils/include',
  '../../contrib/libparserutils/include',
  '../../contrib/libsvgtiny/include'
)

dependencies = [
  dependency('pango'),
  dependency('cairo'),
  dependency('wayland-client'),
  dependency('wayland-cursor'),
  dependency('xkbcommon'),
  dependency('pangocairo')
]

executable('neosurf-vi',
  visurf_src,
  include_directories: include_directories,
  dependencies: dependencies,
  c_args: ['-Dvi',  '-Dnsvi', '-DVISURF_RESPATH="' + (get_option('prefix') / 'share/neosurf-gtk"')],
  link_args: ['-lm'],
  link_with: [neosurf, dom, nsutils],
  install: true
)
