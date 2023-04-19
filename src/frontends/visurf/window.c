#include <assert.h>
#include <cairo/cairo.h>
#include <ctype.h>
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wayland-client.h>
#include "netsurf/browser_window.h"
#include "netsurf/keypress.h"
#include "netsurf/mouse.h"
#include "netsurf/plotters.h"
#include "netsurf/window.h"
#include "utils/errors.h"
#include "utils/nsurl.h"
#include "utils/log.h"
#include "visurf/commands.h"
#include "visurf/layout.h"
#include "visurf/plotters.h"
#include "visurf/pool-buffer.h"
#include "visurf/settings.h"
#include "visurf/undo.h"
#include "visurf/visurf.h"
#include "visurf/window.h"
#include "xdg-shell.h"
#include "xdg-decoration.h"
#include "desktop/textarea.h"
#include "content/handlers/html/form_internal.h"
#include "content/handlers/html/private.h"
#include "content/handlers/html/box.h"
#include "content/handlers/html/interaction.h"

#include <dom/dom.h>

struct pool_buffer *activebuffer;

static const struct wl_callback_listener wl_surface_frame_listener;

static void
cairo_set_source_u32(cairo_t *cairo, uint32_t color) {
	cairo_set_source_rgba(cairo,
			(color >> (3*8) & 0xFF) / 255.0,
			(color >> (2*8) & 0xFF) / 255.0,
			(color >> (1*8) & 0xFF) / 255.0,
			(color >> (0*8) & 0xFF) / 255.0);
}

static int
draw_tabs(struct nsvi_window *win, struct pool_buffer *buf)
{
	int height = 0;
	for (size_t i = 0; i < win->ntab; ++i) {
		const char *title = browser_window_get_title(win->tabs[i]->bw);

		int w, h;
		get_text_size(buf->cairo, config.font, &w, &h, NULL, "%s", title);
		if (h > height) {
			height = h;
		}
	}

	cairo_set_source_u32(buf->cairo, config.tabs.unselected.bg);
	cairo_rectangle(buf->cairo, 0, 0, win->width, config.margin * 2 + height);
	cairo_fill(buf->cairo);

	// TODO: This could be more sophisticated
	// grep for 'XXX: tabwidth' for related code
	int tabwidth = win->width / win->ntab;
	int x = 0;
	for (size_t i = 0; i < win->ntab; ++i) {
		const char *title = browser_window_get_title(win->tabs[i]->bw);

		int w, h;
		get_text_size(buf->cairo, config.font, &w, &h, NULL, "%s", title);
		if (win->tab == i) {
			cairo_set_source_u32(buf->cairo, config.tabs.selected.bg);
		} else {
			cairo_set_source_u32(buf->cairo, config.tabs.unselected.bg);
		}
		cairo_rectangle(buf->cairo, x, 0, tabwidth, height + config.margin * 2);
		cairo_fill(buf->cairo);

		if (win->tabs[i]->throb) {
			cairo_set_source_u32(buf->cairo, config.tabs.throbber.loading);
		} else {
			cairo_set_source_u32(buf->cairo, config.tabs.throbber.loaded);
		}
		cairo_rectangle(buf->cairo, x + config.margin, config.margin,
				config.tabs.throbber.width, height);
		cairo_fill(buf->cairo);

		cairo_save(buf->cairo);
		cairo_rectangle(buf->cairo, x, config.margin,
				tabwidth - config.margin * 2, height);
		cairo_clip(buf->cairo);
		if (win->tab == i) {
			cairo_set_source_u32(buf->cairo, config.tabs.selected.fg);
		} else {
			cairo_set_source_u32(buf->cairo, config.tabs.unselected.fg);
		}
		cairo_move_to(buf->cairo, x + (config.margin * 2) +
				config.tabs.throbber.width, config.margin);
		pango_printf(buf->cairo, config.font, "%s", title);
		cairo_restore(buf->cairo);
		x += tabwidth;
	}

	return height + config.margin * 2;
}

static char *
keybuf_to_str(struct nsvi_bindings *state)
{
	size_t bufsz = 8, bufidx = 0;
	char *keys = calloc(sizeof(char), bufsz);
	for (size_t i = 0; i < state->nbuf; i++) {
		size_t inc = 0;
		if (state->buffer[i].mask != 0) {
			char buf[6] = { 0 };
			size_t idx = 0;
			buf[idx++] = '<';
			if (state->buffer[i].mask & MOD_CTRL) {
				buf[idx++] = 'C';
			}
			if (state->buffer[i].mask & MOD_ALT) {
				buf[idx++] = 'A';
			}
			if (state->buffer[i].mask & MOD_LOGO) {
				buf[idx++] = 'L';
			}
			buf[idx++] = '-';
			buf[idx++] = '\0';
			int ret = snprintf(&keys[bufidx + inc], bufsz - bufidx,
				"%s", buf);
			assert(ret >= 0);
			if ((size_t)ret + 1 > bufsz - bufidx) {
				goto expand;
			}
			inc += ret;
		}
		int ret = xkb_keysym_to_utf8(state->buffer[i].keysym,
			&keys[bufidx + inc], bufsz - bufidx);
		if (ret < 0) {
			goto expand;
		}
		if (ret == 0) {
			continue;
		}
		inc += ret - 1;
		if (state->buffer[i].mask != 0) {
			int ret = snprintf(&keys[bufidx + inc], bufsz - bufidx,
				">");
			assert(ret >= 0);
			if ((size_t)ret + 1 > bufsz - bufidx) {
				goto expand;
			}
			inc += ret;
		}
		bufidx += inc;
		continue;
expand:
		bufsz *= 2;
		keys = realloc(keys, bufsz);
		i--;
		keys[bufidx] = '\0';
	}
	return keys;
}

static int
draw_normal_statusbar(struct nsvi_window *win, struct pool_buffer *buf)
{
	struct gui_window *gw = win->tabs[win->tab];
	int width, height;
	get_text_size(buf->cairo, config.font, &width, &height, NULL, "%s", gw->status);

	cairo_set_source_u32(buf->cairo, config.status.normal.bg);
	cairo_rectangle(buf->cairo, 0, win->height - (config.margin * 2 + height),
			win->width, config.margin * 2 + height);
	cairo_fill(buf->cairo);

	cairo_set_source_u32(buf->cairo, config.status.normal.fg);
	cairo_move_to(buf->cairo, config.margin, win->height - height - config.margin);
	pango_printf(buf->cairo, config.font, "%s", gw->status);

	char *status = keybuf_to_str(&win->state->bindings);
	nsurl *url;
	nserror err = browser_window_get_url(win->tabs[win->tab]->bw, true, &url);
	if (err == NSERROR_OK) {
		const char *data = nsurl_access(url);
		status = realloc(status, strlen(status) + strlen(data) + 2);
		strcat(strcat(status, " "), data);
	}
	int left_width = width;
	get_text_size(buf->cairo, config.font, &width, NULL, NULL, "%s", status);
	if (width > win->width - (int)config.margin * 2 - left_width) {
		width = win->width - config.margin * 2 - left_width;
	}
	cairo_move_to(buf->cairo, win->width - width - config.margin,
		win->height - height - config.margin);
	pango_printf(buf->cairo, config.font, "%s", status);
	free(status);
	return height + config.margin * 2;
}

static int
draw_insert_statusbar(struct nsvi_window *win, struct pool_buffer *buf)
{
	int width, height;
	get_text_size(buf->cairo, config.font, &width, &height, NULL, "INSERT");

	cairo_set_source_u32(buf->cairo, config.status.insert.bg);
	cairo_rectangle(buf->cairo, 0, win->height - (config.margin * 2 + height),
			win->width, config.margin * 2 + height);
	cairo_fill(buf->cairo);

	cairo_set_source_u32(buf->cairo, config.status.insert.fg);
	cairo_move_to(buf->cairo, config.margin, win->height - height - config.margin);
	pango_printf(buf->cairo, config.font, "INSERT");
	return height + config.margin * 2;
}

