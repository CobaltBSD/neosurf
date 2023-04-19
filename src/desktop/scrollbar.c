/*
 * Copyright 2004-2008 James Bursa <bursa@users.sourceforge.net>
 * Copyright 2008 Michael Drake <tlsa@netsurf-browser.org>
 * Copyright 2009 Paul Blokus <paul_pl@users.sourceforge.net>
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

/**
 * \file
 * implementation of scrollbar widget.
 */

#include <stdbool.h>
#include <stdlib.h>

#include "utils/log.h"
#include "utils/messages.h"
#include "utils/utils.h"
#include "utils/nscolour.h"
#include "utils/nsoption.h"
#include "netsurf/browser_window.h"
#include "netsurf/mouse.h"
#include "netsurf/plotters.h"

#include "desktop/system_colour.h"
#include "desktop/scrollbar.h"

/**
 * Scrollbar context
 */
struct scrollbar {
	/** Horizontal scrollbar if true, else vertical */
	bool horizontal;
	/** Length of the scrollbar widget */
	int length;

	/** Length of the full scrollable area */
	int full_size;
	/** Length visible part of the scrollable area */
	int visible_size;

	/** Current scroll offset to visible area */
	int offset;

	/** Position of the scrollbar */
	int bar_pos;
	/** Length of the scrollbar */
	int bar_len;

	/** Callback receiving scrollbar events */
	scrollbar_client_callback client_callback;
	/** User data passed to the callback */
	void *client_data;

	/** Flag indicating drag at progess */
	bool dragging;
	/** Coordinate value at drag start */
	int drag_start_coord;
	/** Scrollbar offset or bar_pos at drag start */
	int drag_start_pos;
	/** Flag indicating that the drag corresponds to a dragged
	 * content area, rather than a dragged scrollbar.
	 */
	bool drag_content;

	/** Parpendicular scrollbar, or NULL */
	struct scrollbar *pair;
	/** Flag indicating that the current drag affects the
	 *  perpendicular scrollbar too
	 */
	bool pair_drag;
};



/*
 * Exported interface.  Documented in desktop/scrollbar.h
 */
nserror
scrollbar_create(bool horizontal,
		 int length,
		 int full_size,
		 int visible_size,
		 void *client_data,
		 scrollbar_client_callback client_callback,
		 struct scrollbar **s)
{
	struct scrollbar *scrollbar;
	int well_length;

	scrollbar = malloc(sizeof(struct scrollbar));
	if (scrollbar == NULL) {
		*s = NULL;
		return NSERROR_NOMEM;
	}

	scrollbar->horizontal = horizontal;
	scrollbar->length = length;
	scrollbar->full_size = full_size;
	scrollbar->visible_size = visible_size;
	scrollbar->offset = 0;
	scrollbar->bar_pos = 0;
	scrollbar->pair = NULL;
	scrollbar->pair_drag = false;

	well_length = length - 2 * SCROLLBAR_WIDTH;
	scrollbar->bar_len = (full_size == 0) ? 0 :
		((well_length * visible_size) / full_size);

	scrollbar->client_callback = client_callback;
	scrollbar->client_data = client_data;

	scrollbar->dragging = false;
	scrollbar->drag_content = false;

	*s = scrollbar;

	return NSERROR_OK;
}


/*
 * Exported interface.  Documented in scrollbar.h
 */
void scrollbar_destroy(struct scrollbar *s)
{
	if (s->pair != NULL) {
		s->pair->pair = NULL;
	}
	free(s);
}


/**
 * Draw an outline rectangle common to several scrollbar elements.
 *
 * \param ctx current redraw context
 * \param area the area of the scrollbar
 * \param c base colour of the outline, the other colours are created by
 *		lightening or darkening this one
 * \param inset true for inset outline, false for an outset one
 * \return NSERROR_OK on success else error code
 */
