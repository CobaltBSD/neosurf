#ifndef NETSURF_VI_H_
#define NETSURF_VI_H_
#include <stdbool.h>
#include <time.h>
#include <wayland-client.h>
#include <wayland-cursor.h>
#include <xkbcommon/xkbcommon.h>
#include "visurf/keybindings.h"
#include "xdg-shell.h"
#include "xdg-decoration.h"
#include "utils/nsurl.h"

enum pointer_event_mask {
	POINTER_EVENT_ENTER = 1 << 0,
	POINTER_EVENT_LEAVE = 1 << 1,
	POINTER_EVENT_MOTION = 1 << 2,
	POINTER_EVENT_BUTTON = 1 << 3,
	POINTER_EVENT_AXIS = 1 << 4,
	POINTER_EVENT_AXIS_SOURCE = 1 << 5,
	POINTER_EVENT_AXIS_STOP = 1 << 6,
	POINTER_EVENT_AXIS_DISCRETE = 1 << 7,
};

struct pointer_event {
	uint32_t event_mask;
	struct wl_surface *surface;
	wl_fixed_t surface_x, surface_y;
	uint32_t button, state;
	uint32_t time;
	uint32_t serial;
	struct {
		bool valid;
		wl_fixed_t value;
		int32_t discrete;
	} axes[2];
	uint32_t axis_source;
};

enum touch_event_mask {
	TOUCH_EVENT_DOWN = 1 << 0,
	TOUCH_EVENT_UP = 1 << 1,
	TOUCH_EVENT_MOTION = 1 << 2,
	TOUCH_EVENT_CANCEL = 1 << 3,
	TOUCH_EVENT_SHAPE = 1 << 4,
	TOUCH_EVENT_ORIENTATION = 1 << 5,
};

struct touch_point {
	bool valid;
	int32_t id;
	uint32_t event_mask;
	struct wl_surface *wl_surface;
	wl_fixed_t surface_x, surface_y;
	wl_fixed_t major, minor;
	wl_fixed_t orientation;
};

struct touch_event {
	uint32_t event_mask;
	uint32_t time;
	uint32_t serial;
	struct touch_point points[10];
};

struct nsvi_filter {
	FILE *in;
	FILE *out;
	pid_t pid;
	char *buf;
	size_t bufsz;
};

struct nsvi_state {
	bool quit;
	struct wl_display *wl_display;
	struct wl_registry *wl_registry;
	struct wl_shm *wl_shm;
	struct wl_compositor *wl_compositor;
	struct xdg_wm_base *xdg_wm_base;
	struct zxdg_decoration_manager_v1 *xdg_decoration_manager;
	struct nsvi_output *outputs;

	struct nsvi_window *windows;
	struct nsvi_window *pointer_focus;
	struct nsvi_window *keyboard_focus;
	struct nsvi_undo *undo;
	struct nsvi_filter filter;

	struct wl_seat *wl_seat;
	struct wl_data_device_manager *wl_data_device_manager;
	struct wl_data_device *wl_data_device;
	struct wl_data_offer *wl_data_offer, *selected_offer;
	struct wl_data_source *wl_data_source;
	char *clipboard;
	size_t cliplen;

	struct wl_pointer *wl_pointer;
	struct pointer_event pointer_event;
	struct wl_cursor_theme *cursors;

	struct wl_touch *wl_touch;
	struct touch_event touch_event;

	struct wl_keyboard *wl_keyboard;
	struct xkb_context *xkb_context;
	struct xkb_keymap *xkb_keymap;
	struct xkb_state *xkb_state;
	struct nsvi_bindings bindings;
	int32_t repeat_rate, repeat_delay;
	uint32_t repeat_key;

	struct nsvi_callback *callbacks;
	struct pollfd *fds;
	size_t nfd;
};

struct nsvi_callback {
	void (*callback)(void *p);
	void *p;
	struct timespec due;
	struct nsvi_callback *next;
};

struct nsvi_output {
	struct wl_output *wl_output;
	int scale;
	struct nsvi_output *next;
};

extern struct nsvi_state *global_state;

struct touch_point *get_touch_point(struct nsvi_state *state, int32_t id);
void nsvi_set_clipboard(struct nsvi_state *state, const char *type,
		const char *data, size_t len);
nserror nsvi_filter_url(struct nsvi_filter *filter,
		const char *in, nsurl **out);

#endif