static int
draw_exline_completions(struct nsvi_window *win, struct pool_buffer *buf, int bottom)
{
	struct exline_state *exline = &win->exline;

	int height = 0;
	size_t nitems = 0;
	for (size_t i = 0; i < exline->ncomp; ++i, ++nitems) {
		int itemwidth, itemheight;
		get_text_size(buf->cairo, config.font,
				&itemwidth, &itemheight,
				NULL, "%s", exline->comps[i]);
		if (height + itemheight + (int)config.margin > win->height / 2) {
			break;
		}
		height += itemheight + config.margin;
	}

	cairo_set_source_u32(buf->cairo, config.status.exline.bg);
	cairo_rectangle(buf->cairo, 0, bottom - (config.margin * 2 + height),
			win->width, config.margin * 2 + height);
	cairo_fill(buf->cairo);

	int y = bottom;
	cairo_set_source_u32(buf->cairo, config.status.exline.fg);
	for (size_t i = 0; i < nitems; i += 1) {
		int itemwidth, itemheight;
		get_text_size(buf->cairo, config.font,
				&itemwidth, &itemheight,
				NULL, "%s", exline->comps[i]);
		y -= itemheight + config.margin;
		cairo_move_to(buf->cairo, config.margin, y);
		pango_printf(buf->cairo, config.font, "%s", exline->comps[i]);
	}

	return height;
}

static int
draw_exline_statusbar(struct nsvi_window *win, struct pool_buffer *buf)
{
	struct exline_state *exline = &win->exline;

	int width, height;
	get_text_size(buf->cairo, config.font, &width, &height, NULL, ":%s", exline->cmd);
	cairo_set_source_u32(buf->cairo, config.status.exline.bg);
	cairo_rectangle(buf->cairo, 0, win->height - (config.margin * 2 + height),
			win->width, config.margin * 2 + height);
	cairo_fill(buf->cairo);

	char c = exline->cmd[exline->index];
	exline->cmd[exline->index] = 0;

	cairo_set_source_u32(buf->cairo, config.status.exline.fg);
	cairo_move_to(buf->cairo, config.margin, win->height - height - config.margin);
	get_text_size(buf->cairo, config.font, &width, NULL, NULL, ":%s", exline->cmd);
	pango_printf(buf->cairo, config.font, ":%s", exline->cmd);

	cairo_move_to(buf->cairo,
			config.margin + width,
			win->height - height - config.margin);
	cairo_line_to(buf->cairo,
			config.margin + width,
			win->height - config.margin);
	cairo_set_line_width(buf->cairo, 1);
	cairo_stroke(buf->cairo);

	exline->cmd[exline->index] = c;

	cairo_move_to(buf->cairo,
			config.margin + width,
			win->height - height - config.margin);
	pango_printf(buf->cairo, config.font, "%s", &exline->cmd[exline->index]);

	int top = win->height - (config.margin + height);
	height += draw_exline_completions(win, buf, top);
	return height + config.margin * 2;
}

static int
draw_follow_statusbar(struct nsvi_window *win, struct pool_buffer *buf)
{
	int width, height;
	get_text_size(buf->cairo, config.font, &width, &height, NULL,
			"Choose a link to follow...");

	cairo_set_source_u32(buf->cairo, config.status.follow.bg);
	cairo_rectangle(buf->cairo, 0, win->height - (config.margin * 2 + height),
			win->width, config.margin * 2 + height);
	cairo_fill(buf->cairo);

	cairo_set_source_u32(buf->cairo, config.status.follow.fg);
	cairo_move_to(buf->cairo, config.margin, win->height - height - config.margin);
	pango_printf(buf->cairo, config.font, "Choose a link to follow...");
	return height + config.margin * 2;
}

static int
draw_error_statusbar(struct nsvi_window *win, struct pool_buffer *buf)
{
	int width, height;
	get_text_size(buf->cairo, config.font, &width, &height, NULL, "%s", win->error_message);

	cairo_set_source_u32(buf->cairo, config.status.error.bg);
	cairo_rectangle(buf->cairo, 0, win->height - (config.margin * 2 + height),
			win->width, config.margin * 2 + height);
	cairo_fill(buf->cairo);

	cairo_set_source_u32(buf->cairo, config.status.error.fg);
	cairo_move_to(buf->cairo, config.margin, win->height - height - config.margin);
	pango_printf(buf->cairo, config.font, "%s", win->error_message);
	return height + config.margin * 2;
}

static int
draw_status(struct nsvi_window *win, struct pool_buffer *buf)
{
	switch (win->mode) {
	case NORMAL:
		return draw_normal_statusbar(win, buf);
	case INSERT:
		return draw_insert_statusbar(win, buf);
	case EXLINE:
		return draw_exline_statusbar(win, buf);
	case FOLLOW:
		return draw_follow_statusbar(win, buf);
	case ERROR:
		return draw_error_statusbar(win, buf);
	}
	abort();
}

static void
draw_hints(struct nsvi_window *win, struct pool_buffer *buf)
{
	int gwheight = win->height - win->tab_height - win->status_height;
	cairo_save(buf->cairo);
	cairo_rectangle(buf->cairo, 0, win->tab_height, win->width, gwheight);
	cairo_clip(buf->cairo);

	struct gui_window *gw = win->tabs[win->tab];
	struct follow_state *follow = &gw->follow;
	for (size_t i = 0; i < follow->nhint; ++i) {
		struct link_hint *hint = &follow->hints[i];
		int x = hint->x, y = hint->y;
		x += gw->sx, y += gw->sy;
		y += win->tab_height;

		if (x < 0 || x >= win->width) {
			continue;
		}
		if (y < win->tab_height || y >= gwheight) {
			continue;
		}

		size_t nlike = 0;
		for (; nlike < follow->hintln && nlike < strlen(hint->hint);
				++nlike) {
			if (follow->hintbuf[nlike] != hint->hint[nlike]) {
				break;
			}
		}
		if (nlike < follow->hintln) {
			continue;
		}

		int width, height;
		get_text_size(buf->cairo, config.font, &width, &height,
				NULL, "%s", hint->hint);

		x -= width + config.margin * 2;
		if (x < 0) {
			x = 0;
		}

		cairo_set_source_u32(buf->cairo, config.hints.border.color);
		cairo_rectangle(buf->cairo, x - config.hints.border.width,
				y - config.hints.border.width,
				width + config.margin * 2 + config.hints.border.width * 2,
				height + config.margin * 2 + config.hints.border.width * 2);
		cairo_fill(buf->cairo);
		cairo_set_source_u32(buf->cairo, config.hints.bg);
		cairo_rectangle(buf->cairo, x, y,
				width + config.margin * 2,
				height + config.margin * 2);
		cairo_fill(buf->cairo);

		char c = hint->hint[nlike];
		hint->hint[nlike] = 0;
		get_text_size(buf->cairo, config.font, &width, &height,
				NULL, "%s", hint->hint);

		cairo_set_source_u32(buf->cairo, config.hints.like);
		cairo_move_to(buf->cairo, x + config.margin, y + config.margin);
		pango_printf(buf->cairo, config.font, "%s", hint->hint);

		hint->hint[nlike] = c;

		cairo_set_source_u32(buf->cairo, config.hints.fg);
		cairo_move_to(buf->cairo, x + config.margin + width, y + config.margin);
		pango_printf(buf->cairo, config.font, "%s", &hint->hint[nlike]);
	}

	cairo_restore(buf->cairo);
}

