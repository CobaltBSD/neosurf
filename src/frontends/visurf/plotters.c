/*
 * Copyright 2006 Rob Kendrick <rjek@rjek.com>
 * Copyright 2005 James Bursa <bursa@users.sourceforge.net>
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
// TODO: Generalize to reduce code dupe between gtk and visurf

#include <math.h>
#include <assert.h>
#include <cairo.h>

#include "utils/log.h"
#include "netsurf/plotters.h"
#include "netsurf/types.h"
#include "utils/nsoption.h"
#include "visurf/bitmap.h"
#include "visurf/layout.h"
#include "visurf/plotters.h"
#include "visurf/pool-buffer.h"
#include "visurf/window.h"

static rect cliprect;

void nsvi_set_colour(colour c)
{
	cairo_set_source_rgba(activebuffer->cairo, 
			      (c & 0xff) / 255.0, 
			      ((c & 0xff00) >> 8) / 255.0, 
			      ((c & 0xff0000) >> 16) / 255.0, 
			      1.0);
}

static inline void nsvi_set_solid(void)
{
	double dashes = 0;
	cairo_set_dash(activebuffer->cairo, &dashes, 0, 0);
}

static inline void nsvi_set_dotted(void)
{
	double cdashes[] = { 1.0, 2.0 };
	cairo_set_dash(activebuffer->cairo, cdashes, 2, 0);
}

static inline void nsvi_set_dashed(void)
{
	double cdashes[] = { 8.0, 2.0 };
	cairo_set_dash(activebuffer->cairo, cdashes, 2, 0);
}

static inline void nsvi_set_line_width(plot_style_fixed width)
{
	if (width == 0) {
		cairo_set_line_width(activebuffer->cairo, 1);
	} else {
		cairo_set_line_width(activebuffer->cairo,
				plot_style_fixed_to_double(width));
	}
}

static nserror
nsvi_plot_clip(const struct redraw_context *ctx, const struct rect *clip)
{
	cairo_reset_clip(activebuffer->cairo);
	cairo_rectangle(activebuffer->cairo, clip->x0, clip->y0,
			clip->x1 - clip->x0, clip->y1 - clip->y0);
	cairo_clip(activebuffer->cairo);

	cliprect = *clip;
	return NSERROR_OK;
}

static nserror
nsvi_plot_arc(const struct redraw_context *ctx,
	       const plot_style_t *style,
	       int x, int y, int radius, int angle1, int angle2)
{
	nsvi_set_colour(style->fill_colour);
	nsvi_set_solid();

	cairo_set_line_width(activebuffer->cairo, 1);
	cairo_arc(activebuffer->cairo, x, y, radius,
		  (angle1 + 90) * (M_PI / 180),
		  (angle2 + 90) * (M_PI / 180));
	cairo_stroke(activebuffer->cairo);

	return NSERROR_OK;
}

static nserror
nsvi_plot_disc(const struct redraw_context *ctx,
		const plot_style_t *style,
		int x, int y, int radius)
{
	if (style->fill_type != PLOT_OP_TYPE_NONE) {
		nsvi_set_colour(style->fill_colour);
		nsvi_set_solid();
		cairo_set_line_width(activebuffer->cairo, 0);
		cairo_arc(activebuffer->cairo, x, y, radius, 0, M_PI * 2);
		cairo_fill(activebuffer->cairo);
		cairo_stroke(activebuffer->cairo);
	}

	if (style->stroke_type != PLOT_OP_TYPE_NONE) {
		nsvi_set_colour(style->stroke_colour);

		switch (style->stroke_type) {
		case PLOT_OP_TYPE_SOLID: /* Solid colour */
		default:
			nsvi_set_solid();
			break;

		case PLOT_OP_TYPE_DOT: /* Doted plot */
			nsvi_set_dotted();
			break;

		case PLOT_OP_TYPE_DASH: /* dashed plot */
			nsvi_set_dashed();
			break;
		}

		nsvi_set_line_width(style->stroke_width);

		cairo_arc(activebuffer->cairo, x, y, radius, 0, M_PI * 2);

		cairo_stroke(activebuffer->cairo);
	}

	return NSERROR_OK;
}