static inline nserror
scrollbar_rectangle(const struct redraw_context *ctx,
		    struct rect *area,
		    colour c,
		    bool inset)
{
	struct rect line;
	nserror res;

	static plot_style_t c0 = {
		.stroke_type = PLOT_OP_TYPE_SOLID,
		.stroke_width = plot_style_int_to_fixed(1),
	};

	static plot_style_t c1 = {
		.stroke_type = PLOT_OP_TYPE_SOLID,
		.stroke_width = plot_style_int_to_fixed(1),
	};

	static plot_style_t c2 = {
		.stroke_type = PLOT_OP_TYPE_SOLID,
		.stroke_width = plot_style_int_to_fixed(1),
	};

	if (inset) {
		c0.stroke_colour = darken_colour(c);
		c1.stroke_colour = lighten_colour(c);
	} else {
		c0.stroke_colour = lighten_colour(c);
		c1.stroke_colour = darken_colour(c);
	}
	c2.stroke_colour = blend_colour(c0.stroke_colour, c1.stroke_colour);

	/* Plot the outline */

	line.x0 = area->x0; line.y0 = area->y0;
	line.x1 = area->x1; line.y1 = area->y0;
	res = ctx->plot->line(ctx, &c0, &line);
	if (res != NSERROR_OK) {
		return res;
	}

	line.x0 = area->x1; line.y0 = area->y0;
	line.x1 = area->x1; line.y1 = area->y1 + 1;
	res = ctx->plot->line(ctx, &c1, &line);
	if (res != NSERROR_OK) {
		return res;
	}

	line.x0 = area->x1; line.y0 = area->y0;
	line.x1 = area->x1; line.y1 = area->y0 + 1;
	res = ctx->plot->line(ctx, &c2, &line);
	if (res != NSERROR_OK) {
		return res;
	}

	line.x0 = area->x1; line.y0 = area->y1;
	line.x1 = area->x0; line.y1 = area->y1;
	res = ctx->plot->line(ctx, &c1, &line);
	if (res != NSERROR_OK) {
		return res;
	}

	line.x0 = area->x0; line.y0 = area->y1;
	line.x1 = area->x0; line.y1 = area->y0;
	res = ctx->plot->line(ctx, &c0, &line);
	if (res != NSERROR_OK) {
		return res;
	}

	line.x0 = area->x0; line.y0 = area->y1;
	line.x1 = area->x0; line.y1 = area->y1 + 1;
	res = ctx->plot->line(ctx, &c2, &line);

	return res;
}


/*
 * Exported interface.  Documented in scrollbar.h
 */
