# visurf

visurf is a work-in-progress frontend for Netsurf:

- vi-inspired keybindings
- Minimal, lightweight interface
- Wayland only

![](https://l.sr.ht/evhg.png)

## Building visurf

Kind of crap right now but here's the procedure I recommend:

1. Grab the latest release tarball from netsurf-browser.org
2. Remove the 'netsurf' and 'libcss' directories and replace them with this repo
   and `git://git.netsurf-browser.org/libcss.git` respectively
3. Run `make TARGET=visurf PREFIX=/usr LIBDIR=lib INCLUDEDIR=include`

This will build once. As you make changes, you can run an incremental build with
this script:

```sh
#!/bin/sh
export TARGET=visurf
export PKG_CONFIG_PATH=$(pwd)/inst-visurf/lib/pkgconfig:$PKG_CONFIG_PATH
export PATH=$PWD/inst-visurf/bin:$PATH
make --directory=netsurf PREFIX=/usr TARGET=$TARGET NETSURF_LOG_LEVEL=DEBUG
```