static bool
draw_frame(struct nsvi_window *win)
{
	struct pool_buffer *buffer = get_next_buffer(
			win->state->wl_shm, win->buffers,
			win->width, win->height, win->scale);
	if (!buffer) {
		NSLOG(netsurf, WARNING, "Unable to obtain buffer");
		return false;
	}
	activebuffer = buffer;

	struct gui_window *gw = win->tabs[win->tab];

	cairo_save(buffer->cairo);
	cairo_scale(buffer->cairo, win->scale, win->scale);
	int tab_height = draw_tabs(win, buffer);
	if (tab_height != win->tab_height) {
		win->tab_height = tab_height;
		browser_window_schedule_reformat(gw->bw);
	}

	int status_height = draw_status(win, buffer);
	if (status_height != win->status_height) {
		win->status_height = status_height;
		browser_window_schedule_reformat(gw->bw);
	}

	struct redraw_context ctx = {
		.interactive = true,
		.background_images = true,
		.plot = &nsvi_plotters
	};

	struct rect clip;
	clip.x0 = 0;
	clip.y0 = win->tab_height;
	clip.x1 = win->width;
	clip.y1 = win->height - status_height;

	int bx = gw->sx;
	int by = win->tab_height;

	if (bx > 0) {
		bx = 0;
	}

	if (gw->sy < 0) {
		by += gw->sy;
	}

	browser_window_redraw(gw->bw,
			bx, by,
			&clip, &ctx);

	if (gw->caret.enabled) {
		cairo_set_source_u32(buffer->cairo, config.caret.color);
		int x = gw->caret.x + gw->sx,
		    y = win->tab_height + gw->caret.y + gw->sy;
		cairo_move_to(buffer->cairo, x, y);
		cairo_line_to(buffer->cairo, x, y + gw->caret.height);
		cairo_set_line_width(buffer->cairo, 1);
		cairo_stroke(buffer->cairo);
	}

	if (gw->follow.hints) {
		draw_hints(win, buffer);
	}

	wl_surface_set_buffer_scale(win->wl_surface, win->scale);
	wl_surface_damage_buffer(win->wl_surface, 0, 0, INT32_MAX, INT32_MAX);
	wl_surface_attach(win->wl_surface, buffer->buffer, 0, 0);

	win->dirty = false;

	if (!win->pending_frame) {
		struct wl_callback *cb = wl_surface_frame(win->wl_surface);
		wl_callback_add_listener(cb, &wl_surface_frame_listener, win);
		win->pending_frame = true;
	}

	cairo_restore(buffer->cairo);
	return true;
}

static void
wl_surface_frame_done(void *data, struct wl_callback *cb, uint32_t time)
{
	wl_callback_destroy(cb);

	struct nsvi_window *win = data;
	win->pending_frame = false;

	if (!win->dirty) {
		return;
	}

	if (!draw_frame(win)) {
		return;
	}

	wl_surface_commit(win->wl_surface);
}

static const struct wl_callback_listener wl_surface_frame_listener = {
	.done = wl_surface_frame_done,
};

void
request_frame(struct nsvi_window *win)
{
	win->dirty = true;
	if (win->pending_frame) {
		return;
	}

	struct wl_callback *cb = wl_surface_frame(win->wl_surface);
	wl_callback_add_listener(cb, &wl_surface_frame_listener, win);
	wl_surface_commit(win->wl_surface);

	win->pending_frame = true;
}

static const char *
nsvi_window_xcursor(enum gui_pointer_shape shape)
{
	switch (shape) {
	case GUI_POINTER_DEFAULT:
		return "default";
	case GUI_POINTER_POINT:
		return "pointer";
	case GUI_POINTER_CARET:
		return "text";
	case GUI_POINTER_MENU:
		return "pencil";
	case GUI_POINTER_UP:
		return "top_side";
	case GUI_POINTER_DOWN:
		return "bottom_side";
	case GUI_POINTER_LEFT:
		return "left_side";
	case GUI_POINTER_RIGHT:
		return "right_side";
	case GUI_POINTER_RU:
		return "top_right_corner";
	case GUI_POINTER_LD:
		return "bottom_left_corner";
	case GUI_POINTER_LU:
		return "top_left_corner";
	case GUI_POINTER_RD:
		return "bottom_right_corner";
	case GUI_POINTER_CROSS:
		return "cross";
	case GUI_POINTER_MOVE:
		return "fleur";
	case GUI_POINTER_WAIT:
		return "wait";
	case GUI_POINTER_HELP:
		return "question_arrow";
	case GUI_POINTER_NO_DROP:
		return "no-drop";
	case GUI_POINTER_NOT_ALLOWED:
		return "not-allowed";
	case GUI_POINTER_PROGRESS:
		// TODO: Animate this
		return "watch";
	}
	abort(); // Unreachable
}

static void
nsvi_window_set_cursor(struct nsvi_window *win)
{
	const char *name = nsvi_window_xcursor(win->mouse.shape);
	struct wl_cursor *cursor = wl_cursor_theme_get_cursor(
		win->state->cursors, name);
	if (!cursor) {
		NSLOG(netsurf, WARNING,
			"Unable to select cursor %s, falling back to default",
			name);
		cursor = wl_cursor_theme_get_cursor(
			win->state->cursors, "left_ptr");
		assert(cursor);
	}
	win->mouse.cursor = cursor;

	if (!win->mouse.cursor_surface) {
		win->mouse.cursor_surface = wl_compositor_create_surface(
			win->state->wl_compositor);
	}

	// TODO: Animated cursors
	struct wl_cursor_image *image = cursor->images[0];
	struct wl_buffer *buffer = wl_cursor_image_get_buffer(image);
	wl_pointer_set_cursor(win->state->wl_pointer, win->mouse.serial,
			win->mouse.cursor_surface,
			image->hotspot_x, image->hotspot_y);
	wl_surface_attach(win->mouse.cursor_surface, buffer, 0, 0);
	wl_surface_damage_buffer(win->mouse.cursor_surface, 0, 0,
			INT32_MAX, INT32_MAX);
	wl_surface_commit(win->mouse.cursor_surface);
}

static void
xdg_toplevel_configure(void *data, struct xdg_toplevel *xdg_toplevel,
	int32_t width, int32_t height, struct wl_array *states)
{
	struct nsvi_window *win = data;
	enum xdg_toplevel_state *xdg_state;
	win->fullscreen = false;
	wl_array_for_each(xdg_state, states) {
		if (*xdg_state == XDG_TOPLEVEL_STATE_FULLSCREEN) {
			win->fullscreen = true;
		}
	}

	struct gui_window *tab = win->tabs[win->tab];
	if (width == 0 || height == 0) {
		/* Compositor is deferring to us */
		return;
	}
	if (win->width != width || win->height != height) {
		win->width = width;
		win->height = height;
		browser_window_schedule_reformat(tab->bw);
	}
}

static void
xdg_toplevel_close(void *data, struct xdg_toplevel *toplevel)
{
	struct nsvi_window *win = data;
	while (win->ntab) {
		browser_window_destroy(win->tabs[0]->bw);
	}
}

static const struct xdg_toplevel_listener xdg_toplevel_listener = {
	.configure = xdg_toplevel_configure,
	.close = xdg_toplevel_close,
};

static void
xdg_surface_configure(void *data,
		struct xdg_surface *xdg_surface,
		uint32_t serial)
{
	struct nsvi_window *win = data;
	xdg_surface_ack_configure(xdg_surface, serial);

	if (!draw_frame(win)) {
		return;
	}

	wl_surface_commit(win->wl_surface);
}

static const struct xdg_surface_listener xdg_surface_listener = {
	.configure = xdg_surface_configure,
};

static void
wl_surface_enter(void *data,
	struct wl_surface *wl_surface,
	struct wl_output *output)
{
	struct nsvi_window *win = data;
	struct nsvi_output *viout = wl_output_get_user_data(output);
	assert(viout);

	struct window_output *winout = calloc(1, sizeof(struct window_output));
	winout->output = viout;
	winout->next = win->outputs;
	win->outputs = winout;

	int max_scale = 1;
	for (winout = win->outputs; winout; winout = winout->next) {
		if (winout->output->scale > max_scale) {
			max_scale = winout->output->scale;
		}
	}
	if (win->scale != max_scale) {
		win->scale = max_scale;
		request_frame(win);
	}
}

static void
wl_surface_leave(void *data,
	struct wl_surface *wl_surface,
	struct wl_output *output)
{
	struct nsvi_window *win = data;
	struct window_output *winout = NULL;
	struct window_output **prev = &win->outputs;
	for (winout = win->outputs; winout; winout = winout->next) {
		if (winout->output->wl_output == output) {
			break;
		}
		prev = &winout->next;
	}
	assert(winout);

	*prev = winout->next;
	free(winout);

	int max_scale = 1;
	for (winout = win->outputs; winout; winout = winout->next) {
		if (winout->output->scale > max_scale) {
			max_scale = winout->output->scale;
		}
	}
	if (win->scale != max_scale) {
		win->scale = max_scale;
		request_frame(win);
	}
}

