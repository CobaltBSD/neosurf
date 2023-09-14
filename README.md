# NeoSurf

This project is a fork of Netsurf with various privacy-focused improvements and additions, and a revamped build system.

## Why?
NeoSurf has a different development vision from NetSurf.

![NeoSurf](img/scr1.png?raw=true "NeoSurf Homepage")
![GNU.org](img/scr2.png?raw=true "GNU.org")

## Development
Current focus is on general improvements to the codebase, as well as fixing a number of issues and bugs that were present in the most recent versions of NetSurf.

## Biggest differences from NetSurf
* [Visurf support](https://sr.ht/~sircmpwn/visurf/), with various upstream improvements and fixes
* Various upstream improvements and UI enhancements to Gtk frontend
* Removed compatibility for super old and/or obscure libraries/software/operating systems
* Dedicated LibreSSL support
* Numerous privacy improvements
* Rewritten build system
* Simplified frontend development

## Known Issues
At the moment preferences on Gtk frontend do not take effect, and as such this menu is currently disabled. This issue is inherited from NetSurf and will be amended soon.

## Building and installation
To build with both Visurf and Gtk frontends:
```sh
$ cmake -B build -DCMAKE_INSTALL_PREFIX=/usr
```
To build with Visurf only:
```sh
$ cmake -B build -DCMAKE_INSTALL_PREFIX=/usr -DNEOSURF_BUILD_GTK_FRONTEND=OFF
```
To build with Gtk only:
```sh
$ cmake -B build -DCMAKE_INSTALL_PREFIX=/usr -DNEOSURF_BUILD_VI_FRONTEND=OFF
```
To build libneosurf only (no frontend):
```sh
$ cmake -B build -DCMAKE_INSTALL_PREFIX=/usr -DNEOSURF_BUILD_GTK_FRONTEND=OFF -DNEOSURF_BUILD_VI_FRONTEND=OFF
```

Then:
```sh
$ make -C build
```

And as root:
```sh
# make -C build install
```

NeoSurf is geared towards the Cobalt operating system, but is intended to remaining portable on any Linux or BSD-based platforms.

At build-time, NeoSurf requires the following programs:
* python3
* cmake
* any CMake-compatible build utility (typically GNU make)
* Z Shell
* gperf
* flex
* bison or byacc
* pkg-config or pkgconf

At runtime and build-time, the following libraries and their development headers are required:
* libcurl
* OpenSSL or LibreSSL (libcrypto, libssl)
* libpsl
* libxml2
* libjpeg, libjpeg-turbo or mozjpeg (optional)
* libpng (optional)
* libwebp (optional)
* cairo (Visurf and Gtk only)
* pango (Visurf and Gtk only)
* wayland (Visurf only)
* xkbcommon (Visurf only)
* gtk+ (Gtk only)

However you obtain these utilities and libraries depends on your distribution.

## Using neosurf-vi
### Keybindings
Ctrl+q, Ctrl+w - Close tab

i - Toggle insert mode (for text boxes)

j, Down - Scroll down

k, Up - Scroll up

h, Left - Scroll left

l, Right - Scroll right

H - Previous page

L - Next page

r - Reload

R - Force reload

o - Shorthand for `:open`

O - Same as above, but opens in a new tab

Ctrl+t - New tab

go - Shorthand for `:open`, but automatically adds the current URL

gO - Same as above, but will open in a new tab

Ctrl+d - Scroll down fast

Ctrl+u - Scroll up fast

gg, Home - Jump to top

G, End - Jump to bottom

Space, Next - Scroll down even faster

Prior - Scroll up even faster

\+ - Zoom in

\- - Zoom out

= - Reset zoom

gt, J, Ctrl+Next - Next tab

gT, K, Ctrl+Prior - Previous tab

f, F - Different variants of follow mode

yy, yt, Yy, Yt, Ctrl+c - Different variants of yank

pp, Pp, pP, PP - Different variants of paste

u - Undo

/ - Find occurences of some text

n - Next item in search

N - Previous item in search

[[ - Previous page

]] - Next page

### Commands
:back

:bind

:close

:exec

:exline

:follow

:forward

:fullscreen

:insert

:open

:page

:paste

:quit

:reload

:scroll

:search

:set

:source

:stop

:tab

:tabmove

:unbind

:undo

:yank

:zoom
