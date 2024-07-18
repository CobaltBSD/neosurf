neosurf_gtk_src = files(
    'gui.c',
	'misc.c',
	'schedule.c',
	'layout_pango.c',
	'bitmap.c',
	'plotters.c',
	'scaffolding.c',
	'gdk.c',
	'completion.c',
	'throbber.c',
	'accelerator.c',
	'selection.c',
	'window.c',
	'fetch.c',
	'download.c',
	'menu.c',
	'print.c',
	'search.c',
	'tabs.c',
	'toolbar.c',
	'compat.c',
	'viewdata.c',
	'viewsource.c',
	'preferences.c',
	'about.c',
	'resources.c',
	'corewindow.c',
	'local_history.c',
	'global_history.c',
	'cookies.c',
	'hotlist.c',
	'page_info.c'
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
    dependency('gtk+-3.0')
]

executable('neosurf-gtk',
    neosurf_gtk_src,
    include_directories: include_directories,
    dependencies: dependencies,
    c_args: ['-Dgtk',  '-Dnsgtk', '-DGTK_RESPATH="' + (get_option('prefix') / 'share/neosurf-gtk"'), '-DNEOSURF_HOMEPAGE="about:welcome"'],
    link_args: ['-lm'],
    link_with: [neosurf],
	install: true
)
