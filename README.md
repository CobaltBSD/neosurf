# NeoSurf - An improved fork of NetSurf

This project is a fork of Netsurf with various improvements and additions, such as Visurf, as well as a modernized build system.

The current build system is a bit of a hack job at the moment. It will be improved with time.

In the near term, the next major priority is to swap out neosurf's gstreamer-based video handler with libmpv or ffmpeg.

In the long term, NeoSurf intends to be a web browser for what the web *should* be. While it aims to become a much more robust and efficient browser, certain aspects of common browsing technologies such as DRM and useless/insecure features of JavaScript will never be introduced.

For comparison, the total amount of lines of code and text (as measured by `find -print0 | xargs -0 cat | wc -l`) of Firefox (as of version 102.10.0) contains approximately 48,500,000 lines of code and text; Neosurf, as of 0.4, contains about 561,000, or just about 1.1% of Firefox's codebase. Vanilla Netsurf (as of 3.10) contains about 1,437,000 lines. Furthermore, the text-only browsers Lynx, Links and ELinks use about 478,000, 344,000 and 527,000 lines respectively.

## Why?
Mainline Netsurf is fairly antiquated and is no longer suitable for browsing most websites. Furthermore, all major web browsers are labrynthine in complexity which makes auditing them realistically impossible. Neosurf aims to be a fully usable web browser that is both minimalist and easy to use and navigate with.
