#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wayland-client.h>
#include <wayland-cursor.h>
#include <xkbcommon/xkbcommon.h>
#include <neosurf/utils/config.h>
#include <neosurf/utils/messages.h>
#include <neosurf/utils/filepath.h>
#include <neosurf/utils/file.h>
#include <neosurf/utils/nsoption.h>
#include <neosurf/utils/log.h>
#include <neosurf/content/fetch.h>
#include <neosurf/browser_window.h>
#include <neosurf/bitmap.h>
#include <neosurf/clipboard.h>
#include <neosurf/cookie_db.h>
#include <neosurf/fetch.h>
#include <neosurf/layout.h>
#include <neosurf/misc.h>
#include <neosurf/neosurf.h>
#include <neosurf/url_db.h>
#include <neosurf/window.h>
#include <neosurf/utils/errors.h>
#include <neosurf/utils/nsurl.h>
#include "visurf/bitmap.h"
#include "visurf/commands.h"
#include "visurf/fetch.h"
#include "visurf/keybindings.h"
#include "visurf/layout.h"
#include "visurf/plotters.h"
#include "visurf/settings.h"
#include "visurf/timespec-util.h"
#include "visurf/undo.h"
#include "visurf/visurf.h"
#include "visurf/window.h"
#include "xdg-shell.h"
#include "xdg-decoration.h"

#define SLEN(x) (sizeof((x)) - 1)

char **respaths;
struct nsvi_state *global_state;

static void
fatal(const char *why)
{
	fprintf(stderr, "Fatal: %s\n", why);
	exit(1);
}

static nserror
nsvi_schedule_remove(void (*callback)(void *p), void *p)
{
	struct nsvi_callback **prev = &global_state->callbacks;
	struct nsvi_callback *cb = global_state->callbacks;
	nserror r = NSERROR_NOT_FOUND;
	while (cb) {
		if (cb->callback == callback && cb->p == p) {
			struct nsvi_callback *next = cb->next;
			*prev = next;
			free(cb);
			cb = next;
			r = NSERROR_OK;
		} else {
			prev = &cb->next;
			cb = cb->next;
		}
	}
	return r;
}

static nserror
nsvi_misc_schedule(int ms, void (*callback)(void *p), void *p)
{
	nserror r = nsvi_schedule_remove(callback, p);
	if (ms < 0) {
		return r;
	}

	struct nsvi_callback *cb = calloc(1, sizeof(struct nsvi_callback));
	cb->callback = callback;
	cb->p = p;

	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	timespec_add_msec(&cb->due, &now, ms);

	cb->next = global_state->callbacks;
	global_state->callbacks = cb;
	return NSERROR_OK;
}

static struct gui_misc_table vi_misc_table = {
	.schedule = nsvi_misc_schedule,
};

static void
wl_data_source_target(void *data,
	struct wl_data_source *wl_data_source,
	const char *mime_type)
{
	// Not used
}

static void
wl_data_source_send(void *data,
	struct wl_data_source *wl_data_source,
	const char *mime_type,
	int32_t fd)
{
	// TODO: Send asyncronously
	struct nsvi_state *state = data;
	write(fd, state->clipboard, state->cliplen);
	close(fd);
}

static void
wl_data_source_cancelled(void *data, struct wl_data_source *wl_data_source)
{
	struct nsvi_state *state = data;
	wl_data_source_destroy(wl_data_source);
	state->wl_data_source = NULL;
}

static void
wl_data_source_dnd_drop_performed(void *data,
	struct wl_data_source *wl_data_source)
{
	// Not used
}

static void
wl_data_source_dnd_finished(void *data, struct wl_data_source *wl_data_source)
{
	// Not used
}

static void
wl_data_source_action(void *data,
	struct wl_data_source *wl_data_source,
	uint32_t dnd_action)
{
	// Not used
}

static const struct wl_data_source_listener wl_data_source_listener = {
	.target = wl_data_source_target,
	.send = wl_data_source_send,
	.cancelled = wl_data_source_cancelled,
	.dnd_drop_performed = wl_data_source_dnd_drop_performed,
	.dnd_finished = wl_data_source_dnd_finished,
	.action = wl_data_source_action,
};

void
nsvi_set_clipboard(struct nsvi_state *state, const char *type,
		const char *data, size_t len)
{
	struct nsvi_window *win = state->keyboard_focus;
	if (!win) {
		NSLOG(neosurf, WARNING, "Cannot set clipboard without window focus");
		return;
	}
	if (state->wl_data_source) {
		wl_data_source_destroy(state->wl_data_source);
	}
	struct wl_data_source *source = wl_data_device_manager_create_data_source(
			state->wl_data_device_manager);
	wl_data_source_add_listener(source, &wl_data_source_listener, state);
	wl_data_source_offer(source, type);
	if (strncmp(type, "text/", 5) == 0) {
		wl_data_source_offer(source, "text/plain;charset=utf-8");
		wl_data_source_offer(source, "STRING");
		wl_data_source_offer(source, "TEXT");
		wl_data_source_offer(source, "UTF8_STRING");
	}
	state->clipboard = strdup(data);
	state->cliplen = len;
	state->wl_data_source = source;
	wl_data_device_set_selection(state->wl_data_device,
			source, win->keyboard_enter_serial);
}

static void
nsvi_clipboard_get(char **buffer, size_t *length)
{
	struct wl_data_offer *offer = global_state->selected_offer;
	if (!offer) {
		return;
	}

	int fds[2];
	pipe(fds);
	wl_data_offer_receive(offer, "text/plain", fds[1]);
	close(fds[1]);

	// TODO: Async read
	wl_display_roundtrip(global_state->wl_display);

	size_t datasz = 1024, dataln = 0;
	char *data = calloc(1, datasz);
	while (true) {
		char buf[1024];
		ssize_t n = read(fds[0], buf, sizeof(buf));
		if (n <= 0) {
			break;
		}
		while (dataln + n >= datasz) {
			datasz *= 2;
			data = realloc(data, datasz);
			assert(data);
		}
		memcpy(&data[dataln], buf, n);
		dataln += n;
	}
	close(fds[0]);
	*buffer = data;
	*length = dataln;
}

