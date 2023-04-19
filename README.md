# NeoSurf - An improved fork of NetSurf

This project is a fork of Netsurf with various improvements and additions, such as Visurf, as well as a modernized build system.

The current build system is a bit of a hack job at the moment. It will be improved with time.

In the near term, the next major priority is to swap out neosurf's gstreamer-based video handler with libmpv or ffmpeg.

In the long term, NeoSurf intends to be a web browser for what the web *should* be. While it aims to become a much more robust and efficient browser, certain aspects of common browsing technologies such as DRM and useless/insecure features of JavaScript will never be introduced.

For comparison, the total amount of lines of code and text (as measured by `find -print0 | xargs -0 cat | wc -l`) as of version 0.3 totals at roughly 706,000, but this is expected to decrease as further improvements are made to the build system and code base. Firefox (as of version 102.10.0) contains approximately 48,500,000 lines of code and text.