nserror
scrollbar_redraw(struct scrollbar *s,
		 int x, int y,
		 const struct rect *clip,
		 float scale,
		 const struct redraw_context *ctx)
{
	int w = SCROLLBAR_WIDTH;
	int bar_pos, bar_c0, bar_c1;
	int v[6]; /* array of triangle vertices */
	struct rect area;
	struct rect rect;
	nserror res;

	plot_style_t bg_fill_style = {
		.fill_type = PLOT_OP_TYPE_SOLID,
		.fill_colour = nscolours[NSCOLOUR_SCROLL_WELL],
	};
	plot_style_t fg_fill_style = {
		.fill_type = PLOT_OP_TYPE_SOLID,
		.fill_colour = nscolours[NSCOLOUR_BUTTON_BG],
	};
	plot_style_t arrow_fill_style = {
		.fill_type = PLOT_OP_TYPE_SOLID,
		.fill_colour = nscolours[NSCOLOUR_BUTTON_FG],
	};

	area.x0 = x;
	area.y0 = y;
	area.x1 = x + (s->horizontal ? s->length : SCROLLBAR_WIDTH) - 1;
	area.y1 = y + (s->horizontal ? SCROLLBAR_WIDTH : s->length) - 1;
	bar_pos = s->bar_pos;
	bar_c1 = (s->horizontal ? area.x0 : area.y0) + SCROLLBAR_WIDTH +
		s->bar_pos + s->bar_len - 1;

	if (scale != 1.0) {
		w *= scale;
		area.x0 *= scale;
		area.y0 *= scale;
		area.x1 *= scale;
		area.y1 *= scale;
		bar_pos *= scale;
		bar_c1 *= scale;
	}

	bar_c0 = (s->horizontal ? area.x0 : area.y0) + w + bar_pos;

	/* if scrollbar is outside the clipping rectangle, nothing to render */
	if ((area.x1 < clip->x0) ||
	    (area.y1 < clip->y0) ||
	    (clip->x1 < area.x0) ||
	    (clip->y1 < area.y0)) {
		return NSERROR_OK;
	}

	if (s->horizontal) {
		/* scrollbar is horizontal */

		/* scrollbar outline */
		res = scrollbar_rectangle(ctx, &area,
					  bg_fill_style.fill_colour, true);
		if (res != NSERROR_OK) {
			return res;
		}

		/* left arrow icon border */
		rect.x0 = area.x0 + 1;
		rect.y0 = area.y0 + 1;
		rect.x1 = area.x0 + w - 2;
		rect.y1 = area.y1 - 1;
		res = scrollbar_rectangle(ctx, &rect,
					  fg_fill_style.fill_colour, false);
		if (res != NSERROR_OK) {
			return res;
		}

		/* left arrow icon background */
		rect.x0 = area.x0 + 2;
		rect.y0 = area.y0 + 2;
		rect.x1 = area.x0 + w - 2;
		rect.y1 = area.y1 - 1;
		res = ctx->plot->rectangle(ctx, &fg_fill_style, &rect);
		if (res != NSERROR_OK) {
			return res;
		}

		/* left arrow */
		v[0] = area.x0 + w / 4;
		v[1] = area.y0 + w / 2;
		v[2] = area.x0 + w * 3 / 4;
		v[3] = area.y0 + w / 4;
		v[4] = area.x0 + w * 3 / 4;
		v[5] = area.y0 + w * 3 / 4;
		res = ctx->plot->polygon(ctx, &arrow_fill_style, v, 3);
		if (res != NSERROR_OK) {
			return res;
		}

		/* scrollbar well background */
		rect.x0 = area.x0 + w - 1;
		rect.y0 = area.y0 + 1;
		rect.x1 = area.x1 - w + 2;
		rect.y1 = area.y1;
		res = ctx->plot->rectangle(ctx, &bg_fill_style, &rect);
		if (res != NSERROR_OK) {
			return res;
		}

		/* scrollbar position indicator bar */
		rect.x0 = bar_c0;
		rect.y0 = area.y0 + 1;
		rect.x1 = bar_c1;
		rect.y1 = area.y1 - 1;
		res = scrollbar_rectangle(ctx, &rect,
					  fg_fill_style.fill_colour, false);
		if (res != NSERROR_OK) {
			return res;
		}

		rect.x0 = bar_c0 + 1;
		rect.y0 = area.y0 + 2;
		rect.x1 = bar_c1;
		rect.y1 = area.y1 - 1;
		res = ctx->plot->rectangle(ctx, &fg_fill_style, &rect);
		if (res != NSERROR_OK) {
			return res;
		}

		/* right arrow icon border */
		rect.x0 = area.x1 - w + 2;
		rect.y0 = area.y0 + 1;
		rect.x1 = area.x1 - 1;
		rect.y1 = area.y1 - 1;
		res = scrollbar_rectangle(ctx, &rect,
					  fg_fill_style.fill_colour, false);
		if (res != NSERROR_OK) {
			return res;
		}

		/* right arrow icon background */
		rect.x0 = area.x1 - w + 3;
		rect.y0 = area.y0 + 2;
		rect.x1 = area.x1 - 1;
		rect.y1 = area.y1 - 1;
		res = ctx->plot->rectangle(ctx, &fg_fill_style, &rect);
		if (res != NSERROR_OK) {
			return res;
		}

		/* right arrow */
		v[0] = rect.x1 - w / 4 + 1;
		v[1] = rect.y0 + w / 2;
		v[2] = rect.x1 - w * 3 / 4 + 1;
		v[3] = rect.y0 + w / 4;
		v[4] = rect.x1 - w * 3 / 4 + 1;
		v[5] = rect.y0 + w * 3 / 4;
		res = ctx->plot->polygon(ctx, &arrow_fill_style, v, 3);
		if (res != NSERROR_OK) {
			return res;
		}
	} else {
		/* scrollbar is vertical */

		/* outline */
		res = scrollbar_rectangle(ctx, &area,
					  bg_fill_style.fill_colour, true);
		if (res != NSERROR_OK) {
			return res;
		}

		/* top arrow border */
		rect.x0 = area.x0 + 1;
		rect.y0 = area.y0 + 1;
		rect.x1 = area.x1 - 1;
		rect.y1 = area.y0 + w - 2;
		res = scrollbar_rectangle(ctx, &rect,
					  fg_fill_style.fill_colour, false);
		if (res != NSERROR_OK) {
			return res;
		}

		/* top arrow background */
		rect.x0 = area.x0 + 2;
		rect.y0 = area.y0 + 2;
		rect.x1 = area.x1 - 1;
		rect.y1 = area.y0 + w - 2;
		res = ctx->plot->rectangle(ctx, &fg_fill_style, &rect);
		if (res != NSERROR_OK) {
			return res;
		}

		/* up arrow */
		v[0] = area.x0 + w / 2;
		v[1] = area.y0 + w / 4;
		v[2] = area.x0 + w / 4;
		v[3] = area.y0 + w * 3 / 4;
		v[4] = area.x0 + w * 3 / 4;
		v[5] = area.y0 + w * 3 / 4;
		res = ctx->plot->polygon(ctx, &arrow_fill_style, v, 3);
		if (res != NSERROR_OK) {
			return res;
		}

		/* scrollbar well background */
		rect.x0 = area.x0 + 1;
		rect.y0 = area.y0 + w - 1;
		rect.x1 = area.x1;
		rect.y1 = area.y1 - w + 2;
		res = ctx->plot->rectangle(ctx, &bg_fill_style, &rect);
		if (res != NSERROR_OK) {
			return res;
		}

		/* scrollbar position indicator bar */
		rect.x0 = area.x0 + 1;
		rect.y0 = bar_c0;
		rect.x1 = area.x1 - 1;
		rect.y1 = bar_c1;
		res = scrollbar_rectangle(ctx, &rect,
					  fg_fill_style.fill_colour, false);
		if (res != NSERROR_OK) {
			return res;
		}

		rect.x0 = area.x0 + 2;
		rect.y0 = bar_c0 + 1;
		rect.x1 = area.x1 - 1;
		rect.y1 = bar_c1;
		res = ctx->plot->rectangle(ctx, &fg_fill_style, &rect);
		if (res != NSERROR_OK) {
			return res;
		}

		/* down arrow icon border */
		rect.x0 = area.x0 + 1;
		rect.y0 = area.y1 - w + 2;
		rect.x1 = area.x1 - 1;
		rect.y1 = area.y1 - 1;
		res = scrollbar_rectangle(ctx, &rect,
					  fg_fill_style.fill_colour, false);
		if (res != NSERROR_OK) {
			return res;
		}

		/* down arrow icon background */
		rect.x0 = area.x0 + 2;
		rect.y0 = area.y1 - w + 3;
		rect.x1 = area.x1 - 1;
		rect.y1 = area.y1 - 1;
		res = ctx->plot->rectangle(ctx, &fg_fill_style, &rect);
		if (res != NSERROR_OK) {
			return res;
		}

		/* down arrow */
		v[0] = area.x0 + w / 2;
		v[1] = area.y1 - w / 4 + 1;
		v[2] = area.x0 + w / 4;
		v[3] = area.y1 - w * 3 / 4 + 1;
		v[4] = area.x0 + w * 3 / 4;
		v[5] = area.y1 - w * 3 / 4 + 1;
		res = ctx->plot->polygon(ctx, &arrow_fill_style, v, 3);
		if (res != NSERROR_OK) {
			return res;
		}
	}

	return NSERROR_OK;
}


