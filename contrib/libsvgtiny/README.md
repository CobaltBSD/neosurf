# Libsvgtiny

Libsvgtiny is a library for parsing SVG files for display.

The overall idea of the library is to take some SVG as input, and return a list of paths and texts which can be rendered easily. The library does not do the actual rendering.

All supported SVG objects, for example circles, lines, and gradient filled shapes, are converted to flat-filled paths or a fragment of text, and all coordinates are converted, transformed etc. to pixels.

Libsvgtiny is Licensed under the MIT License,
http://opensource.org/licenses/mit-license.php

Written by James Bursa <james@netsurf-browser.org>.

## SVG support

Libsvgtiny is initially aiming to implement SVG Tiny, as defined in http://www.w3.org/TR/SVGMobile/.

SVG Tiny elements supported: defs, g, svg, circle, line, path, polygon, polyline, rect, text

SVG Tiny elements not yet supported: desc, metadata, title, use, a, switch, ellipse, image, font, font-face, font-face-name, font-face-src, glyph, hkern, missing-glyph, animate, animateColor, animateMotion, animateTransform, mpath, set, foreignObject

Additional elements supported: linearGradient, stop

Text support is incomplete.

The style attribute is supported.

## Using libsvgtiny

The interface is in the header svgtiny.h

```c
  #include <svgtiny.h>
```

First create a `svgtiny_diagram` using `svgtiny_create()`:

```c
  struct svgtiny_diagram *diagram;
  diagram = svgtiny_create();
```

This will return a pointer to a new diagram, or NULL if there was not enough memory.

SVGs are parsed from memory using `svgtiny_parse()`:

```c
  svgtiny_code code;
  code = svgtiny_parse(diagram, buffer, size, url, 1000, 1000);
```

The arguments are the pointer returned by `svgtiny_create()`, a buffer containing the SVG data, the size of the SVG in bytes, the url that the SVG came from, and the target viewport width and height in pixels.

The function returns `svgtiny_OK` if there were no problems, and diagram is updated. The diagram can then be rendered by looping through the array `diagram->shape[0..diagram->shape_count]`:

```c
  for (unsigned int i = 0; i != diagram->shape_count; i++) {
```

Path shapes have a non-NULL path pointer. The path is an array of floats of length `path_length`. The array contains segment type codes followed by 0 to 3 pairs of coordinates (depending on the segment type):
* `svgtiny_PATH_MOVE x y`
* `svgtiny_PATH_CLOSE`
* `svgtiny_PATH_LINE x y`
* `svgtiny_PATH_BEZIER x1 y1 x2 y2 x3 y3`

A path always starts with a MOVE.

The fill and stroke attributes give the colors of the path, or `svgtiny_TRANSPARENT` if the path is not filled or stroked. Colors are in 0xRRGGBB format (except when compiled for RISC OS). The macros `svgtiny_RED`, `svgtiny_GREEN`, and `svgtiny_BLUE` can be used to extract the components.

The width of the path is in `stroke_width`.

Text shapes have a NULL path pointer and a non-NULL text pointer. Text is in UTF-8. The coordinates of the text are in `text_x`, `text_y`. Text colors and stroke width are as for paths.

If memory runs out during parsing, `svgtiny_parse()` returns `svgtiny_OUT_OF_MEMORY`, but the diagram is still valid up to the point when memory was exhausted, and may safely be rendered.

If there is an error in the SVG (for example, an element is missing an attribute required by the specification), `svgtiny_SVG_ERROR` is returned, but the diagram is still valid up to the point of the error. The error is recorded in `diagram->error_message` and the line that caused it in `diagram->error_line`.

`svgtiny_LIBXML_ERROR` indicates that parsing the XML failed. The returned diagram will contain no shapes. `svgtiny_NOT_SVG` means that the XML did not contain a top-level `<svg>` element.

To free memory used by a diagram, use `svgtiny_free()`:

```c
  svgtiny_free(diagram);
```

For an example, see svgtiny\_test.c.


[1] http://upload.wikimedia.org/wikipedia/commons/f/fd/Ghostscript\_Tiger.svg