static void
nsvi_clipboard_set(const char *buffer, size_t length,
	nsclipboard_styles styles[], int n_styles)
{
	// TODO: Support rich text copy?
	nsvi_set_clipboard(global_state, "text/plain", buffer, length);
}

static struct gui_clipboard_table vi_clipboard_table = {
	.get = nsvi_clipboard_get,
	.set = nsvi_clipboard_set,
};

static void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
	xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
	.ping = xdg_wm_base_ping,
};

static void
wl_pointer_enter(void *data, struct wl_pointer *wl_pointer,
		uint32_t serial, struct wl_surface *surface,
		wl_fixed_t surface_x, wl_fixed_t surface_y)
{
	struct nsvi_state *state = data;
	state->pointer_event.event_mask |= POINTER_EVENT_ENTER;
	state->pointer_event.serial = serial;
	state->pointer_event.surface = surface;
	state->pointer_event.surface_x = surface_x,
		state->pointer_event.surface_y = surface_y;
}

static void
wl_pointer_leave(void *data, struct wl_pointer *wl_pointer,
		uint32_t serial, struct wl_surface *surface)
{
	struct nsvi_state *state = data;
	state->pointer_event.serial = serial;
	state->pointer_event.surface = surface;
	state->pointer_event.event_mask |= POINTER_EVENT_LEAVE;
}

static void
wl_pointer_motion(void *data, struct wl_pointer *wl_pointer, uint32_t time,
		wl_fixed_t surface_x, wl_fixed_t surface_y)
{
	struct nsvi_state *state = data;
	state->pointer_event.event_mask |= POINTER_EVENT_MOTION;
	state->pointer_event.time = time;
	state->pointer_event.surface_x = surface_x,
		state->pointer_event.surface_y = surface_y;
}

static void
wl_pointer_button(void *data, struct wl_pointer *wl_pointer, uint32_t serial,
		uint32_t time, uint32_t button, uint32_t state)
{
	struct nsvi_state *vistate = data;
	vistate->pointer_event.event_mask |= POINTER_EVENT_BUTTON;
	vistate->pointer_event.time = time;
	vistate->pointer_event.serial = serial;
	vistate->pointer_event.button = button,
		vistate->pointer_event.state = state;
}

static void
wl_pointer_axis(void *data, struct wl_pointer *wl_pointer, uint32_t time,
		uint32_t axis, wl_fixed_t value)
{
	struct nsvi_state *state = data;
	state->pointer_event.event_mask |= POINTER_EVENT_AXIS;
	state->pointer_event.time = time;
	state->pointer_event.axes[axis].valid = true;
	state->pointer_event.axes[axis].value = value;
}

static void
wl_pointer_frame(void *data, struct wl_pointer *wl_pointer)
{
	struct nsvi_state *state = data;
	struct pointer_event *event = &state->pointer_event;

	struct nsvi_window *win = state->pointer_focus;
	if (event->event_mask & POINTER_EVENT_ENTER) {
		for (win = state->windows; win; win = win->next) {
			if (win->wl_surface == event->surface) {
				state->pointer_focus = win;
				break;
			}
		}
	}
	if (event->event_mask & POINTER_EVENT_LEAVE) {
		state->pointer_focus = NULL;
	}
	if (win != NULL) {
		nsvi_window_pointer_event(win, event);
	}

	memset(event, 0, sizeof(*event));
}

static void
wl_pointer_axis_source(void *data, struct wl_pointer *wl_pointer,
		uint32_t axis_source)
{
	struct nsvi_state *state = data;
	state->pointer_event.event_mask |= POINTER_EVENT_AXIS_SOURCE;
	state->pointer_event.axis_source = axis_source;
}

static void
wl_pointer_axis_stop(void *data, struct wl_pointer *wl_pointer,
		uint32_t time, uint32_t axis)
{
	struct nsvi_state *state = data;
	state->pointer_event.time = time;
	state->pointer_event.event_mask |= POINTER_EVENT_AXIS_STOP;
	state->pointer_event.axes[axis].valid = true;
}

static void
wl_pointer_axis_discrete(void *data, struct wl_pointer *wl_pointer,
		uint32_t axis, int32_t discrete)
{
	struct nsvi_state *state = data;
	state->pointer_event.event_mask |= POINTER_EVENT_AXIS_STOP;
	state->pointer_event.axes[axis].valid = true;
	state->pointer_event.axes[axis].discrete = discrete;
}

static const struct wl_pointer_listener wl_pointer_listener = {
	.enter = wl_pointer_enter,
	.leave = wl_pointer_leave,
	.motion = wl_pointer_motion,
	.button = wl_pointer_button,
	.axis = wl_pointer_axis,
	.frame = wl_pointer_frame,
	.axis_source = wl_pointer_axis_source,
	.axis_stop = wl_pointer_axis_stop,
	.axis_discrete = wl_pointer_axis_discrete,
};