/*
 * Exported interface.  Documented in scrollbar.h
 */
void scrollbar_set(struct scrollbar *s, int value, bool bar_pos)
{
	int well_length;
	int old_offset = s->offset;
	struct scrollbar_msg_data msg;

	if (value < 0) {
		value = 0;
	}

	if (s->full_size == s->visible_size) {
		return;
	}

	well_length = s->length - 2 * SCROLLBAR_WIDTH;
	if (bar_pos) {
		if (value > well_length - s->bar_len) {
			s->bar_pos = well_length - s->bar_len;
		} else {
			s->bar_pos = value;
		}

		s->offset = ((well_length - s->bar_len) < 1) ? 0 :
			(((s->full_size - s->visible_size) *
			  s->bar_pos) / (well_length - s->bar_len));

	} else {
		if (value > s->full_size - s->visible_size) {
			s->offset = s->full_size - s->visible_size;
		} else {
			s->offset = value;
		}

		s->bar_pos = (s->full_size < 1) ? 0 :
			((well_length * s->offset) / s->full_size);
	}

	if (s->offset != old_offset) {
		/* client callback if there was a chnage */
		msg.scrollbar = s;
		msg.msg = SCROLLBAR_MSG_MOVED;
		msg.scroll_offset = s->offset;
		s->client_callback(s->client_data, &msg);
	}
}