static nserror
nsvi_plot_line(const struct redraw_context *ctx,
		const plot_style_t *style,
		const struct rect *line)
{
	nsvi_set_colour(style->stroke_colour);

	switch (style->stroke_type) {
	case PLOT_OP_TYPE_SOLID: /* Solid colour */
	default:
		nsvi_set_solid();
		break;

	case PLOT_OP_TYPE_DOT: /* Doted plot */
		nsvi_set_dotted();
		break;

	case PLOT_OP_TYPE_DASH: /* dashed plot */
		nsvi_set_dashed();
		break;
	}

	if (style->stroke_type != PLOT_OP_TYPE_NONE) {
		nsvi_set_colour(style->stroke_colour);
	}

	nsvi_set_line_width(style->stroke_width);

	/* core expects horizontal and vertical lines to be on pixels, not
	 * between pixels
	 */
	cairo_move_to(activebuffer->cairo,
		      (line->x0 == line->x1) ? line->x0 + 0.5 : line->x0,
		      (line->y0 == line->y1) ? line->y0 + 0.5 : line->y0);
	cairo_line_to(activebuffer->cairo,
		      (line->x0 == line->x1) ? line->x1 + 0.5 : line->x1,
		      (line->y0 == line->y1) ? line->y1 + 0.5 : line->y1);
	cairo_stroke(activebuffer->cairo);

	return NSERROR_OK;
}

void nsvi_plot_caret(int x, int y, int h)
{
	nsvi_set_solid(); /* solid line */
	nsvi_set_colour(0); /* black */
	cairo_set_line_width(activebuffer->cairo, 1); /* thin line */

	/* core expects horizontal and vertical lines to be on pixels, not
	 * between pixels */
	cairo_move_to(activebuffer->cairo, x + 0.5, y);
	cairo_line_to(activebuffer->cairo, x + 0.5, y + h - 1);
	cairo_stroke(activebuffer->cairo);
}

static nserror
nsvi_plot_rectangle(const struct redraw_context *ctx,
		     const plot_style_t *style,
		     const struct rect *rect)
{
	if (style->fill_type != PLOT_OP_TYPE_NONE) {
		nsvi_set_colour(style->fill_colour);
		nsvi_set_solid();

		cairo_set_line_width(activebuffer->cairo, 0);
		cairo_rectangle(activebuffer->cairo,
				rect->x0,
				rect->y0,
				rect->x1 - rect->x0,
				rect->y1 - rect->y0);
		cairo_fill(activebuffer->cairo);
		cairo_stroke(activebuffer->cairo);
	}

	if (style->stroke_type != PLOT_OP_TYPE_NONE) {
		nsvi_set_colour(style->stroke_colour);

		switch (style->stroke_type) {
		case PLOT_OP_TYPE_SOLID: /* Solid colour */
		default:
			nsvi_set_solid();
			break;

		case PLOT_OP_TYPE_DOT: /* Doted plot */
			nsvi_set_dotted();
			break;

		case PLOT_OP_TYPE_DASH: /* dashed plot */
			nsvi_set_dashed();
			break;
		}

		nsvi_set_line_width(style->stroke_width);

		cairo_rectangle(activebuffer->cairo,
				rect->x0 + 0.5,
				rect->y0 + 0.5,
				rect->x1 - rect->x0,
				rect->y1 - rect->y0);
		cairo_stroke(activebuffer->cairo);
	}
	return NSERROR_OK;
}