static void
wl_keyboard_keymap(void *data, struct wl_keyboard *wl_keyboard,
		uint32_t format, int32_t fd, uint32_t size)
{
	struct nsvi_state *state = data;
	assert(format == WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1);

	char *map_shm = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
        if ((void *) -1 == map_shm) {
            printf("Could not map memory: %d %s\n", errno,strerror(errno));
            exit(1);
        }

	struct xkb_keymap *xkb_keymap = xkb_keymap_new_from_string(
			state->xkb_context, map_shm,
			XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
	munmap(map_shm, size);
	close(fd);

	struct xkb_state *xkb_state = xkb_state_new(xkb_keymap);
	xkb_keymap_unref(state->xkb_keymap);
	xkb_state_unref(state->xkb_state);
	state->xkb_keymap = xkb_keymap;
	state->xkb_state = xkb_state;
	state->bindings.xkb_state = xkb_state;
}

static void
wl_keyboard_enter(void *data, struct wl_keyboard *wl_keyboard,
		uint32_t serial, struct wl_surface *surface,
		struct wl_array *keys)
{
	struct nsvi_state *state = data;
	for (struct nsvi_window *win = state->windows; win; win = win->next) {
		if (win->wl_surface == surface) {
			state->keyboard_focus = win;
			win->keyboard_enter_serial = serial;
		}
	}
}

static void
wl_keyboard_leave(void *data, struct wl_keyboard *wl_keyboard,
		uint32_t serial, struct wl_surface *surface)
{
	struct nsvi_state *state = data;
	state->keyboard_focus = NULL;
}

static void
repeat_key(void *data)
{
	struct nsvi_state *state = data;
	struct nsvi_window *win = state->keyboard_focus;
	if (!win) {
		return;
	}
	nsvi_window_key_event(win, state->repeat_key, true);
	nsvi_misc_schedule(1000 / state->repeat_rate, &repeat_key, state);
}

static void
wl_keyboard_key(void *data, struct wl_keyboard *wl_keyboard,
		uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
{
	struct nsvi_state *vistate = data;
	struct nsvi_window *win = vistate->keyboard_focus;
	assert(win);

	bool pressed = state == WL_KEYBOARD_KEY_STATE_PRESSED;
	uint32_t keycode = key + 8;
	nsvi_window_key_event(win, keycode, pressed);

	if (pressed && vistate->repeat_rate != 0
			&& xkb_keymap_key_repeats(vistate->xkb_keymap, keycode)) {
		vistate->repeat_key = keycode;
		nsvi_misc_schedule(vistate->repeat_delay, &repeat_key, vistate);
	} else {
		nsvi_misc_schedule(-1, &repeat_key, vistate);
	}
}

static void
wl_keyboard_modifiers(void *data, struct wl_keyboard *wl_keyboard,
		uint32_t serial, uint32_t mods_depressed,
		uint32_t mods_latched, uint32_t mods_locked,
		uint32_t group)
{
	struct nsvi_state *state = data;
	xkb_state_update_mask(state->xkb_state,
		mods_depressed, mods_latched, mods_locked, 0, 0, group);
}

static void
wl_keyboard_repeat_info(void *data, struct wl_keyboard *wl_keyboard,
		int32_t rate, int32_t delay)
{
	struct nsvi_state *state = data;
	state->repeat_rate = rate;
	state->repeat_delay = delay;
}

static const struct wl_keyboard_listener wl_keyboard_listener = {
	.keymap = wl_keyboard_keymap,
	.enter = wl_keyboard_enter,
	.leave = wl_keyboard_leave,
	.key = wl_keyboard_key,
	.modifiers = wl_keyboard_modifiers,
	.repeat_info = wl_keyboard_repeat_info,
};

struct touch_point *
get_touch_point(struct nsvi_state *state, int32_t id)
{
	struct touch_event *touch = &state->touch_event;
	const size_t nmemb = sizeof(touch->points) / sizeof(struct touch_point);
	int invalid = -1;
	for (size_t i = 0; i < nmemb; ++i) {
		if (touch->points[i].id == id) {
			touch->points[i].valid = true;
			return &touch->points[i];
		}
		if (invalid == -1 && !touch->points[i].valid) {
			invalid = i;
		}
	}
	if (invalid == -1) {
		return NULL;
	}
	touch->points[invalid].valid = true;
	touch->points[invalid].id = id;
	return &touch->points[invalid];
}

static void
wl_touch_down(void *data, struct wl_touch *wl_touch, uint32_t serial,
		uint32_t time, struct wl_surface *surface, int32_t id,
		wl_fixed_t x, wl_fixed_t y)
{
	struct nsvi_state *state = data;
	struct touch_point *point = get_touch_point(state, id);
	if (point == NULL) {
		return;
	}
	point->event_mask |= TOUCH_EVENT_DOWN;
	point->wl_surface = surface;
	point->surface_x = x, point->surface_y = y;
	state->touch_event.time = time;
	state->touch_event.serial = serial;
}

static void
wl_touch_up(void *data, struct wl_touch *wl_touch, uint32_t serial,
		uint32_t time, int32_t id)
{
	struct nsvi_state *state = data;
	struct touch_point *point = get_touch_point(state, id);
	if (point == NULL) {
		return;
	}
	point->event_mask |= TOUCH_EVENT_UP;
}

static void
wl_touch_motion(void *data, struct wl_touch *wl_touch, uint32_t time,
		int32_t id, wl_fixed_t x, wl_fixed_t y)
{
	struct nsvi_state *state = data;
	struct touch_point *point = get_touch_point(state, id);
	if (point == NULL) {
		return;
	}
	point->event_mask |= TOUCH_EVENT_MOTION;
	point->surface_x = x, point->surface_y = y;
	state->touch_event.time = time;
}

static void
wl_touch_frame(void *data, struct wl_touch *wl_touch)
{
	struct nsvi_state *state = data;
	struct touch_event *event = &state->touch_event;
	for (struct nsvi_window *win = state->windows; win; win = win->next) {
		nsvi_window_touch_event(win, event);
	}
	if (event->event_mask & TOUCH_EVENT_CANCEL) {
		*event = (struct touch_event){0};
		return;
	}
	for (size_t i = 0; i < sizeof(event->points) / sizeof(event->points[0]);
			++i) {
		struct touch_point *point = &event->points[i];
		if (point->event_mask & TOUCH_EVENT_UP) {
			*point = (struct touch_point){0};
		}
	}
}

static void
wl_touch_cancel(void *data, struct wl_touch *wl_touch)
{
	struct nsvi_state *state = data;
	state->touch_event.event_mask |= TOUCH_EVENT_CANCEL;
}

static void
wl_touch_shape(void *data, struct wl_touch *wl_touch,
		int32_t id, wl_fixed_t major, wl_fixed_t minor)
{
	struct nsvi_state *state = data;
	struct touch_point *point = get_touch_point(state, id);
	if (point == NULL) {
		return;
	}
	point->event_mask |= TOUCH_EVENT_SHAPE;
	point->major = major, point->minor = minor;
}

static void
wl_touch_orientation(void *data, struct wl_touch *wl_touch,
		int32_t id, wl_fixed_t orientation)
{
	struct nsvi_state *state = data;
	struct touch_point *point = get_touch_point(state, id);
	if (point == NULL) {
		return;
	}
	point->event_mask |= TOUCH_EVENT_ORIENTATION;
	point->orientation = orientation;
}

static const struct wl_touch_listener wl_touch_listener = {
	.down = wl_touch_down,
	.up = wl_touch_up,
	.motion = wl_touch_motion,
	.frame = wl_touch_frame,
	.cancel = wl_touch_cancel,
	.shape = wl_touch_shape,
	.orientation = wl_touch_orientation,
};

static void
wl_seat_capabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities)
{
	struct nsvi_state *state = data;
	bool have_pointer = capabilities & WL_SEAT_CAPABILITY_POINTER,
		have_keyboard = capabilities & WL_SEAT_CAPABILITY_KEYBOARD,
		have_touch = capabilities & WL_SEAT_CAPABILITY_TOUCH;

	if (have_pointer && state->wl_pointer == NULL) {
		state->wl_pointer = wl_seat_get_pointer(state->wl_seat);
		wl_pointer_add_listener(state->wl_pointer,
				&wl_pointer_listener, state);
	} else if (!have_pointer && state->wl_pointer != NULL) {
		wl_pointer_release(state->wl_pointer);
		state->wl_pointer = NULL;
	}

	if (have_keyboard && state->wl_keyboard == NULL) {
		state->wl_keyboard = wl_seat_get_keyboard(state->wl_seat);
		wl_keyboard_add_listener(state->wl_keyboard,
				&wl_keyboard_listener, state);
	} else if (!have_keyboard && state->wl_keyboard != NULL) {
		wl_keyboard_release(state->wl_keyboard);
		state->wl_keyboard = NULL;
	}

	if (have_touch && state->wl_touch == NULL) {
		state->wl_touch = wl_seat_get_touch(state->wl_seat);
		wl_touch_add_listener(state->wl_touch,
				&wl_touch_listener, state);
	} else if (!have_touch && state->wl_touch != NULL) {
		wl_touch_release(state->wl_touch);
		state->wl_touch = NULL;
	}
}

static void
wl_seat_name(void *data, struct wl_seat *wl_seat, const char *name)
{
	// This space deliberately left blank
}

static const struct wl_seat_listener wl_seat_listener = {
	.capabilities = wl_seat_capabilities,
	.name = wl_seat_name,
};

static void
wl_output_geometry(void *data, struct wl_output *wl_output,
	int32_t x, int32_t y, int32_t physical_width, int32_t physical_height,
	int32_t subpixel, const char *make, const char *model,
	int32_t transform)
{
	// TODO: We could pass the subpixel info on to pango
	// TODO: Compute DPI and give it to core
}

static void
wl_output_mode(void *data, struct wl_output *wl_output,
     uint32_t flags, int32_t width, int32_t height, int32_t refresh)
{
	// This space deliberately left blank
}

static void
wl_output_done(void *data, struct wl_output *wl_output)
{
	// This space deliberately left blank
}

static void
wl_output_scale(void *data, struct wl_output *wl_output, int32_t factor)
{
	struct nsvi_output *output = data;
	output->scale = factor;
}

static const struct wl_output_listener wl_output_listener = {
	.geometry = wl_output_geometry,
	.mode = wl_output_mode,
	.done = wl_output_done,
	.scale = wl_output_scale,
};

static void wl_data_offer_offer(void *data,
	struct wl_data_offer *offer,
	const char *mime_type)
{
	struct nsvi_state *state = data;
	if (strcmp(mime_type, "text/plain") == 0) {
		state->wl_data_offer = offer;
	}
}

/* Notification of actions available to D&D sources */

static void wl_data_offer_source_actions(void *data,
                                       struct wl_data_offer *offer, uint32_t source_actions)
{
  printf("wl_data_offer # source_actions | offer:%p, source_actions:", offer);
  printf("\n");
}

/* Server-selected action will be notified (for drop destination) */

static void wl_data_offer_action(void *data, struct wl_data_offer *offer, uint32_t dnd_action)
{
  printf("wl_data_offer # action | offer:%p, dnd_action:", offer);
  printf("\n");
}

static const struct wl_data_offer_listener wl_data_offer_listener = {
	.offer = wl_data_offer_offer,
  .action = wl_data_offer_action,
  .source_actions = wl_data_offer_source_actions,
};

static void
wl_data_device_data_offer(void *data,
		struct wl_data_device *data_device,
		struct wl_data_offer *offer)
{
	wl_data_offer_add_listener(offer, &wl_data_offer_listener, data);
}

static void
wl_data_device_enter(void *data, struct wl_data_device *wl_data_device,
	uint32_t serial, struct wl_surface *surface,
	wl_fixed_t x, wl_fixed_t y,
	struct wl_data_offer *id)
{
	// TODO
}

static void
wl_data_device_leave(void *data, struct wl_data_device *wl_data_device)
{
	// TODO
}

static void
wl_data_device_motion(void *data, struct wl_data_device *wl_data_device,
       uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
	// TODO
}

static void
wl_data_device_drop(void *data, struct wl_data_device *wl_data_device)
{
	// TODO
}

static void
wl_data_device_selection(void *data,
	struct wl_data_device *wl_data_device,
	struct wl_data_offer *id)
{
	struct nsvi_state *state = data;
	if (id == state->wl_data_offer) {
		state->selected_offer = id;
	} else {
		// No text/plain offer selected
		state->selected_offer = NULL;
		return;
	}
}

static const struct wl_data_device_listener wl_data_device_listener = {
	.data_offer = wl_data_device_data_offer,
	.enter = wl_data_device_enter,
	.leave = wl_data_device_leave,
	.motion = wl_data_device_motion,
	.drop = wl_data_device_drop,
	.selection = wl_data_device_selection,
};

static void
registry_global(void *data, struct wl_registry *wl_registry,
	uint32_t name, const char *interface, uint32_t version)
{
	struct nsvi_state *state = data;
	if (strcmp(interface, wl_shm_interface.name) == 0) {
		state->wl_shm = wl_registry_bind(wl_registry,
				name, &wl_shm_interface, 1);
	} else if (strcmp(interface, wl_compositor_interface.name) == 0) {
		state->wl_compositor = wl_registry_bind(wl_registry,
				name, &wl_compositor_interface, 4);
	} else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
		state->xdg_wm_base = wl_registry_bind(wl_registry,
				name, &xdg_wm_base_interface, 1);
		xdg_wm_base_add_listener(state->xdg_wm_base,
				&xdg_wm_base_listener, state);
	} else if (strcmp(interface, zxdg_decoration_manager_v1_interface.name) == 0) {
		state->xdg_decoration_manager = wl_registry_bind(wl_registry,
				name, &zxdg_decoration_manager_v1_interface, 1);
	} else if (strcmp(interface, wl_seat_interface.name) == 0) {
		state->wl_seat = wl_registry_bind(wl_registry,
				name, &wl_seat_interface, 7);
		wl_seat_add_listener(state->wl_seat,
				&wl_seat_listener, state);
	} else if (strcmp(interface, wl_output_interface.name) == 0) {
		struct nsvi_output *output =
			calloc(1, sizeof(struct nsvi_output));
		output->wl_output = wl_registry_bind(wl_registry,
				name, &wl_output_interface, 3);
		wl_output_add_listener(output->wl_output,
				&wl_output_listener, output);
		output->next = state->outputs;
		state->outputs = output;
	} else if (strcmp(interface, wl_data_device_manager_interface.name) == 0) {
		state->wl_data_device_manager = wl_registry_bind(wl_registry,
				name, &wl_data_device_manager_interface, 3);
	}
}

