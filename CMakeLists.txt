cmake_minimum_required(VERSION 3.20)
project(neosurf)

#NOTE: The build system right now is a low quality and hastily written mess. It will be reworked soon.

#TODO need to install in $HOME/.config/neosurf: src/resources/icons, src/frontends/gtk/res/throbber, src/resources/en/*, src/resources/{default,adblock,internal,quirks}.css, src/resources/neosurf.png, src/frontends/gtk/res/default.ico, src/frontends/gtk/res/languages, src/frontends/gtk/res/accelerators

#TODO: appimage building support

add_subdirectory(contrib)
add_subdirectory(src)

option(NEOSURF_INSTALL_NSGENBIND "Installs the nsgenbind utility with neosurf" OFF)
option(NEOSURF_INSTALL_GEN_PARSER "Installs the libcss gen_parser utility with neosurf" OFF)
option(NEOSURF_BUILD_XXD "Build and install a complimentary xxd implementation" OFF)
