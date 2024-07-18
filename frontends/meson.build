if get_option('NEOSURF_BUILD_GTK_FRONTEND')
    subdir('gtk')
endif

if get_option('NEOSURF_BUILD_VI_FRONTEND')
    subdir('visurf')
endif

install_subdir('gtk/res',
    install_dir: 'share/neosurf-gtk',
    strip_directory: true
)

install_subdir('../src/resources',
    install_dir: 'share/neosurf-gtk',
    strip_directory: true
)