static void
registry_global_remove(void *data,
	struct wl_registry *wl_registry, uint32_t name)
{
	/* This space deliberately left blank */
}

static const struct wl_registry_listener wl_registry_listener = {
	.global = registry_global,
	.global_remove = registry_global_remove,
};

char *config_home;

static nserror
check_dirname(const char *path, const char *leaf, char **dirname_out)
{
	nserror ret;
	char *dirname = NULL;
	struct stat dirname_stat;

	ret = neosurf_mkpath(&dirname, NULL, 2, path, leaf);
	if (ret != NSERROR_OK) {
		return ret;
	}

	if (stat(dirname, &dirname_stat) == 0) {
		if (S_ISDIR(dirname_stat.st_mode)) {
			if (access(dirname, R_OK | W_OK) == 0) {
				*dirname_out = dirname;
				return NSERROR_OK;
			} else {
				ret = NSERROR_PERMISSION;
			}
		} else {
			ret = NSERROR_NOT_DIRECTORY;
		}
	} else {
		ret = NSERROR_NOT_FOUND;
	}

	free(dirname);

	return ret;
}

static nserror
get_config_home(char **config_home_out)
{
	nserror ret;
	char *home_dir;
	char *xdg_config_dir;
	char *config_home;

	home_dir = getenv("HOME");
	xdg_config_dir = getenv("XDG_CONFIG_HOME");

	if ((xdg_config_dir == NULL) || (*xdg_config_dir == 0)) {
		if (home_dir == NULL) {
			return NSERROR_NOT_DIRECTORY;
		}

		ret = check_dirname(home_dir, ".config/neosurf", &config_home);
		if (ret != NSERROR_OK) {
			return ret;
		}
	} else {
		ret = check_dirname(xdg_config_dir, "neosurf", &config_home);
		if (ret != NSERROR_OK) {
			return ret;
		}
	}

	NSLOG(neosurf, INFO, "\"%s\"", config_home);

	*config_home_out = config_home;
	return NSERROR_OK;
}

