#ifndef NETSURF_VI_BITMAP_H
#define NETSURF_VI_BITMAP_H

#include <cairo.h>

struct gui_bitmap_table;
extern struct gui_bitmap_table vi_bitmap_table;

struct bitmap {
	cairo_surface_t *surface;
	cairo_surface_t *scsurface;
	bool converted;
};

int nsvi_bitmap_get_width(void *vbitmap);
int nsvi_bitmap_get_height(void *vbitmap);

#endif
