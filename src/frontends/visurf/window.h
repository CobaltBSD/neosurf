#ifndef NETSURF_VI_WINDOW_H_
#define NETSURF_VI_WINDOW_H_
#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>
#include "desktop/search.h"
#include "netsurf/mouse.h"
#include "visurf/pool-buffer.h"
#include "visurf/visurf.h"
#include "utils/nsurl.h"

struct gui_window_table;
extern struct gui_window_table vi_window_table;

extern struct pool_buffer *activebuffer;

enum window_mode {
	NORMAL,
	INSERT,
	EXLINE,
	FOLLOW,
	ERROR,
};

struct exline_state {
	char *cmd;
	size_t cmdln, cmdsz;
	size_t index;

	// Completions are the list of items above the prompt, ordered from most
	// to least relevant candidate and rendered in this order from bottom to
	// top above the exline prompt. The contents of this array depends on
	// the text already entered in the prompt; for example, an incomplete
	// "open" command might show a list of URLs from the history.
	char **comps;
	size_t ncomp, zcomp;
	size_t compix;
};

enum touch_operation {
	TOUCH_NONE,
	TOUCH_PENDING,
	TOUCH_SCROLL,
	TOUCH_DRAG,
	TOUCH_ZOOM,
};

struct nsvi_window_touch {
	enum touch_operation op;
	uint32_t start_time;
	int down_x, down_y;

	union {
		// State for a scrolling operation
		struct {
			int32_t id;
			int latest_x, latest_y;
		} scroll;

		// State for a drag (text selection) operation
		struct {
			int32_t id;
		} drag;

		// State for a zoom operation
		struct {
			int32_t a, b; // slot id
			int init_distance;
		} zoom;
	};
};

// An nsvi_window has one XDG toplevel and may have one or more tabs.
struct nsvi_window {
	struct nsvi_state *state;

	struct wl_surface *wl_surface;
	struct xdg_surface *xdg_surface;
	struct xdg_toplevel *xdg_toplevel;
	struct zxdg_toplevel_decoration_v1 *xdg_toplevel_decoration;
	struct pool_buffer buffers[2];

	struct gui_window **tabs;
	size_t ntab, tab;

	struct window_output *outputs;
	int width, height;
	int scale;
	bool dirty;
	bool pending_frame;
	char *error_message;
	bool fullscreen;

	enum window_mode mode;
	struct exline_state exline;
	int status_height, tab_height;

	struct {
		int pointer_x, pointer_y;
		double pressed_x, pressed_y;
		browser_mouse_state state;
		enum gui_pointer_shape shape;
		int serial;
		struct wl_cursor *cursor;
		struct wl_surface *cursor_surface;
	} mouse;
	uint32_t keyboard_enter_serial;
	struct nsvi_window_touch touch;

	struct nsvi_window *next;
};

enum follow_mode {
	FOLLOW_OPEN,
	FOLLOW_OPEN_TAB,
	FOLLOW_OPEN_WINDOW,
	FOLLOW_YANK,
	FOLLOW_YANK_PRIMARY,
};

struct link_hint {
	int x, y;
	int width, height;
	char *hint;
	struct box *node;
};

struct follow_state {
	const char *alphabet;
	size_t needed;
	enum follow_mode mode;

	struct link_hint *hints;
	size_t nhint, zhint;

	char *hintbuf;
	size_t hintln;

	// Only applicable for follow_mode FOLLOW_OPEN_TAB
	bool background;
};

// A gui_window represents a single NetSurf browser_window, i.e. a tab.
struct gui_window {
	struct nsvi_window *window;
	struct browser_window *bw;
	const char *status;
	char *search;
	search_flags_t search_flags;
	double sx, sy;
	bool throb;

	struct {
		bool enabled;
		int x, y, height;
	} caret;

	struct follow_state follow;
};

struct window_output {
	struct nsvi_output *output;
	struct window_output *next;
};

void nsvi_window_pointer_event(struct nsvi_window *win,
		struct pointer_event *event);
void nsvi_window_touch_event(struct nsvi_window *win,
		struct touch_event *event);
void nsvi_window_key_event(struct nsvi_window *win,
		xkb_keycode_t keycode, bool pressed);
void request_frame(struct nsvi_window *win);

void gui_window_constrain_scroll(struct gui_window *win);

#endif
