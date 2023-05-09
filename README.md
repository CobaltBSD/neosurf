# NeoSurf - An improved fork of NetSurf

This project is a fork of Netsurf with various improvements and additions, such as Visurf, as well as a modernized build system.

In the long term, NeoSurf intends to be a web browser for what the web *should* be. While it aims to become a much more robust and efficient browser, certain aspects of common browsing technologies such as DRM and useless/insecure features of JavaScript will never be introduced.

For comparison, the total amount of lines of code and text (as measured by `find -print0 | xargs -0 cat | wc -l`) of Firefox (as of version 102.10.0) contains approximately 48,500,000 lines of code and text; NeoSurf, as of 0.6, contains about 457,000, or just about 0.9% of Firefox's codebase. Vanilla NetSurf (as of 3.10) contains about 1,437,000 lines. Furthermore, the text-only browsers Lynx, Links and ELinks use about 478,000, 344,000 and 527,000 lines respectively.

## Why?
Mainline Netsurf is fairly antiquated and is no longer suitable for browsing most websites. Furthermore, all major web browsers are labrynthine in complexity which makes auditing them realistically impossible. Neosurf aims to be a fully usable web browser that is both minimalist and easy to use and navigate with.

NeoSurf aims to be a modernized, desktop-focused browser focused on providing a minimalist browsing experience. It's mid-way between a standard web browser and the numerous text-based browsers.

## Development
Current focus is on general improvements to the codebase, as well as fixing a number of issues and bugs that were present in the most recent versions of NetSurf.

## Building and installation
In the root of the source tree:
```sh
$ mkdir -v build
$ cd build

$ cmake ..
$ make
$ sudo make install
```

Note: At the moment, the vi frontend is hardcoded to the source directory and the Gtk3 frontend requires manually copying the .ui files in src/frontends/gtk/res/gtk3 to $HOME/.config/neosurf
Both of these are temporary issues and will be fixed very soon.

At build-time, NeoSurf requires Python3, CMake, GNU make (or another CMake-compatible build utility), and a POSIX-compliant shell.

At runtime and build-time, the following libraries, which you probably already have, are also required:
libjpeg-turbo
libpng
zlib
cairo
pango
wayland
xkbcommon
glib
libcurl
OpenSSL (LibreSSL supported)
libwebp
gdk-pixbuf
gtk+3
libpsl