static nserror
nsvi_plot_polygon(const struct redraw_context *ctx,
		   const plot_style_t *style,
		   const int *p,
		   unsigned int n)
{
	unsigned int i;

	nsvi_set_colour(style->fill_colour);
	nsvi_set_solid();

	cairo_set_line_width(activebuffer->cairo, 0);
	cairo_move_to(activebuffer->cairo, p[0], p[1]);
	for (i = 1; i != n; i++) {
		cairo_line_to(activebuffer->cairo, p[i * 2], p[i * 2 + 1]);
	}
	cairo_fill(activebuffer->cairo);
	cairo_stroke(activebuffer->cairo);

	return NSERROR_OK;
}

static nserror
nsvi_plot_path(const struct redraw_context *ctx,
		const plot_style_t *pstyle,
		const float *p,
		unsigned int n,
		const float transform[6])
{
	unsigned int i;
	cairo_matrix_t old_ctm, n_ctm;

	if (n == 0)
		return NSERROR_OK;

	if (p[0] != PLOTTER_PATH_MOVE) {
		NSLOG(netsurf, INFO, "Path does not start with move");
		return NSERROR_INVALID;
	}

	/* Save CTM */
	cairo_get_matrix(activebuffer->cairo, &old_ctm);

	/* Set up line style and width */
	nsvi_set_line_width(pstyle->stroke_width);
	nsvi_set_solid();

	/* Load new CTM */
	n_ctm.xx = transform[0];
	n_ctm.yx = transform[1];
	n_ctm.xy = transform[2];
	n_ctm.yy = transform[3];
	n_ctm.x0 = transform[4] + old_ctm.x0;
	n_ctm.y0 = transform[5] + old_ctm.y0;

	cairo_set_matrix(activebuffer->cairo, &n_ctm);

	/* Construct path */
	for (i = 0; i < n; ) {
		if (p[i] == PLOTTER_PATH_MOVE) {
			cairo_move_to(activebuffer->cairo, p[i+1], p[i+2]);
			i += 3;
		} else if (p[i] == PLOTTER_PATH_CLOSE) {
			cairo_close_path(activebuffer->cairo);
			i++;
		} else if (p[i] == PLOTTER_PATH_LINE) {
			cairo_line_to(activebuffer->cairo, p[i+1], p[i+2]);
			i += 3;
		} else if (p[i] == PLOTTER_PATH_BEZIER) {
			cairo_curve_to(activebuffer->cairo, p[i+1], p[i+2],
				       p[i+3], p[i+4],
				       p[i+5], p[i+6]);
			i += 7;
		} else {
			NSLOG(netsurf, INFO, "bad path command %f", p[i]);
			/* Reset matrix for safety */
			cairo_set_matrix(activebuffer->cairo, &old_ctm);
			return NSERROR_INVALID;
		}
	}

	/* Restore original CTM */
	cairo_set_matrix(activebuffer->cairo, &old_ctm);

	/* Now draw path */
	if (pstyle->fill_colour != NS_TRANSPARENT) {
		nsvi_set_colour(pstyle->fill_colour);

		if (pstyle->stroke_colour != NS_TRANSPARENT) {
			/* Fill & Stroke */
			cairo_fill_preserve(activebuffer->cairo);
			nsvi_set_colour(pstyle->stroke_colour);
			cairo_stroke(activebuffer->cairo);
		} else {
			/* Fill only */
			cairo_fill(activebuffer->cairo);
		}
	} else if (pstyle->stroke_colour != NS_TRANSPARENT) {
		/* Stroke only */
		nsvi_set_colour(pstyle->stroke_colour);
		cairo_stroke(activebuffer->cairo);
	}

	return NSERROR_OK;
}

