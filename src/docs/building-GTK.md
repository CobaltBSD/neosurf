Build Instructions for GTK NetSurf
==================================

This document provides instructions for building the GTK version of NetSurf
and provides guidance on obtaining NetSurf's build dependencies.

GTK NetSurf has been tested on Debian, Ubuntu, Fedora 8, FreeBSD, NetBSD and
Solaris 10.  NetSurf requires at minimum GTK 2.12.


Quick Start
-----------

See the QUICK-START document, which provides a simple environment with
which you can fetch, build and install NetSurf and its dependencies.

The QUICK-START is the recommended way to build NetSurf.


Manual building
---------------

If you can't follow the quick start instructions, you will have to build
NetSurf manually.  The instructions for doing this are given below.


Obtaining the build dependencies
--------------------------------

Many of NetSurf's dependencies are packaged on various operating systems.
The remainder must be installed manually.  Currently, some of the libraries
developed as part of the NetSurf project have not had official releases.
Hopefully they will soon be released with downloadable tarballs and packaged
in common distros.  For now, you'll have to make do with Git checkouts.

### Package installation

Debian-like OS:

    $ apt-get install libgtk2.0-dev libcurl3-dev libpng-dev 
    $ apt-get install librsvg2-dev libjpeg-dev

If you want to build with gtk 3 replace libgtk2.0-dev with libgtk-3-dev 

Recent OS versions might need libcurl4-dev instead of libcurl3-dev but
note that when it has not been built with OpenSSL, the SSL_CTX is not
available and results that certification details won't be presented in case
they are invalid.  But as this is currently unimplemented in the GTK
flavour of NetSurf, this won't make a difference at all.

Fedora:

    $ yum install curl-devel libpng-devel
    $ yum install librsvg2-devel expat-devel

Other:

You'll need to install the development resources for libglade2, libcurl3,
libpng and librsvg.


### Preparing your workspace

NetSurf has a number of libraries which must be built in-order and
installed into your workspace. Each library depends on a core build
system which NetSurf projects use. This build system relies on the
presence of things like pkg-config to find libraries and also certain
environment variables in order to work correctly.

Assuming you are preparing a workspace in /home/netsurf/workspace then
the following steps will set you up:

### Make the workspace directory and change to it

    $ mkdir -p ${HOME}/netsurf/workspace
    $ cd ${HOME}/netsurf/workspace

### Make the temporary install space

    $ mkdir inst

### Make an environment script

    $ cat > env.sh <<'EOF'
      export PKG_CONFIG_PATH=${HOME}/netsurf/workspace/inst/lib/pkgconfig::
      export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${HOME}/netsurf/workspace/inst/lib
      export PREFIX=${HOME}/netsurf/workspace/inst
      EOF

### Change to workspace and source the environment

Whenever you wish to start development in a new shell, run the following:

    $ cd ${HOME}/netsurf/workspace
    $ source env.sh

From here on, any commands in this document assume you have sourced your
shell environment.


### The NetSurf project's libraries

The NetSurf project has developed several libraries which are required by
the browser. These are:

| BuildSystem    | Shared build system, needed to build the other libraries |
| LibParserUtils | Parser building utility functions                        |
| LibWapcaplet   | String internment                                        |
| Hubbub         | HTML5 compliant HTML parser                              |
| LibCSS         | CSS parser and selection engine                          |
| LibNSGIF       | GIF format image decoder                                 |
| LibNSBMP       | BMP and ICO format image decoder                         |
| LibROSprite    | RISC OS Sprite format image decoder                      |

To fetch each of these libraries, run the appropriate commands from the
Docs/LIBRARIES file, from within your workspace directory.

To build and install these libraries, simply enter each of their directories
and run:

    $ make install

> Note:
> 
> We advise enabling iconv() support in libparserutils, which vastly
> increases the number of supported character sets.  To do this,
> create a file called Makefile.config.override in the libparserutils
> directory, containing the following line:
>
>     CFLAGS += -DWITH_ICONV_FILTER
>
> For more information, consult the libparserutils README file.

Now you should have all the NetSurf project libraries built and installed.


### Getting the NetSurf source

From your workspace directory, run the following command to get the NetSurf
source:

    $ git clone git://git.netsurf-browser.org/netsurf.git

And change to the 'netsurf' directory:

    $ cd netsurf

Building and executing NetSurf
------------------------------

First of all, you should examine the options in Makefile.defaults
and gtk/Makefile.defaults and enable and disable relevant features
as you see fit by editing a Makefile.config file.

Some of these options can be automatically detected and used, and
where this is the case they are set to such.  Others cannot be
automatically detected from the Makefile, so you will either need to
install the dependencies, or set them to NO.

You should then obtain NetSurf's dependencies, keeping in mind which options
you have enabled in the configuration file.  See the next section for
specifics.

Once done, to build GTK NetSurf on a UNIX-like platform, simply run:

    $ make

If that produces errors, you probably don't have some of NetSurf's
build dependencies installed. See "Obtaining NetSurf's dependencies"
below. Or turn off the complaining features in a Makefile.config
file. You may need to "make clean" before attempting to build after
installing the dependencies.

Run NetSurf by executing "nsgtk3":

    $ ./nsgtk3


### Builtin resources

There are numerous resources that accompany NetSurf, such as the
image files for icons, cursors and the ui builder files that
construct the browsers interface.

Some of these resources can be compiled into the browser executable
removing the need to install these resources separately. The GLib
library on which GTK is based provides this functionality to
NetSurf.

Up until GLib version 2.32 only the GDK pixbuf could be integrated
in this way and is controlled with the NETSURF_USE_INLINE_PIXBUF
variable (set in makefile.config).

Glib version 2.32 and later integrated support for any file to be a
resource while depreciating the old inline pixbuf interface. NetSurf
gtk executables can integrate many resources using this interface,
configuration is controlled with the NETSURF_USE_GRESOURCE variable.

Loading from file is the fallback if a resource has not been
compiled in, because of this if both of these features are
unavailable (or disabled) NetSurf will automatically fall back to
loading all its resources from files.

The resource initialisation within the browser ensures it can access
all the resources at start time, however it does not verify the
resources are valid so failures could still occur subsequently. This
is especially true for file based resources as they can become
inaccessible after initialisation.


Note for packagers
------------------

If you are packaging NetSurf, see the PACKAGING-GTK document.
