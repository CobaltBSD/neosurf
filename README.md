# NeoSurf

This project is a fork of Netsurf with various improvements and additions, and a revamped build system.

## Why?

NeoSurf has different development goals than NetSurf. It aims to emphasize privacy while introducing a look and feel of the modern web. NeoSurf also streamlines the process of developing new frontends: You can simply use NeoSurf's shared library and headers rather than needing to add the frontend to the source tree.

NeoSurf is still in very early development and has some issues; help would be appreciated.

![NeoSurf](img/scr1.png?raw=true "NeoSurf Homepage")
![GNU.org](img/scr2.png?raw=true "GNU.org")

## Development
Current focus is on general improvements to the codebase, as well as fixing a number of issues and bugs that were present in the most recent versions of NetSurf.

## Known Issues
At the moment preferences on Gtk3 frontend do not take effect, and as such this menu is currently disabled. NeoSurf also has a number of memory issues and unpredictable behavior which doesn't directly impact the user experience in any direct way, but are highly problematic nonetheless. Both of these issues are inherited from NetSurf and are slowly being stamped out.

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
* libjpeg-turbo (or mozjpeg)
* libpng
* libcurl
* OpenSSL or LibreSSL (libcrypto, libssl)
* libwebp
* libpsl
* gperf
* libxml2
* flex
* bison or byacc
* pkg-config or pkgconf
* cairo (Visurf and Gtk3 only)
* pango (Visurf and Gtk3 only)
* wayland (Visurf only)
* xkbcommon (Visurf only)
* gtk+3 (Gtk3 only)

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