/*
 * Exported interface.  Documented in scrollbar.h
 */
bool scrollbar_scroll(struct scrollbar *s, int change)
{
	int well_length;
	int old_offset = s->offset;
	struct scrollbar_msg_data msg;

	if (change == 0 || s->full_size <= s->visible_size) {
		/* zero scroll step, or unscrollable */
		return false;
	}

	/* Convert named change values to appropriate pixel offset value */
	switch (change) {
	case SCROLL_TOP:
		change = -s->full_size;
		break;

	case SCROLL_PAGE_UP:
		change = -s->visible_size;
		break;

	case SCROLL_PAGE_DOWN:
		change = s->visible_size;
		break;

	case SCROLL_BOTTOM:
		change = s->full_size;
		break;

	default:
		/* Change value is already a pixel offset */
		break;
	}

	/* Get new offset */
	if (s->offset + change > s->full_size - s->visible_size) {
		s->offset = s->full_size - s->visible_size;
	} else if (s->offset + change < 0) {
		s->offset = 0;
	} else {
		s->offset += change;
	}

	if (s->offset == old_offset) {
		/* Nothing happened */
		return false;
	}

	/* Update scrollbar */
	well_length = s->length - 2 * SCROLLBAR_WIDTH;
	s->bar_pos = (s->full_size < 1) ? 0 :
		((well_length * s->offset) / s->full_size);

	msg.scrollbar = s;
	msg.msg = SCROLLBAR_MSG_MOVED;
	msg.scroll_offset = s->offset;
	s->client_callback(s->client_data, &msg);

	return true;
}


/*
 * Exported interface.  Documented in scrollbar.h
 */
int scrollbar_get_offset(struct scrollbar *s)
{
	if (s == NULL) {
		return 0;
	}
	return s->offset;
}


/*
 * Exported interface.  Documented in scrollbar.h
 */
