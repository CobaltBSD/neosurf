#ifndef NETSURF_VI_LAYOUT_H
#define NETSURF_VI_LAYOUT_H
#include <stdbool.h>
#include <pango/pango.h>

struct plot_font_style;

nserror nsfont_paint(int x, int y, const char *string, size_t length, const struct plot_font_style *fstyle);

/**
 * Convert a plot style to a PangoFontDescription.
 *
 * \param fstyle plot style for this text
 * \return A new Pango font description
 */
PangoFontDescription *nsfont_style_to_description(const struct plot_font_style *fstyle);


void pango_printf(cairo_t *cairo, const char *font, const char *fmt, ...);
void get_text_size(cairo_t *cairo, const char *font, int *width, int *height,
		int *baseline, const char *fmt, ...);

struct gui_layout_table;
extern struct gui_layout_table vi_layout_table;

#endif