static const struct wl_surface_listener wl_surface_listener = {
	.enter = wl_surface_enter,
	.leave = wl_surface_leave,
};

static struct nsvi_window *
nsvi_window_create(void)
{
	struct nsvi_window *win = calloc(1, sizeof(struct nsvi_window));
	win->state = global_state;
	win->next = global_state->windows;
	global_state->windows = win;
	win->width = 640;
	win->height = 480;
	win->scale = 1;
	win->mouse.shape = GUI_POINTER_DEFAULT;

	// TEMP
	win->exline.ncomp = 3;
	win->exline.comps = calloc(3, sizeof(char *));
	win->exline.comps[0] = strdup("completion 1");
	win->exline.comps[1] = strdup("completion 2");
	win->exline.comps[2] = strdup("completion 3");

	win->wl_surface = wl_compositor_create_surface(
		global_state->wl_compositor);
	wl_surface_add_listener(win->wl_surface,
		&wl_surface_listener, win);
	win->xdg_surface = xdg_wm_base_get_xdg_surface(
		global_state->xdg_wm_base, win->wl_surface);
	xdg_surface_add_listener(win->xdg_surface, &xdg_surface_listener, win);
	win->xdg_toplevel = xdg_surface_get_toplevel(win->xdg_surface);
	xdg_toplevel_add_listener(win->xdg_toplevel,
		&xdg_toplevel_listener, win);
	xdg_toplevel_set_title(win->xdg_toplevel, "visurf");
	xdg_toplevel_set_app_id(win->xdg_toplevel, "visurf");
	if (global_state->xdg_decoration_manager != NULL) {
		win->xdg_toplevel_decoration =
			zxdg_decoration_manager_v1_get_toplevel_decoration(
					global_state->xdg_decoration_manager,
					win->xdg_toplevel);
		zxdg_toplevel_decoration_v1_set_mode(win->xdg_toplevel_decoration,
			ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
	}
	wl_surface_commit(win->wl_surface);
	return win;
}

static void
nsvi_window_add_tab(struct nsvi_window *win,
		struct gui_window *gw, bool background)
{
	gw->window = win;

	size_t i = win->tab + 1;
	if (!win->tabs) {
		win->tabs = calloc(1, sizeof(struct gui_window *));
		i = 0;
	} else {
		win->tabs = realloc(win->tabs,
			(win->ntab + 1) * sizeof(struct gui_window *));
		assert(win->tabs);
	}
	memmove(&win->tabs[i + 1], &win->tabs[i],
		sizeof(struct gui_window *) * (win->ntab - i));
	++win->ntab;
	win->tabs[i] = gw;
	if (!background) {
		win->tab = i;
	}
	request_frame(win);
}

static struct gui_window *
nsvi_gui_window_create(struct browser_window *bw,
	struct gui_window *existing,
	gui_window_create_flags flags)
{
	struct gui_window *gw = NULL;
	struct nsvi_window *win = NULL;
	if (flags & GW_CREATE_FOCUS_LOCATION) {
		// XXX: What should we do here? Tee up `exline open $url`?
		// Under what circumstances does this happen?
		NSLOG(netsurf, WARNING, "Location bar focus requested");
	}
	if (flags & GW_CREATE_TAB) {
		assert(existing);
		win = existing->window;
	} else if (existing) {
		win = existing->window;
		gw = existing;
	}

	if (!win) {
		win = nsvi_window_create();
	}
	if (!gw) {
		gw = calloc(1, sizeof(struct gui_window));
		gw->bw = bw;
		nsvi_window_add_tab(win, gw, !(flags & GW_CREATE_FOREGROUND));
	}
	gw->search_flags = SEARCH_FLAG_FORWARDS;
	return gw;
}

static void
nsvi_gui_window_destroy(struct gui_window *gw)
{
	struct nsvi_window *win = gw->window;
	size_t i = 0;
	for (; i < win->ntab; ++i) {
		if (win->tabs[i] == gw) {
			break;
		}
	}
	assert(i < win->ntab);

	memmove(&win->tabs[i], &win->tabs[i + 1],
		sizeof(struct gui_window *) * (win->ntab - (i + 1)));
	free(gw->search);
	free(gw);

	--win->ntab;
	if (win->tab >= win->ntab) {
		--win->tab;
	}
	if (win->ntab != 0) {
		request_frame(win);
		return;
	}

	struct nsvi_window **prev = &win->state->windows;
	for (struct nsvi_window *w = win->state->windows; w; w = w->next) {
		if (w == win) {
			*prev = w->next;
		}
		prev = &w->next;
	}

	if (!win->state->windows) {
		win->state->quit = true;
	}

	xdg_toplevel_destroy(win->xdg_toplevel);
	xdg_surface_destroy(win->xdg_surface);
	wl_surface_destroy(win->wl_surface);
	destroy_buffer(&win->buffers[0]);
	destroy_buffer(&win->buffers[1]);

	struct window_output *winout = win->outputs;
	while (winout) {
		struct window_output *next = winout->next;
		free(winout);
		winout = next;
	}

	free(win->exline.cmd);
	free(win->tabs);
	free(win);
}

static nserror
nsvi_window_invalidate(struct gui_window *gw, const struct rect *rect)
{
	struct nsvi_window *win = gw->window;
	if (win->tabs[win->tab] != gw) {
		return NSERROR_OK;
	}

	// TODO: Partial invaliation
	request_frame(win);
	return NSERROR_OK;
}

static bool
nsvi_window_get_scroll(struct gui_window *gw, int *sx, int *sy)
{
	*sx = gw->sx;
	*sy = -gw->sy;
	return true;
}

static nserror
nsvi_window_set_scroll(struct gui_window *gw, const struct rect *rect)
{
	struct nsvi_window *win = gw->window;
	gw->sx = rect->x0, gw->sy = -rect->y0;
	gui_window_constrain_scroll(gw);
	if (win->tabs[win->tab] == gw) {
		request_frame(win);
	}
	return NSERROR_OK;
}

static nserror
nsvi_window_get_dimensions(struct gui_window *gw, int *width, int *height)
{
	struct nsvi_window *win = gw->window;
	*width = win->width;
	*height = win->height - (win->status_height + win->tab_height);
	return NSERROR_OK;
}

static nserror
nsvi_window_event(struct gui_window *gw, enum gui_window_event event)
{
	struct nsvi_window *win = gw->window;
	browser_editor_flags ed_flags =
		browser_window_get_editor_flags(gw->bw);
	bool can_paste = !(ed_flags & BW_EDITOR_CAN_PASTE);
	switch (event) {
	case GW_EVENT_REMOVE_CARET:
		gw->caret.enabled = false;
		if (win->tabs[win->tab] == gw) {
			if (win->mode == INSERT && can_paste) {
				win->mode = NORMAL;
			}
			request_frame(win);
		}
		break;
	case GW_EVENT_START_THROBBER:
		gw->throb = true;
		request_frame(win);
		break;
	case GW_EVENT_STOP_THROBBER:
		gw->throb = false;
		request_frame(win);
		break;
	default: break; // Don't care
	}
	return NSERROR_OK;
}

static void
nsvi_window_set_title(struct gui_window *gw, const char *title)
{
	struct nsvi_window *win = gw->window;
	if (win->tabs[win->tab] != gw) {
		return;
	}
	xdg_toplevel_set_title(win->xdg_toplevel, title);
}

static void
nsvi_window_set_status(struct gui_window *gw, const char *text)
{
	gw->status = text;
	struct nsvi_window *win = gw->window;
	if (win->tabs[win->tab] != gw) {
		return;
	}
	request_frame(win);
}

static void
nsvi_window_set_pointer(struct gui_window *gw, enum gui_pointer_shape shape)
{
	struct nsvi_window *win = gw->window;
	if (win->tabs[win->tab] != gw) {
		// XXX: Do we want to stash the desired mouse cursor on the
		// gui_window for a tab switch?
		return;
	}
	if (shape != win->mouse.shape) {
		win->mouse.shape = shape;
		nsvi_window_set_cursor(win);
	}
}

static void
nsvi_window_place_caret(struct gui_window *gw,
	int x, int y, int height,
	const struct rect *clip)
{
	gw->caret.x = x;
	gw->caret.y = y;
	gw->caret.height = height;
	gw->caret.enabled = true;
	struct nsvi_window *win = gw->window;
	if (win->tabs[win->tab] != gw) {
		return;
	}
	win->mode = INSERT;
	request_frame(win);
}

static struct nsurl *
nsvi_window_url_filter(struct gui_window *gw, struct nsurl *url)
{
	struct nsvi_window *win = gw->window;
	struct nsvi_state *state = win->state;
	struct nsvi_filter *filter = &state->filter;
	nsurl *new;
	nserror err = nsvi_filter_url(filter, nsurl_access(url), &new);
	if (err != NSERROR_OK) {
		return url;
	}
	return new;
}

struct gui_window_table vi_window_table = {
	.create = nsvi_gui_window_create,
	.destroy = nsvi_gui_window_destroy,
	.invalidate = nsvi_window_invalidate,
	.get_scroll = nsvi_window_get_scroll,
	.set_scroll = nsvi_window_set_scroll,
	.get_dimensions = nsvi_window_get_dimensions,
	.event = nsvi_window_event,
	.set_title = nsvi_window_set_title,
	.set_status = nsvi_window_set_status,
	.set_pointer = nsvi_window_set_pointer,
	.place_caret = nsvi_window_place_caret,
	.url_filter = nsvi_window_url_filter,
};

void
gui_window_constrain_scroll(struct gui_window *gw)
{
	int width, height;
	browser_window_get_extents(gw->bw, true, &width, &height);

	struct nsvi_window *win = gw->window;
	width -= win->width;
	height -= win->height - (win->status_height + win->tab_height);

	if (gw->sy > 0) {
		gw->sy = 0;
	}
	if (gw->sy < -height) {
		gw->sy = -height;
	}
	if (gw->sx > 0) {
		gw->sx = 0;
	}
	if (gw->sx < -width) {
		gw->sx = -width;
	}
}

static void
nsvi_window_handle_normal_key(struct nsvi_window *win,
		xkb_keycode_t keycode, bool pressed)
{
	if (!pressed) {
		return;
	}
	xkb_keysym_t sym = xkb_state_key_get_one_sym(
			win->state->xkb_state, keycode);
	uint32_t mask = xkb_state_mask(win->state->xkb_state);
	uint32_t codepoint = xkb_state_key_get_utf32(
			win->state->xkb_state, keycode);
	enum input_key key = codepoint;
	if (mask == MOD_CTRL) {
		switch (sym) {
		case XKB_KEY_a:
			key = NS_KEY_SELECT_ALL;
			break;
		case XKB_KEY_c:
			key = NS_KEY_COPY_SELECTION;
			break;
		case XKB_KEY_r:
			key = NS_KEY_REDO;
			break;
		case XKB_KEY_u:
			key = NS_KEY_UNDO;
			break;
		case XKB_KEY_v:
			key = NS_KEY_PASTE;
			break;
		case XKB_KEY_x:
			key = NS_KEY_CUT_SELECTION;
			break;
		case XKB_KEY_Left:
			key = NS_KEY_WORD_LEFT;
			break;
		case XKB_KEY_Right:
			key = NS_KEY_WORD_RIGHT;
			break;
		default: break;
		}
	} else if (mask == 0) {
		switch (sym) {
		case XKB_KEY_BackSpace:
			key = NS_KEY_DELETE_LEFT;
			break;
		case XKB_KEY_Delete:
			key = NS_KEY_DELETE_RIGHT;
			break;
		case XKB_KEY_Tab:
			key = NS_KEY_TAB;
			break;
		case XKB_KEY_Escape:
			key = NS_KEY_ESCAPE;
			win->mode = NORMAL;
			request_frame(win);
			break;
		case XKB_KEY_Left:
			key = NS_KEY_LEFT;
			break;
		case XKB_KEY_Right:
			key = NS_KEY_RIGHT;
			break;
		case XKB_KEY_Up:
			key = NS_KEY_UP;
			break;
		case XKB_KEY_Down:
			key = NS_KEY_DOWN;
			break;
		case XKB_KEY_Home:
			key = NS_KEY_LINE_START;
			break;
		case XKB_KEY_End:
			key = NS_KEY_LINE_END;
			break;
		case XKB_KEY_Page_Up:
			key = NS_KEY_PAGE_UP;
			break;
		case XKB_KEY_Page_Down:
			key = NS_KEY_PAGE_DOWN;
			break;
		default: break;
		}
	}

	if (key == 0) {
		return;
	}

	browser_window_key_press(win->tabs[win->tab]->bw, key);
}

static bool
isdelim(char d)
{
	return isspace(d) || d == '/';
}

static size_t
exline_next_word_start_backward(struct exline_state *exline)
{
	if (exline->index == 0 || exline->index == 1) {
		return 0;
	}

	size_t index = exline->index;
	while (index != 0 && isdelim(exline->cmd[index - 1])) {
		--index;
	}
	while (index != 0 && !isdelim(exline->cmd[index - 1])) {
		--index;
	}

	return index;
}

static size_t
exline_next_word_start_forward(struct exline_state *exline)
{
	if (exline->index == exline->cmdln || exline->index == exline->cmdln - 1) {
		return exline->cmdln;
	}

	size_t index = exline->index;
	while (index != exline->cmdln && !isdelim(exline->cmd[index])) {
		++index;
	}
	while (index != exline->cmdln && isdelim(exline->cmd[index + 1])) {
		++index;
	}
	if (isdelim(exline->cmd[index])) {
		++index;
	}

	return index;
}

static void
exline_delete_backward(struct exline_state *exline, size_t size)
{
	if (exline->index == 0) {
		return;
	}
	if (size > exline->index) {
		size = exline->index;
	}

	memmove(&exline->cmd[exline->index - size],
		&exline->cmd[exline->index],
		(exline->cmdln - exline->index) + size);
	exline->index -= size;
	exline->cmdln -= size;
}

static void
nsvi_window_exline_paste(struct nsvi_window *win)
{
	struct wl_data_offer *offer = win->state->selected_offer;
	if (!offer) {
		return;
	}

	int fds[2];
	pipe(fds);
	wl_data_offer_receive(offer, "text/plain", fds[1]);
	close(fds[1]);

	// TODO: Async read
	wl_display_roundtrip(win->state->wl_display);

	struct exline_state *exline = &win->exline;
	while (true) {
		char buf[1024];
		ssize_t n = read(fds[0], buf, sizeof(buf));
		if (n <= 0) {
			break;
		}
		if (exline->cmdln + n + 1 >= exline->cmdsz) {
			while (exline->cmdln + n + 1 >= exline->cmdsz) {
				exline->cmdsz *= 2;
			}
			char *new = realloc(exline->cmd, exline->cmdsz);
			assert(new);
			exline->cmd = new;
		}
		memmove(&exline->cmd[exline->index + n],
				&exline->cmd[exline->index],
				(exline->cmdln - exline->index) + 1);
		memcpy(&exline->cmd[exline->index], buf, n);
		exline->index += n;
		exline->cmdln += n;
	}
	close(fds[0]);
	request_frame(win);
}

static void
nsvi_window_handle_exline_key(struct nsvi_window *win,
		xkb_keycode_t keycode, bool pressed)
{
	if (!pressed) {
		return;
	}

	struct exline_state *exline = &win->exline;
	xkb_keysym_t sym = xkb_state_key_get_one_sym(
			win->state->xkb_state, keycode);
	uint32_t codepoint = xkb_state_key_get_utf32(
			win->state->xkb_state, keycode);
	uint32_t mask = xkb_state_mask(win->state->xkb_state);
	if (mask == 0) {
		switch (sym) {
		case XKB_KEY_Return:
			win->mode = NORMAL;
			request_frame(win);
			char *cmd = exline->cmd;
			exline->cmd = NULL;
			exline->cmdsz = 0;
			nsvi_command(win->state, cmd);
			free(cmd);
			return;
		case XKB_KEY_Escape:
			win->mode = NORMAL;
			request_frame(win);
			free(exline->cmd);
			exline->cmd = NULL;
			exline->cmdsz = 0;
			return;
		case XKB_KEY_Left:
			if (exline->index > 0) {
				--exline->index;
				request_frame(win);
			}
			return;
		case XKB_KEY_Right:
			if (exline->index < exline->cmdln) {
				++exline->index;
				request_frame(win);
			}
			return;
		case XKB_KEY_Home:
			exline->index = 0;
			request_frame(win);
			return;
		case XKB_KEY_End:
			exline->index = exline->cmdln;
			request_frame(win);
			return;
		case XKB_KEY_Delete:
			if (exline->index != exline->cmdln) {
				memmove(&exline->cmd[exline->index],
					&exline->cmd[exline->index + 1],
					(exline->cmdln - exline->index) + 1);
				--exline->cmdln;
				request_frame(win);
			}
			return;
		case XKB_KEY_BackSpace:
			exline_delete_backward(exline, 1);
			request_frame(win);
			return;
		default:
			break;
		}
	} else if (mask & MOD_CTRL) {
		// TODO: De-dupe code between different keybindings for the same
		// action
		switch (sym) {
		case XKB_KEY_Left:
			exline->index = exline_next_word_start_backward(exline);
			request_frame(win);
			return;
		case XKB_KEY_Right:
			exline->index = exline_next_word_start_forward(exline);
			request_frame(win);
			return;
		case XKB_KEY_BackSpace:
		case XKB_KEY_Delete:
		case XKB_KEY_w:
			exline_delete_backward(exline, exline->index -
				exline_next_word_start_backward(exline));
			request_frame(win);
			return;
		case XKB_KEY_a:
			exline->index = 0;
			request_frame(win);
			return;
		case XKB_KEY_e:
			exline->index = exline->cmdln;
			request_frame(win);
			return;
		case XKB_KEY_k:
			exline->cmd[exline->index] = '\0';
			exline->cmdln = exline->index;
			request_frame(win);
			return;
		case XKB_KEY_j:
			win->mode = NORMAL;
			request_frame(win);
			char *cmd = exline->cmd;
			exline->cmd = NULL;
			exline->cmdsz = 0;
			nsvi_command(win->state, cmd);
			free(cmd);
			return;
		case XKB_KEY_v:
			nsvi_window_exline_paste(win);
			return;
		case XKB_KEY_h:
			exline_delete_backward(exline, 1);
			request_frame(win);
			return;
		default:
			break;
		}
	} else if (mask & MOD_ALT) {
		switch (sym) {
		case XKB_KEY_b:
			exline->index = exline_next_word_start_backward(exline);
			request_frame(win);
			return;
		case XKB_KEY_f:
			exline->index = exline_next_word_start_forward(exline);
			request_frame(win);
			return;
		default:
			break;
		}
	}

	if (codepoint <= 0x7F && !isprint(codepoint)) {
		return;
	}

	int sz = xkb_state_key_get_utf8(win->state->xkb_state, keycode, NULL, 0);
	if (exline->cmdln + sz + 1 >= exline->cmdsz) {
		exline->cmdsz *= 2;
		char *new = realloc(exline->cmd, exline->cmdsz);
		assert(new);
		exline->cmd = new;
	}
	memmove(&exline->cmd[exline->index + sz],
			&exline->cmd[exline->index],
			(exline->cmdln - exline->index) + 1);
	char buf[9];
	xkb_state_key_get_utf8(win->state->xkb_state, keycode, buf, sizeof(buf));
	memcpy(&exline->cmd[exline->index], buf, sz);
	exline->index += sz;
	exline->cmdln += sz;
	request_frame(win);
}

static void
nsvi_window_finish_hints(struct gui_window *gw)
{
	struct follow_state *follow = &gw->follow;
	for (size_t i = 0; i < follow->nhint; i += 1) {
		free(follow->hints[i].hint);
	}
	free(follow->hints);
	free(follow->hintbuf);
	follow->hints = NULL;
	follow->nhint = 0;
	request_frame(gw->window);
}

static void
nsvi_window_follow_hint(struct nsvi_window *win, struct link_hint *hint)
{
	nserror error = NSERROR_OK;
	enum browser_window_create_flags flags = BW_CREATE_HISTORY | BW_CREATE_TAB;
	struct gui_window *gw = win->tabs[win->tab];
	win->mode = NORMAL;
	struct form_control *gadget = hint->node->gadget;
	nsurl *url = hint->node->href;
	assert(gadget || url);
	if (gadget) {
		switch (gadget->type) {
		case GADGET_SELECT:
			/* TODO select elements unimplemented in visurf */
			break;
		case GADGET_CHECKBOX:
			gadget->selected = !gadget->selected;
			dom_html_input_element_set_checked(
				(dom_html_input_element *)(gadget->node),
				gadget->selected);
			break;
		case GADGET_RADIO:
			form_radio_set(gadget);
			break;
		case GADGET_IMAGE:
		case GADGET_SUBMIT:
			error = form_submit(content_get_url(
				(struct content *) gadget->html),
				gw->bw, gadget->form, gadget);
			break;
		case GADGET_TEXTBOX:
		case GADGET_PASSWORD:
		case GADGET_TEXTAREA:
			html_set_focus(gadget->html, HTML_FOCUS_TEXTAREA,
				(union html_focus_owner) hint->node,
				false, 0, 0, 0, NULL);
			textarea_set_caret(
				gadget->data.text.ta, textarea_get_text(
				gadget->data.text.ta, NULL, 0) - 1);
			break;
		case GADGET_HIDDEN:
			/* not possible */
			break;
		case GADGET_RESET:
			break;
		case GADGET_FILE:
			/* TODO file elements unimplemented in visurf */
			break;
		case GADGET_BUTTON:
			/* This gadget cannot be activated */
			break;
		}

	} else if (url) switch (gw->follow.mode) {
	case FOLLOW_OPEN:
		error = browser_window_navigate(gw->bw, url, NULL,
			BW_NAVIGATE_HISTORY, NULL, NULL, NULL);
		break;
	case FOLLOW_OPEN_TAB:
		if (!gw->follow.background) {
			flags |= BW_CREATE_FOREGROUND;
		}
		error = browser_window_create(flags, url, NULL, gw->bw, NULL);
		break;
	case FOLLOW_OPEN_WINDOW:
		error = browser_window_create(BW_CREATE_HISTORY,
			url, NULL, NULL, NULL);
		break;
	case FOLLOW_YANK:
		nsvi_set_clipboard(win->state, "text/plain",
			nsurl_access(url), strlen(nsurl_access(url)));
		break;
	case FOLLOW_YANK_PRIMARY:
		// TODO
		break;
	}

	if (error != NSERROR_OK) {
		// TODO: Display error
		NSLOG(netsurf, ERROR, "Error following hint");
	}

	nsvi_window_finish_hints(gw);
}

static void
nsvi_window_handle_follow_key(struct nsvi_window *win,
		xkb_keycode_t keycode, bool pressed)
{
	if (!pressed) {
		return;
	}

	// TODO: Select the desired link to follow
	xkb_keysym_t sym = xkb_state_key_get_one_sym(
			win->state->xkb_state, keycode);
	switch (sym) {
	case XKB_KEY_Escape:
		win->mode = NORMAL;
		nsvi_window_finish_hints(win->tabs[win->tab]);
		return;
	}

	struct gui_window *gw = win->tabs[win->tab];
	struct follow_state *follow = &gw->follow;
	uint32_t codepoint = xkb_state_key_get_utf32(
			win->state->xkb_state, keycode);
	if (codepoint <= 0x7F && !isprint(codepoint)) {
		return;
	}
	if (follow->hintln >= follow->needed) {
		return;
	}

	int sz = xkb_state_key_get_utf8(win->state->xkb_state, keycode, NULL, 0);
	xkb_state_key_get_utf8(win->state->xkb_state, keycode,
			&follow->hintbuf[follow->hintln],
			follow->needed + 1);

	bool partial = false;
	for (size_t i = 0; i < follow->nhint; ++i) {
		if (strcmp(follow->hints[i].hint, follow->hintbuf) == 0) {
			nsvi_window_follow_hint(win, &follow->hints[i]);
			return;
		}
		if (strncmp(follow->hints[i].hint,
				follow->hintbuf,
				follow->hintln + 1) == 0) {
			partial = true;
		}
	}

	if (partial) {
		follow->hintln += sz;
	} else {
		// No match, undo keypress
		follow->hintbuf[follow->hintln] = 0;
	}
	request_frame(win);
}

void
nsvi_window_key_event(struct nsvi_window *win,
	xkb_keycode_t keycode, bool pressed)
{
	struct nsvi_state *state = win->state;
	switch (win->mode) {
	case INSERT:
		nsvi_window_handle_normal_key(win, keycode, pressed);
		return;
	case NORMAL:
		nsvi_bindings_handle(&win->state->bindings, keycode, pressed);
		if (!state->quit) {
			request_frame(win);
		}
		return;
	case EXLINE:
		nsvi_window_handle_exline_key(win, keycode, pressed);
		return;
	case FOLLOW:
		nsvi_window_handle_follow_key(win, keycode, pressed);
		return;
	case ERROR:
		if (!pressed) {
			win->mode = NORMAL;
			free(win->error_message);
		}
		return;
	}
}

static bool
nsvi_window_browser_coords(struct nsvi_window *win,
		struct gui_window *gw,
		int surface_x, int surface_y,
		int *browser_x, int *browser_y)
{
	*browser_x = surface_x - gw->sx;
	*browser_y = surface_y - gw->sy - win->tab_height;
	if (surface_y < win->tab_height
			|| surface_y >= (win->height - win->status_height)) {
		return false;
	}
	return true;
}

static void
nsvi_window_pointer_motion_event(struct nsvi_window *win,
		struct pointer_event *event)
{
	struct gui_window *gw = win->tabs[win->tab];
	int pointer_x = wl_fixed_to_int(event->surface_x);
	int pointer_y = wl_fixed_to_int(event->surface_y);
	if ((fabs((float)pointer_x - win->mouse.pointer_x) < 5.0)
			&& (fabs((float)pointer_y - win->mouse.pointer_y) < 5.0)) {
		// Mouse hasn't moved far enough from press coordinate
		// for this to be considered a drag.
		return;
	} else {
		// This is a drag, ensure it's always treated as such,
		// even if we drag back over the press location.
		win->mouse.pointer_x = INT_MIN;
		win->mouse.pointer_y = INT_MIN;
	}

	int bx, by;
	int bpx, bpy;
	if (!nsvi_window_browser_coords(win, gw,
			pointer_x, pointer_y, &bx, &by)) {
		goto exit;
	}
	nsvi_window_browser_coords(win, gw, pointer_x, pointer_y, &bpx, &bpy);
	if (win->mouse.state & BROWSER_MOUSE_PRESS_1) {
		// Start button 1 drag
		browser_window_mouse_click(gw->bw, BROWSER_MOUSE_DRAG_1,
				bpx, bpy);

		// Replace PRESS with HOLDING and declare drag in progress
		win->mouse.state ^= (BROWSER_MOUSE_PRESS_1 | BROWSER_MOUSE_HOLDING_1);
		win->mouse.state |= BROWSER_MOUSE_DRAG_ON;
	} else if (win->mouse.state & BROWSER_MOUSE_PRESS_2) {
		// Start button 2 drag
		browser_window_mouse_click(gw->bw, BROWSER_MOUSE_DRAG_2,
				bpx, bpy);

		// Replace PRESS with HOLDING and declare drag in progress
		win->mouse.state ^= (BROWSER_MOUSE_PRESS_2 | BROWSER_MOUSE_HOLDING_2);
		win->mouse.state |= BROWSER_MOUSE_DRAG_ON;
	}

	// TODO
	// Handle modifiers being removed
	//if (win->mouse.state & BROWSER_MOUSE_MOD_1 && !shift) {
	//	win->mouse.state ^= BROWSER_MOUSE_MOD_1;
	//}
	//if (win->mouse.state & BROWSER_MOUSE_MOD_2 && !ctrl) {
	//	win->mouse.state ^= BROWSER_MOUSE_MOD_2;
	//}

exit:
	browser_window_mouse_track(gw->bw, win->mouse.state, bx, by);
	win->mouse.pointer_x = pointer_x;
	win->mouse.pointer_y = pointer_y;
}

static void
nsvi_window_pointer_button_press_event(struct nsvi_window *win,
		struct pointer_event *event)

{
	struct gui_window *gw = win->tabs[win->tab];
	win->mouse.pressed_x = win->mouse.pointer_x;
	win->mouse.pressed_y = win->mouse.pointer_y;

	if (win->mouse.pressed_y <= win->tab_height) {
		// TODO: This could be more sophisticated
		// grep for 'XXX: tabwidth' for related code
		int tabwidth = win->width / win->ntab;
		int tab = win->mouse.pressed_x / tabwidth;
		if (event->button == BTN_MIDDLE) {
			nsurl *url;
			if (browser_window_get_url(win->tabs[tab]->bw,
			true, &url) == NSERROR_OK) {
				nsvi_undo_tab_new(url);
			}
			browser_window_destroy(win->tabs[tab]->bw);
			return;
		}
		win->tab = tab;
		request_frame(win);
	}

	switch (event->button) {
	case BTN_LEFT:
		win->mouse.state = BROWSER_MOUSE_PRESS_1;
		break;
	case BTN_MIDDLE:
		win->mouse.state = BROWSER_MOUSE_PRESS_2;
		break;
	case BTN_RIGHT:
		// TODO: Show context menu
		break;
	default:
		return;
	}

	// TODO: double & triple clicks
	// TODO: Modifiers

	int bx, by;
	if (!nsvi_window_browser_coords(win, gw,
			win->mouse.pointer_x,
			win->mouse.pointer_y,
			&bx, &by)) {
		return;
	}
	browser_window_mouse_click(gw->bw, win->mouse.state, bx, by);
}

static void
nsvi_window_pointer_button_release_event(struct nsvi_window *win,
		struct pointer_event *event)
{
	struct gui_window *gw = win->tabs[win->tab];

	// If the mouse state is PRESS then we are waiting for a release to emit
	// a click event, otherwise just reset the state to nothing
	if (win->mouse.state & BROWSER_MOUSE_PRESS_1) {
		win->mouse.state ^= (BROWSER_MOUSE_PRESS_1 | BROWSER_MOUSE_CLICK_1);
	} else if (win->mouse.state & BROWSER_MOUSE_PRESS_2) {
		win->mouse.state ^= (BROWSER_MOUSE_PRESS_2 | BROWSER_MOUSE_CLICK_2);
	}

	// TODO: Modifiers

	int bx, by;
	if (!nsvi_window_browser_coords(win, gw,
			win->mouse.pointer_x,
			win->mouse.pointer_y,
			&bx, &by)) {
		goto exit;
	}

	if (win->mouse.state & (BROWSER_MOUSE_CLICK_1 | BROWSER_MOUSE_CLICK_2)) {
		browser_window_mouse_click(gw->bw, win->mouse.state, bx, by);
	} else {
		browser_window_mouse_track(gw->bw, 0, bx, by);
	}

exit:
	win->mouse.state = 0;
}

void
nsvi_window_pointer_event(struct nsvi_window *win, struct pointer_event *event)
{
	struct gui_window *gw = win->tabs[win->tab];

	// Pointer handling is adapted from the GTK version
	if (event->event_mask & POINTER_EVENT_ENTER) {
		win->mouse.serial = event->serial;
		nsvi_window_set_cursor(win);
	}
	if (event->event_mask & POINTER_EVENT_BUTTON) {
		if (event->state == WL_POINTER_BUTTON_STATE_PRESSED) {
			nsvi_window_pointer_button_press_event(win, event);
		} else {
			nsvi_window_pointer_button_release_event(win, event);
		}
	}

	// Exit early if gw->bw has been browser_window_destroy()-ed
	if (gw != win->tabs[win->tab]) {
		return;
	}

	if (event->event_mask & POINTER_EVENT_ENTER
			|| event->event_mask & POINTER_EVENT_MOTION) {
		nsvi_window_pointer_motion_event(win, event);
	}
	int bx, by;
	bool bounds = nsvi_window_browser_coords(win, gw,
			win->mouse.pointer_x,
			win->mouse.pointer_y,
			&bx, &by);
	if (bounds && event->event_mask & POINTER_EVENT_LEAVE) {
		browser_window_mouse_click(gw->bw, BROWSER_MOUSE_LEAVE, bx, by);
	}
	// TODO: More sophsiticated scrolling behavior
	if (event->event_mask & POINTER_EVENT_AXIS) {
		double dy = 0;
		if (event->axes[0].valid) {
			dy = wl_fixed_to_double(event->axes[0].value);
		}
		double dx = 0;
		if (event->axes[1].valid) {
			dx = wl_fixed_to_double(event->axes[1].value);
		}
		dx *= 2 * win->scale;
		dy *= 2 * win->scale;

		if (!browser_window_scroll_at_point(gw->bw, bx, by, dx, dy)) {
			gw->sy -= dy;
			gw->sx -= dx;

			gui_window_constrain_scroll(gw);
			request_frame(win);
		}
	}
}

static void
nsvi_window_touch_scroll(struct nsvi_window *win, struct touch_event *event)
{
	struct nsvi_window_touch *touch = &win->touch;
	struct touch_point *point =
		get_touch_point(win->state, touch->scroll.id);
	assert(point && point->valid);
	if (point->event_mask & TOUCH_EVENT_UP
			|| point->event_mask & TOUCH_EVENT_CANCEL) {
		// XXX: For TOUCH_EVENT_CANCEL we might want to restore the
		// original scroll value
		// TODO: Inertia
		touch->op = TOUCH_NONE;
		return;
	}
	if (!(point->event_mask & TOUCH_EVENT_MOTION)) {
		return;
	}
	int bx, by;
	struct gui_window *gw = win->tabs[win->tab];
	bool bounds = nsvi_window_browser_coords(win, gw,
			touch->down_x, touch->down_y,
			&bx, &by);
	if (!bounds) {
		return;
	}
	int sx = wl_fixed_to_int(point->surface_x),
	    sy = wl_fixed_to_int(point->surface_y);
	int dx = touch->scroll.latest_x - sx, dy = touch->scroll.latest_y - sy;
	if (!browser_window_scroll_at_point(gw->bw, bx, by, dx, dy)) {
		gw->sy -= dy;
		gw->sx -= dx;

		gui_window_constrain_scroll(gw);
		request_frame(win);
	}
	touch->scroll.latest_x = sx;
	touch->scroll.latest_y = sy;
}

static void
nsvi_window_touch_drag(struct nsvi_window *win, struct touch_event *event)
{
	struct nsvi_window_touch *touch = &win->touch;
	struct touch_point *point =
		get_touch_point(win->state, touch->scroll.id);
	assert(point && point->valid);

	int sx = wl_fixed_to_int(point->surface_x),
	    sy = wl_fixed_to_int(point->surface_y);

	int bx, by;
	struct gui_window *gw = win->tabs[win->tab];
	nsvi_window_browser_coords(win, gw, sx, sy, &bx, &by);

	if (point->event_mask & TOUCH_EVENT_UP
			|| point->event_mask & TOUCH_EVENT_CANCEL) {
		browser_window_mouse_track(gw->bw, 0, bx, by);
		touch->op = TOUCH_NONE;
	} else {
		browser_window_mouse_track(gw->bw,
			BROWSER_MOUSE_DRAG_ON | BROWSER_MOUSE_HOLDING_1,
			bx, by);
	}
}

static void
nsvi_window_touch_zoom(struct nsvi_window *win, struct touch_event *event)
{
	// TODO
}

static void
nsvi_window_touch_pending(struct nsvi_window *win, struct touch_event *event)
{
	// In this state, the user has started a touch gesture, but we do not
	// yet know what it is.
	size_t npoint = 0;
	struct touch_point *points[2];
	for (size_t i = 0; i < sizeof(event->points) / sizeof(event->points[0]); ++i) {
		struct touch_point *point = &event->points[i];
		if (!point->valid) {
			continue;
		}
		if (point->wl_surface == win->wl_surface && npoint < 2) {
			points[npoint] = point;
			++npoint;
		}
	}

	int bx, by;
	struct gui_window *gw = win->tabs[win->tab];
	bool bounds = nsvi_window_browser_coords(win, gw,
			win->touch.down_x,
			win->touch.down_y,
			&bx, &by);

	// The following gestures are supported:
	// - Down & up within 1000ms and 25 units: tap
	// - Down, then move greater than 25 units: scroll
	// - Down, hold 1 second, then move: drag
	// - Down, hold 1 second, up: context menu (TODO)
	// - Two fingers: zoom (TODO)
	uint32_t diff = event->time - win->touch.start_time;
	int sx, sy;
	assert(npoint != 0);
	switch (npoint) {
	case 1:
		sx = wl_fixed_to_int(points[0]->surface_x);
		sy = wl_fixed_to_int(points[0]->surface_y);
		int dx = win->touch.down_x - sx, dy = win->touch.down_y - sy;
		int length = (int)sqrt((dx * dx) + (dy * dy));
		if (points[0]->event_mask & TOUCH_EVENT_UP
				|| points[0]->event_mask & TOUCH_EVENT_CANCEL) {
			// Tap interaction?
			if (diff < 1000 && length < 25) {
				if (bounds) {
					browser_window_mouse_click(gw->bw,
						BROWSER_MOUSE_PRESS_1,
						bx, by);
					browser_window_mouse_click(gw->bw,
						BROWSER_MOUSE_CLICK_1,
						bx, by);
					browser_window_mouse_track(gw->bw, 0, bx, by);
				} else {
					// TODO: Have they tapped the UI?
				}
			}
			win->touch.op = TOUCH_NONE;
			return;
		}
		if (length > 25) {
			// Scroll interaction
			win->touch.op = TOUCH_SCROLL;
			win->touch.scroll.id = points[0]->id;
			win->touch.scroll.latest_x = win->touch.down_x;
			win->touch.scroll.latest_y = win->touch.down_y;
		} else if (diff > 1000) {
			// Drag interaction
			win->touch.op = TOUCH_DRAG;
			browser_window_mouse_click(gw->bw,
					BROWSER_MOUSE_PRESS_1,
					bx, by);
			browser_window_mouse_click(gw->bw,
					BROWSER_MOUSE_DRAG_1,
					bx, by);
			browser_window_mouse_track(gw->bw,
					BROWSER_MOUSE_DRAG_ON | BROWSER_MOUSE_HOLDING_1,
					bx, by);
		}
		break;
	case 2:
		NSLOG(netsurf, WARNING, "Initiate zoom interaction");
		break;
	default:
		abort(); // Invariant
	}
}

void
nsvi_window_touch_event(struct nsvi_window *win, struct touch_event *event)
{
	switch (win->touch.op) {
	case TOUCH_NONE:
		// Handled below
		break;
	case TOUCH_PENDING:
		nsvi_window_touch_pending(win, event);
		return;
	case TOUCH_SCROLL:
		nsvi_window_touch_scroll(win, event);
		return;
	case TOUCH_DRAG:
		nsvi_window_touch_drag(win, event);
		return;
	case TOUCH_ZOOM:
		nsvi_window_touch_zoom(win, event);
		return;
	}

	// Store some initial information about the touch gesture
	struct touch_point *initial = NULL;
	for (size_t i = 0; i < sizeof(event->points) / sizeof(event->points[0]); ++i) {
		struct touch_point *point = &event->points[i];
		if (!point->valid) {
			continue;
		}
		if (!(point->event_mask & TOUCH_EVENT_DOWN)) {
			continue;
		}
		if (point->wl_surface == win->wl_surface) {
			initial = point;
		}
	}

	if (!initial) {
		return;
	}

	win->touch.op = TOUCH_PENDING;
	win->touch.start_time = event->time;
	win->touch.down_x = wl_fixed_to_int(initial->surface_x);
	win->touch.down_y = wl_fixed_to_int(initial->surface_y);
}