void scrollbar_set_extents(struct scrollbar *s, int length,
			   int visible_size, int full_size)
{
	int cur_excess = s->full_size - s->visible_size;
	int well_length;
	struct scrollbar_msg_data msg;

	if (length == s->length &&
	    visible_size == s->visible_size &&
	    full_size == s->full_size) {
		/* Nothing's changed. */
		return;
	}

	if (length != -1) {
		s->length = length;
	}
	if (visible_size != -1) {
		s->visible_size = visible_size;
	}
	if (full_size != -1) {
		s->full_size = full_size;
	}

	if (s->full_size < s->visible_size) {
		s->full_size = s->visible_size;
	}

	/* Update scroll offset (scaled in proportion with change in excess) */
	if (cur_excess <= 0) {
		s->offset = 0;
	} else {
		s->offset = (full_size - visible_size) * s->offset / cur_excess;
	}

	well_length = s->length - 2 * SCROLLBAR_WIDTH;

	if (s->full_size < 1) {
		s->bar_len = well_length;
		s->bar_pos = 0;
	} else {
		s->bar_len = (well_length * s->visible_size) / s->full_size;
		s->bar_pos = (well_length * s->offset) / s->full_size;
	}

	msg.scrollbar = s;
	msg.msg = SCROLLBAR_MSG_MOVED;
	msg.scroll_offset = s->offset;
	s->client_callback(s->client_data, &msg);
}


/*
 * Exported interface.  Documented in scrollbar.h
 */
bool scrollbar_is_horizontal(struct scrollbar *s)
{
	return s->horizontal;
}


/**
 * Internal procedure used for starting a drag scroll for a scrollbar.
 *
 * \param s		the scrollbar to start the drag for
 * \param x		the X coordinate of the drag start
 * \param y		the Y coordinate of the drag start
 * \param content_drag	whether this should be a reverse drag (used when the
 *			user drags the content area, rather than the scrollbar)
 * \param pair		whether the drag is a '2D' scroll
 */
static void
scrollbar_drag_start_internal(struct scrollbar *s,
			      int x, int y,
			      bool content_drag,
			      bool pair)
{
	struct scrollbar_msg_data msg;

	s->drag_start_coord = s->horizontal ? x : y;
	s->drag_start_pos = (content_drag) ? s->offset : s->bar_pos;

	s->dragging = true;
	s->drag_content = content_drag;

	msg.scrollbar = s;

	/** \todo some proper numbers please! */
	if (s->horizontal) {
		msg.x0 = -2048;
		msg.x1 = 2048;
		msg.y0 = 0;
		msg.y1 = 0;
	} else {
		msg.x0 = 0;
		msg.x1 = 0;
		msg.y0 = -2048;
		msg.y1 = 2048;
	}

	if (pair && s->pair != NULL) {
		s->pair_drag = true;

		s->pair->drag_start_coord =
			s->pair->horizontal ? x : y;

		s->pair->drag_start_pos = (content_drag) ? s->pair->offset :
			s->pair->bar_pos;

		s->pair->dragging = true;
		s->pair->drag_content = content_drag;

		if (s->pair->horizontal) {
			msg.x0 = -2048;
			msg.x1 = 2048;
		} else {
			msg.y0 = -2048;
			msg.y1 = 2048;
		}
	}
	msg.msg = SCROLLBAR_MSG_SCROLL_START;
	s->client_callback(s->client_data, &msg);
}


/*
 * Exported interface.  Documented in scrollbar.h
 */
