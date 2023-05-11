# NeoSurf - An improved fork of NetSurf

This project is a fork of Netsurf with various improvements and additions, such as [Visurf](https://sr.ht/~sircmpwn/visurf/), as well as a modernized build system.

In the long term, NeoSurf intends to be a web browser for what the web *should* be. While it aims to become a much more robust and efficient browser, certain aspects of common browsing technologies such as DRM and useless/insecure features of JavaScript will never be introduced.

For comparison, the total amount of lines of code and text (as measured by `find -print0 | xargs -0 cat | wc -l`) of Firefox (as of version 102.10.0) contains approximately 48,500,000 lines of code and text; NeoSurf, as of 0.6, contains about 457,000, or just about 0.9% of Firefox's codebase. Vanilla NetSurf (as of 3.10) contains about 1,437,000 lines. Furthermore, the text-only browsers Lynx, Links and ELinks use about 478,000, 344,000 and 527,000 lines respectively.

NeoSurf is still in very early development and has some issues; help would be appreciated.

![NeoSurf](img/scr1.png?raw=true "NeoSurf Homepage")
![GNU.org](img/scr2.png?raw=true "GNU.org")

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
```

To build with both Visurf and Gtk3 frontends:
```sh
$ cmake .. -DCMAKE_INSTALL_PREFIX=/usr
```
To build with Visurf only:
```sh
$ cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DNEOSURF_BUILD_GTK3_FRONTEND=OFF
```
To build with Gtk3 only:
```sh
$ cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DNEOSURF_BUILD_VI_FRONTEND=OFF
```
To build libneosurf only (no frontend):
```sh
$ cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DNEOSURF_BUILD_GTK3_FRONTEND=OFF -DNEOSURF_BUILD_VI_FRONTEND=OFF
```

Then:
```sh
$ make
$ sudo make install
```

At build-time, NeoSurf requires Python3, CMake, GNU make (or another CMake-compatible build utility), and a POSIX-compliant shell.

At runtime and build-time, the following libraries and their development headers are required:
* libjpeg-turbo
* libpng
* libcurl
* OpenSSL or LibreSSL
* libwebp
* libpsl
* gperf
* libxml2
* cairo (Visurf and Gtk3 only)
* pango (Visurf and Gtk3 only)
* wayland (Visurf only)
* xkbcommon (Visurf only)
* glib (Gtk3 only)
* gdk-pixbuf (Gtk3 only)
* gtk+3 (Gtk3 only)

However you obtain these utilities and libraries depends on your distribution.