static nserror
create_config_home(char **config_home_out)
{
	nserror ret;
	char *home_dir;
	char *xdg_config_dir;
	char *config_home;
	char *config_home_trailing;

	home_dir = getenv("HOME");
	xdg_config_dir = getenv("XDG_CONFIG_HOME");
	config_home = NULL;

	if ((xdg_config_dir == NULL) || (*xdg_config_dir == 0)) {
		if ((home_dir == NULL) || (*home_dir == 0)) {
			return NSERROR_NOT_DIRECTORY;
		}

		ret = neosurf_mkpath(&config_home, NULL, 3, home_dir, ".config", "neosurf");
	} else {
		ret = neosurf_mkpath(&config_home, NULL, 2, xdg_config_dir, "neosurf");
	}

	if (ret != NSERROR_OK) {
		return ret;
	}

	// For creating the directory a trailing / is required.
	config_home_trailing = NULL;
	ret = neosurf_mkpath(&config_home_trailing, NULL, 2, config_home, "/");
	if (ret != NSERROR_OK) {
		return ret;
	}

	ret = neosurf_mkdir_all(config_home_trailing);
	free(config_home_trailing);
	if (ret != NSERROR_OK) {
		return ret;
	}

	*config_home_out = config_home;

	return NSERROR_OK;
}