scrollbar_mouse_status
scrollbar_mouse_action(struct scrollbar *s,
		       browser_mouse_state mouse,
		       int x, int y)
{
	int x0, y0, x1, y1;
	int val;
	scrollbar_mouse_status status = SCROLLBAR_MOUSE_NONE;
	bool h;

	/* we want mouse presses and mouse drags that were not started at the
	 * scrollbar indication bar to be launching actions on the scroll area
	 */
	bool but1 = ((mouse & BROWSER_MOUSE_PRESS_1) ||
		     ((mouse & BROWSER_MOUSE_HOLDING_1) &&
		      (mouse & BROWSER_MOUSE_DRAG_ON) &&
		      !s->dragging));
	bool but2 = ((mouse & BROWSER_MOUSE_PRESS_2) ||
		     ((mouse & BROWSER_MOUSE_HOLDING_2) &&
		      (mouse & BROWSER_MOUSE_DRAG_ON) &&
		      !s->dragging));

	h = s->horizontal;

	x0 = 0;
	y0 = 0;
	x1 = h ? s->length : SCROLLBAR_WIDTH;
	y1 = h ? SCROLLBAR_WIDTH : s->length;

	if (!s->dragging && !(x >= x0 && x <= x1 && y >= y0 && y <= y1)) {
		/* Not a drag and mouse outside scrollbar widget */
		return SCROLLBAR_MOUSE_NONE;
	}


	if (h) {
		val = x;
	} else {
		val = y;
	}

	if (s->dragging) {
		val -= s->drag_start_coord;
		if (s->drag_content) {
			val = -val;
		}
		if (val != 0) {
			scrollbar_set(s, s->drag_start_pos + val,
				      !(s->drag_content));
		}
		if (s->pair_drag) {
			scrollbar_mouse_action(s->pair, mouse, x, y);
			status = SCROLLBAR_MOUSE_BOTH;
		} else {
			status = h ? SCROLLBAR_MOUSE_HRZ : SCROLLBAR_MOUSE_VRT;
		}
		return status;
	}

	if (val < SCROLLBAR_WIDTH) {
		/* left/up arrow */

		status = h ? SCROLLBAR_MOUSE_LFT : SCROLLBAR_MOUSE_UP;
		if (but1) {
			scrollbar_set(s, s->offset - SCROLLBAR_WIDTH, false);
		} else if (but2) {
			scrollbar_set(s, s->offset + SCROLLBAR_WIDTH, false);
		}
	} else if (val < SCROLLBAR_WIDTH + s->bar_pos) {
		/* well between left/up arrow and bar */

		status = h ? SCROLLBAR_MOUSE_PLFT : SCROLLBAR_MOUSE_PUP;

		if (but1) {
			scrollbar_set(s, s->offset - s->length, false);
		} else if (but2) {
			scrollbar_set(s, s->offset + s->length, false);
		}
	} else if (val > s->length - SCROLLBAR_WIDTH) {
		/* right/down arrow */

		status = h ? SCROLLBAR_MOUSE_RGT : SCROLLBAR_MOUSE_DWN;

		if (but1) {
			scrollbar_set(s, s->offset + SCROLLBAR_WIDTH, false);
		} else if (but2) {
			scrollbar_set(s, s->offset - SCROLLBAR_WIDTH, false);
		}
	} else if (val > SCROLLBAR_WIDTH + s->bar_pos +	s->bar_len) {
		/* well between right/down arrow and bar */

		status = h ? SCROLLBAR_MOUSE_PRGT : SCROLLBAR_MOUSE_PDWN;
		if (but1) {
			scrollbar_set(s, s->offset + s->length, false);
		} else if (but2) {
			scrollbar_set(s, s->offset - s->length, false);
		}
	} else {
		/* scrollbar position indication bar */

		status = h ? SCROLLBAR_MOUSE_HRZ : SCROLLBAR_MOUSE_VRT;
	}


	if (mouse & (BROWSER_MOUSE_DRAG_1 | BROWSER_MOUSE_DRAG_2) &&
	    (val >= SCROLLBAR_WIDTH + s->bar_pos
	     && val < SCROLLBAR_WIDTH + s->bar_pos +
	     s->bar_len)) {
		/* The mouse event is a drag start on the scrollbar position
		 * indication bar. */
		scrollbar_drag_start_internal(s, x, y, false,
					      (mouse & BROWSER_MOUSE_DRAG_2) ? true : false);
	}

	return status;
}


/*
 * Exported interface.  Documented in scrollbar.h
 */