static nserror
nsvi_plot_bitmap(const struct redraw_context *ctx,
		  struct bitmap *bitmap,
		  int x, int y,
		  int width,
		  int height,
		  colour bg,
		  bitmap_flags_t flags)
{
	bool repeat_x = (flags & BITMAPF_REPEAT_X);
	bool repeat_y = (flags & BITMAPF_REPEAT_Y);
	struct rect cliprect_bitmap;
	cairo_surface_t *img_surface;
	int img_width, img_height;

	/* Bail early if we can */
	if (width <= 0 || height <= 0) {
		/* Nothing to plot */
		return NSERROR_OK;
	}

	/* Copy the clip rectangle into bitmap plot clip rectangle */
	cliprect_bitmap = cliprect;

	/* Constrain bitmap plot rectangle for any lack of tiling */
	if (!repeat_x) {
		if (cliprect_bitmap.x1 > width) {
			cliprect_bitmap.x1 = width;
		}
		if (cliprect_bitmap.x0 < x) {
			cliprect_bitmap.x0 = x;
			cliprect_bitmap.x1 -= x - cliprect_bitmap.x0;
		}
	}
	if (!repeat_y) {
		if (cliprect_bitmap.y1 > height) {
			cliprect_bitmap.y1 = height;
		}
		if (cliprect_bitmap.y0 < y) {
			cliprect_bitmap.y0 = y;
			cliprect_bitmap.y1 -= y - cliprect_bitmap.y0;
		}
	}

	/* Bail early if we can */
	if (cliprect_bitmap.y1 <= 0 || cliprect_bitmap.x1 <= 0) {
		/* Nothing to plot */
		return NSERROR_OK;
	}

	/* Get the image's surface and intrinsic dimensions */
	img_surface = bitmap->surface;
	img_width = cairo_image_surface_get_width(img_surface);
	img_height = cairo_image_surface_get_height(img_surface);

	/* Set the source surface */
	if ((img_width == width) && (img_height == height)) {
		/* Non-scaled rendering */
		cairo_set_source_surface(activebuffer->cairo, img_surface, x, y);

		/* Enable tiling if we're repeating */
		if (repeat_x || repeat_y) {
			cairo_pattern_set_extend(
					cairo_get_source(activebuffer->cairo),
					CAIRO_EXTEND_REPEAT);
		}

		/* Render the bitmap */
		cairo_rectangle(activebuffer->cairo,
				cliprect_bitmap.x0,
				cliprect_bitmap.y0,
				cliprect_bitmap.x1,
				cliprect_bitmap.y1);
		cairo_fill(activebuffer->cairo);
	} else {
		/* Scaled rendering */
		double scale_x = (double)width / img_width;
		double scale_y = (double)height / img_height;

		/* Save cairo rendering context state before scaling */
		cairo_save(activebuffer->cairo);
		cairo_scale(activebuffer->cairo, scale_x, scale_y);

		cairo_set_source_surface(activebuffer->cairo, img_surface,
				x / scale_x, y / scale_y);

		/* Enable tiling if we're repeating */
		if (repeat_x || repeat_y) {
			cairo_pattern_set_extend(
					cairo_get_source(activebuffer->cairo),
					CAIRO_EXTEND_REPEAT);
		}

		/* Render the bitmap */
		cairo_rectangle(activebuffer->cairo,
				cliprect_bitmap.x0 / scale_x,
				cliprect_bitmap.y0 / scale_y,
				cliprect_bitmap.x1 / scale_x,
				cliprect_bitmap.y1 / scale_y);
		cairo_fill(activebuffer->cairo);

		/* Restore pre-scaling cairo rendering state */
		cairo_restore(activebuffer->cairo);
	}

	return NSERROR_OK;
}

static nserror
nsvi_plot_text(const struct redraw_context *ctx,
		const struct plot_font_style *fstyle,
		int x,
		int y,
		const char *text,
		size_t length)
{
	return nsfont_paint(x, y, text, length, fstyle);
}


/** GTK plotter table */
const struct plotter_table nsvi_plotters = {
	.clip = nsvi_plot_clip,
	.arc = nsvi_plot_arc,
	.disc = nsvi_plot_disc,
	.line = nsvi_plot_line,
	.rectangle = nsvi_plot_rectangle,
	.polygon = nsvi_plot_polygon,
	.path = nsvi_plot_path,
	.bitmap = nsvi_plot_bitmap,
	.text = nsvi_plot_text,
	.option_knockout = true
};