static nserror
set_defaults(struct nsoption_s *defaults)
{
	char *fname;
	nserror ret;

	config_home = NULL;
	ret = get_config_home(&config_home);
	if (ret == NSERROR_NOT_FOUND) {
		if (config_home != NULL) {
			free(config_home);
		}
		ret = create_config_home(&config_home);
	}
	if (ret != NSERROR_OK) {
		return ret;
	}

	fname = NULL;
	neosurf_mkpath(&fname, NULL, 2, config_home, "Cookies");
	if (fname != NULL) {
		nsoption_setnull_charp(cookie_file, fname);
	}

	fname = NULL;
	neosurf_mkpath(&fname, NULL, 2, config_home, "Cookies");
	if (fname != NULL) {
		nsoption_setnull_charp(cookie_jar, fname);
	}

	/* url database default */
	fname = NULL;
	neosurf_mkpath(&fname, NULL, 2, config_home, "URLs");
	if (fname != NULL) {
		nsoption_setnull_charp(url_file, fname);
	}

	/* bookmark database default */
	fname = NULL;
	neosurf_mkpath(&fname, NULL, 2, config_home, "Hotlist");
	if (fname != NULL) {
		nsoption_setnull_charp(hotlist_path, fname);
	}

	/* download directory default */
	fname = getenv("HOME");
	if (fname != NULL) {
		nsoption_setnull_charp(downloads_directory, strdup(fname));
	}

	// Default fonts
	nsoption_set_charp(font_sans, strdup("Sans"));
	nsoption_set_charp(font_serif, strdup("Serif"));
	nsoption_set_charp(font_mono, strdup("Monospace"));
	nsoption_set_charp(font_cursive, strdup("Serif"));
	nsoption_set_charp(font_fantasy, strdup("Serif"));
	return NSERROR_OK;
}

/** maximum number of languages in language vector */
#define LANGV_SIZE 32
/** maximum length of all strings in language vector */
#define LANGS_SIZE 4096

static const char *
get_language(void)
{
	const char *lang;
	lang = getenv("LANGUAGE");
	if ((lang != NULL) && (lang[0] != '\0')) {
		return lang;
	}

	lang = getenv("LC_ALL");
	if ((lang != NULL) && (lang[0] != '\0')) {
		return lang;
	}

	lang = getenv("LC_MESSAGES");
	if ((lang != NULL) && (lang[0] != '\0')) {
		return lang;
	}

	lang = getenv("LANG");
	if ((lang != NULL) && (lang[0] != '\0')) {
		return lang;
	}
	return NULL;
}

static const char * const *
get_languagev(void)
{
	static const char *langv[LANGV_SIZE];
	int langidx = 0;
	static char langs[LANGS_SIZE];
	char *curp;
	const char *lange;
	int lang_len;
	char *cln;

	if (langv[0] != NULL) {
		return &langv[0];
	}

	curp = &langs[0];

	lange = get_language();

	if (lange != NULL) {
		lang_len = strlen(lange) + 1;
		if (lang_len < (LANGS_SIZE - 2)) {
			memcpy(curp, lange, lang_len);
			while ((curp[0] != 0) &&
			       (langidx < (LANGV_SIZE - 2))) {
				cln = strchr(curp, ':');
				if (cln == NULL) {
					langv[langidx++] = curp;
					curp += lang_len;
					break;
				} else {
					if ((cln - curp) > 1) {
						langv[langidx++] = curp;
					}
					*cln++ = 0;
					lang_len -= (cln - curp);
					curp = cln;
				}
			}
		}
	}

	langv[langidx++] = curp;
	*curp++ = 'C';
	*curp++ = 0;
	langv[langidx] = NULL;
	return &langv[0];
}

static bool
nslog_stream_configure(FILE *fptr)
{
	setbuf(fptr, NULL);
	return true;
}

static char **
nsvi_init_resource(const char *resource_path)
{
	const char * const *langv = get_languagev();
	char **pathv = filepath_path_to_strvec(resource_path);
	char **respath = filepath_generate(pathv, langv);
	filepath_free_strvec(pathv);
	return respath;
}

static void
nsvi_init_bindings(struct nsvi_state *state, struct nsvi_bindings *bindings)
{
	state->bindings.exec = nsvi_command;
	state->bindings.user = state;
	nsvi_bindings_new(bindings, "<colon>", "exline");
	nsvi_bindings_new(bindings, "i", "insert");
	nsvi_bindings_new(bindings, "<C-q>", "close");
	nsvi_bindings_new(bindings, "<C-w>", "close");
	nsvi_bindings_new(bindings, "h", "scroll -h 20+");
	nsvi_bindings_new(bindings, "j", "scroll 20-");
	nsvi_bindings_new(bindings, "k", "scroll 20+");
	nsvi_bindings_new(bindings, "l", "scroll -h 20-");
	nsvi_bindings_new(bindings, "H", "back");
	nsvi_bindings_new(bindings, "L", "forward");
	nsvi_bindings_new(bindings, "r", "reload");
	nsvi_bindings_new(bindings, "R", "reload -f");
	nsvi_bindings_new(bindings, "o", "exline 'open '");
	nsvi_bindings_new(bindings, "O", "exline 'open -t '");
	nsvi_bindings_new(bindings, "<C-t>", "open -t about:welcome");
	nsvi_bindings_new(bindings, "go", "exline \"open $url\"");
	nsvi_bindings_new(bindings, "gO", "exline \"open -t $url\"");
	nsvi_bindings_new(bindings, "<Left>", "scroll -h 20+");
	nsvi_bindings_new(bindings, "<Down>", "scroll 20-");
	nsvi_bindings_new(bindings, "<Up>", "scroll 20+");
	nsvi_bindings_new(bindings, "<Right>", "scroll -h 20-");
	nsvi_bindings_new(bindings, "<C-d>", "scroll 50%-");
	nsvi_bindings_new(bindings, "<C-u>", "scroll 50%+");
	nsvi_bindings_new(bindings, "gg", "scroll -p 0%");
	nsvi_bindings_new(bindings, "G", "scroll -p 100%");
	nsvi_bindings_new(bindings, "<Home>", "scroll -p 0%");
	nsvi_bindings_new(bindings, "<End>", "scroll -p 100%");
	nsvi_bindings_new(bindings, "<space>", "scroll 100%-");
	nsvi_bindings_new(bindings, "<plus>", "zoom 10%+");
	nsvi_bindings_new(bindings, "<minus>", "zoom 10%-");
	nsvi_bindings_new(bindings, "<equal>", "zoom 1");
	nsvi_bindings_new(bindings, "gt", "tab next");
	nsvi_bindings_new(bindings, "J", "tab next");
	nsvi_bindings_new(bindings, "gT", "tab prev");
	nsvi_bindings_new(bindings, "K", "tab prev");
	nsvi_bindings_new(bindings, "<C-Prior>", "tab prev");
	nsvi_bindings_new(bindings, "<C-Next>", "tab next");
	nsvi_bindings_new(bindings, "<Next>", "scroll 100%-");
	nsvi_bindings_new(bindings, "<Prior>", "scroll 100%+");
	nsvi_bindings_new(bindings, "f", "follow");
	nsvi_bindings_new(bindings, "F", "follow -tb");
	nsvi_bindings_new(bindings, "yy", "yank");
	nsvi_bindings_new(bindings, "yt", "yank -t");
	nsvi_bindings_new(bindings, "Yy", "yank -p");
	nsvi_bindings_new(bindings, "Yt", "yank -pt");
	nsvi_bindings_new(bindings, "<C-c>", "yank -s");
	nsvi_bindings_new(bindings, "pp", "paste");
	nsvi_bindings_new(bindings, "Pp", "paste -t");
	nsvi_bindings_new(bindings, "pP", "paste -p");
	nsvi_bindings_new(bindings, "PP", "paste -pt");
	nsvi_bindings_new(bindings, "u", "undo");
	nsvi_bindings_new(bindings, "<slash>", "exline \"search \"");
	nsvi_bindings_new(bindings, "n", "search -n");
	nsvi_bindings_new(bindings, "N", "search -p");
	nsvi_bindings_new(bindings, "<bracketleft><bracketleft>", "page prev");
	nsvi_bindings_new(bindings, "<bracketright><bracketright>", "page next");
}