const char *scrollbar_mouse_status_to_message(scrollbar_mouse_status status)
{
	switch ((unsigned int) status) {
	case SCROLLBAR_MOUSE_UP:
	case SCROLLBAR_MOUSE_UP | SCROLLBAR_MOUSE_USED:
		return messages_get("ScrollUp");
	case SCROLLBAR_MOUSE_PUP:
	case SCROLLBAR_MOUSE_PUP | SCROLLBAR_MOUSE_USED:
		return messages_get("ScrollPUp");
	case SCROLLBAR_MOUSE_VRT:
	case SCROLLBAR_MOUSE_VRT | SCROLLBAR_MOUSE_USED:
		return messages_get("ScrollV");
	case SCROLLBAR_MOUSE_PDWN:
	case SCROLLBAR_MOUSE_PDWN | SCROLLBAR_MOUSE_USED:
		return messages_get("ScrollPDown");
	case SCROLLBAR_MOUSE_DWN:
	case SCROLLBAR_MOUSE_DWN | SCROLLBAR_MOUSE_USED:
		return messages_get("ScrollDown");
	case SCROLLBAR_MOUSE_LFT:
	case SCROLLBAR_MOUSE_LFT | SCROLLBAR_MOUSE_USED:
		return messages_get("ScrollLeft");
	case SCROLLBAR_MOUSE_PLFT:
	case SCROLLBAR_MOUSE_PLFT | SCROLLBAR_MOUSE_USED:
		return messages_get("ScrollPLeft");
	case SCROLLBAR_MOUSE_HRZ:
	case SCROLLBAR_MOUSE_HRZ | SCROLLBAR_MOUSE_USED:
		return messages_get("ScrollH");
	case SCROLLBAR_MOUSE_PRGT:
	case SCROLLBAR_MOUSE_PRGT | SCROLLBAR_MOUSE_USED:
		return messages_get("ScrollPRight");
	case SCROLLBAR_MOUSE_RGT:
	case SCROLLBAR_MOUSE_RGT | SCROLLBAR_MOUSE_USED:
		return messages_get("ScrollRight");
	default:
		break;
	}

	return NULL;
}


/*
 * Exported interface.  Documented in scrollbar.h
 */
void scrollbar_mouse_drag_end(struct scrollbar *s,
			      browser_mouse_state mouse, int x, int y)
{
	struct scrollbar_msg_data msg;
	int val, drag_start_pos;

	assert(s->dragging);

	drag_start_pos = s->drag_start_pos;
	val = (s->horizontal ? x : y) - s->drag_start_coord;

	if (s->drag_content) {
		val = -val;
	}
	if (val != 0) {
		scrollbar_set(s, drag_start_pos + val, !(s->drag_content));
	}

	s->dragging = false;
	s->drag_content = false;

	if (s->pair_drag) {
		s->pair_drag = false;

		drag_start_pos = s->pair->drag_start_pos;
		val = (s->pair->horizontal ? x : y) - s->pair->drag_start_coord;

		if (s->pair->drag_content) {
			val = -val;
		}
		if (val != 0) {
			scrollbar_set(s->pair, drag_start_pos + val,
				      !(s->pair->drag_content));
		}

		s->pair->dragging = false;
		s->pair->drag_content = false;
	}

	msg.scrollbar = s;
	msg.msg = SCROLLBAR_MSG_SCROLL_FINISHED;
	s->client_callback(s->client_data, &msg);
}


/*
 * Exported interface.  Documented in scrollbar.h
 */
void scrollbar_start_content_drag(struct scrollbar *s, int x, int y)
{
	scrollbar_drag_start_internal(s, x, y, true, true);
}


/*
 * Exported interface.  Documented in scrollbar.h
 */
void scrollbar_make_pair(struct scrollbar *horizontal,
			 struct scrollbar *vertical)
{
	assert(horizontal->horizontal &&
	       !vertical->horizontal);

	horizontal->pair = vertical;
	vertical->pair = horizontal;
}


/*
 * Exported interface.  Documented in scrollbar.h
 */
void *scrollbar_get_data(struct scrollbar *s)
{
	return s->client_data;
}
