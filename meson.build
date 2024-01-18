project('neosurf',
        'c',
        license: ['GPL2'],
        version: files('neosurf_version')
)

add_project_arguments('-DNEOSURF_VERSION=' + meson.project_version(), language: 'c')

bld_dir = meson.project_build_root()
src_dir = meson.project_source_root()

subdir('contrib')

#subdir('src')