static void
nsvi_load_config(struct nsvi_state *state)
{
	assert(config_home != NULL);
	char *config = NULL;
	neosurf_mkpath(&config, NULL, 2, config_home, "nsvirc");
	if (config != NULL) {
		nsvi_command_source(state, config);
		free(config);
	}
}

nserror
nsvi_filter_url(struct nsvi_filter *filter, const char *in, nsurl **out)
{
	nsurl *new;
	nserror err;
	if (!filter->in) {
		goto orig;
	}
	size_t r = fprintf(filter->in, "%s\n", in);
	if (r == 0) {
		if (ferror(filter->in) != 0) {
			NSLOG(neosurf, ERROR,
				"Error writing to URL filter: %s",
				strerror(ferror(filter->in)));
		}
		if (feof(filter->in)) {
			NSLOG(neosurf, ERROR, "Unexpected EOF from URL filter");
		}
		goto error;
	}
	ssize_t n = getline(&filter->buf, &filter->bufsz, filter->out);
	if (n == -1) {
		NSLOG(neosurf, ERROR,
			"Error reading from URL filter: %s",
			strerror(errno));
		goto error;
	}
	err = nsurl_create(filter->buf, &new);
	if (err != NSERROR_OK) {
		NSLOG(neosurf, ERROR,
			"Error parsing '%s' from URL filter",
			filter->buf);
		goto error;
	}
	*out = new;
	return NSERROR_OK;
error:
	fclose(filter->in);
	fclose(filter->out);
	filter->in = NULL;
	filter->out = NULL;
	kill(filter->pid, SIGTERM);
orig:
	err = nsurl_create(in, &new);
	*out = new;
	return err;
}

int
main(int argc, char *argv[]) {
	struct nsvi_state state = {0};
	global_state = &state;

	state.xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
	nsvi_init_bindings(&state, &state.bindings);

	nsvi_config_init();

	state.wl_display = wl_display_connect(NULL);
	if (!state.wl_display) {
		fatal("Error connecting to Wayland display");
	}
	state.wl_registry = wl_display_get_registry(state.wl_display);
	wl_registry_add_listener(state.wl_registry, &wl_registry_listener, &state);
	wl_display_roundtrip(state.wl_display);

	if (state.wl_data_device_manager && state.wl_seat) {
		state.wl_data_device = wl_data_device_manager_get_data_device(
				state.wl_data_device_manager, state.wl_seat);
		wl_data_device_add_listener(state.wl_data_device,
				&wl_data_device_listener, &state);
	}

	int xcursor_size = 24;
	if (getenv("XCURSOR_SIZE") != NULL) {
		char *endptr;
		xcursor_size = strtol(getenv("XCURSOR_SIZE"), &endptr, 10);

		if (*endptr || xcursor_size <= 0) {
			NSLOG(neosurf, WARNING, "XCURSOR_SIZE environment variable is set incorrectly");
			xcursor_size = 24;
		}
	}

	state.cursors = wl_cursor_theme_load(getenv("XCURSOR_THEME"), xcursor_size, state.wl_shm);

	state.fds = calloc(1, sizeof(struct pollfd));
	state.nfd = 1;
	state.fds[0] = (struct pollfd){
		.fd = wl_display_get_fd(state.wl_display),
		.events = POLLIN,
	};

	struct neosurf_table vi_table = {
		.misc = &vi_misc_table,
		.clipboard = &vi_clipboard_table,
		.window = &vi_window_table,
		.fetch = &vi_fetch_table,
		.bitmap = &vi_bitmap_table,
		.layout = &vi_layout_table,
	};
	nserror ret = neosurf_register(&vi_table);
	if (ret != NSERROR_OK) {
		fatal("NeoSurf operation table failed registration");
	}

	respaths = nsvi_init_resource("${HOME}/.neosurf/:${NEOSURFRES}:"VISURF_RESPATH);
	nslog_init(nslog_stream_configure, &argc, argv);
	ret = nsoption_init(set_defaults, &nsoptions, &nsoptions_default);
	if (ret != NSERROR_OK) {
		fatal("Options failed to initialise");
	}
	char *options = filepath_find(respaths, "Choices");
	nsoption_read(options, nsoptions);
	free(options);
	nsoption_commandline(&argc, argv, nsoptions);

	char *messages = filepath_find(respaths, "Messages");
	ret = messages_add_from_file(messages);
	if (ret != NSERROR_OK) {
		NSLOG(neosurf, INFO, "Messages failed to load");
	}

	char buf[PATH_MAX];
	filepath_sfinddef(respaths, buf, "mime.types", "/etc/");
	nsvi_fetch_filetype_init(buf);

	nsvi_load_config(&state);

	ret = neosurf_init(NULL);
	free(messages);
	if (ret != NSERROR_OK) {
		fatal("NeoSurf failed to initialise");
	}

	urldb_load(nsoption_charp(url_file));
	urldb_load_cookies(nsoption_charp(cookie_file));

	nserror error;
	nsurl *url = NULL;
	if (argc < 2) {
		error = nsurl_create(config.home, &url);
		if (error != NSERROR_OK) {
			fatal("nsurl_create failed");
		}
	} else {
		struct stat fs;
		char *addr = NULL;
		if (stat(argv[1], &fs) == 0) {
			size_t addrlen;
			char *rp = realpath(argv[1], NULL);
			assert(rp != NULL);

			// Calculate file length, plus terminator
			addrlen = SLEN("file://") + strlen(rp) + 1;
			addr = malloc(addrlen);
			assert(addr != NULL);
			(void) snprintf(addr, addrlen, "file://%s", rp);
			free(rp);
		} else {
			addr = strdup(argv[1]);
		}
		error = nsurl_create(addr, &url);
		if (error != NSERROR_OK) {
			fatal("nsurl_create failed");
		}
	}
	error = browser_window_create(BW_CREATE_HISTORY, url, NULL, NULL, NULL);
	nsurl_unref(url);
	if (error != NSERROR_OK) {
		fatal("browser_window_create failed");
	}

	while (!state.quit) {
		if (wl_display_flush(state.wl_display) == -1 && errno != EAGAIN) {
			break;
		}

		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);

		struct nsvi_callback *next = NULL;
		struct nsvi_callback *cb = state.callbacks;
		while (cb) {
			int64_t ns = timespec_sub_to_nsec(&cb->due, &now);
			if (ns <= 0) {
				struct nsvi_callback copy = *cb;
				nsvi_schedule_remove(copy.callback, copy.p);
				copy.callback(copy.p);
				cb = state.callbacks;
				next = NULL;
			} else if (!next) {
				next = cb;
				cb = cb->next;
			} else {
				ns = timespec_sub_to_nsec(&cb->due, &next->due);
				if (ns <= 0) {
					next = cb;
				}
				cb = cb->next;
			}
		}

		int timeout = -1;
		if (next) {
			struct timespec diff;
			timespec_sub(&diff, &next->due, &now);
			timeout = timespec_to_msec(&diff);
			if (timeout <= 0) {
				timeout = 0;
			}
		}

		int max_fd;
		fd_set read_fd, write_fd, except_fd;
		nserror err = fetch_fdset(&read_fd, &write_fd, &except_fd, &max_fd);
		if (err != NSERROR_OK) {
			fatal("fetch_fdset error");
			break;
		}

		size_t nfetch = 0;
		for (int fd = 0; fd <= max_fd; ++fd) {
			bool present = FD_ISSET(fd, &read_fd)
				|| FD_ISSET(fd, &write_fd)
				|| FD_ISSET(fd, &except_fd);
			if (present) {
				++nfetch;
			}
		}
		if (state.nfd < nfetch + 1) {
			state.nfd = nfetch + 1;
			state.fds = realloc(state.fds,
				sizeof(struct pollfd) * state.nfd);
		}
		int i = 1;
		for (int fd = 0; nfetch && fd <= max_fd; ++fd) {
			bool present = FD_ISSET(fd, &read_fd)
				|| FD_ISSET(fd, &write_fd)
				|| FD_ISSET(fd, &except_fd);
			if (present) {
				state.fds[i].events = 0;
				state.fds[i].fd = fd;
			}
			if (FD_ISSET(fd, &read_fd)) {
				state.fds[i].events |= POLLIN | POLLHUP | POLLERR;
			}
			if (FD_ISSET(fd, &write_fd)) {
				state.fds[i].events |= POLLOUT | POLLERR;
			}
			if (FD_ISSET(fd, &except_fd)) {
				state.fds[i].events |= POLLERR;
			}
			if (present) {
				++i;
			}
		}
		assert((size_t)i == nfetch + 1);

		int r = poll(state.fds, nfetch + 1, timeout);
		if (r < 0) {
			fatal("poll(2) failed");
		}
		if (state.fds[0].revents & POLLIN) {
			wl_display_dispatch(state.wl_display);
		}
	}

	for (char **s = respaths; *s != NULL; s++) {
		free(*s);
	}
	free(respaths);

	free(state.fds);

	if (state.filter.in) {
		fclose(state.filter.in);
		fclose(state.filter.out);
		free(state.filter.buf);
		kill(state.filter.pid, SIGTERM);
	}

	nsvi_undo_finish();
	nsvi_fetch_filetype_fini();
	urldb_save_cookies(nsoption_charp(cookie_jar));
	urldb_save(nsoption_charp(url_file));
	neosurf_exit();
	nsoption_finalise(nsoptions, nsoptions_default);
	nslog_finalise();

	nsvi_config_finish();
	nsvi_bindings_finish(&state.bindings);

	struct nsvi_callback *cb = state.callbacks;
	while (cb) {
		struct nsvi_callback *next = cb->next;
		free(cb);
		cb = next;
	}

	wl_cursor_theme_destroy(state.cursors);
	xkb_keymap_unref(state.xkb_keymap);
	xkb_state_unref(state.xkb_state);
	xkb_context_unref(state.xkb_context);
	return 0;
}
