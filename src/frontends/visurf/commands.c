#include <assert.h>
#include <errno.h>
#include <dom/dom.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <wordexp.h>
#include <unistd.h>
#include "utils/errors.h"
#include "content/content.h"
#include "content/content_protected.h"
#include "content/handlers/html/box.h"
#include "content/handlers/html/box_inspect.h"
#include "content/handlers/html/html.h"
#include "content/handlers/html/html_save.h"
#include "content/hlcache.h"
#include "desktop/browser_history.h"
#include "desktop/search.h"
#include "netsurf/browser_window.h"
#include "netsurf/content.h"
#include "netsurf/content_type.h"
#include "netsurf/keypress.h"
#include "visurf/commands.h"
#include "visurf/getopt.h"
#include "visurf/settings.h"
#include "visurf/undo.h"
#include "visurf/visurf.h"
#include "visurf/window.h"
#include "utils/log.h"
#include "utils/nsurl.h"
#include "utils/string.h"
#include "xdg-shell.h"

static char *
join_args(int argc, char *argv[])
{
	assert(argc > 0);
	int len = 0, i;
	for (i = 0; i < argc; ++i) {
		len += strlen(argv[i]) + 1;
	}
	char *res = malloc(len);
	len = 0;
	for (i = 0; i < argc; ++i) {
		strcpy(res + len, argv[i]);
		len += strlen(argv[i]);
		res[len++] = ' ';
	}
	res[len - 1] = '\0';
	return res;
}

static void
handle_error(char *error_message) {
	struct nsvi_window *win = global_state->keyboard_focus;
	win->error_message = error_message;
	win->mode = ERROR;
}

static int
cmd_back(struct nsvi_state *state, int argc, char *argv[])
{
	if (argc != 1) {
		NSLOG(netsurf, ERROR, "back: unexpected argument");
		handle_error(strdup("back: unexpected argument"));
		return 1;
	}
	struct nsvi_window *win = global_state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, ERROR, "back: no active window");
		handle_error(strdup("back: no active window"));
		return 1;
	}
	struct gui_window *gw = win->tabs[win->tab];
	browser_window_history_back(gw->bw, false);
	return 0;
}

static int
cmd_bind(struct nsvi_state *state, int argc, char *argv[])
{
	if (argc != 3) {
		NSLOG(netsurf, ERROR, "bind: incorrect number of arguments");
		handle_error(strdup("bind: incorrect number of arguments"));
		return 1;
	}
	if (nsvi_bindings_new(&state->bindings, argv[1], argv[2]) != NSERROR_OK) {
		NSLOG(netsurf, ERROR, "bind: invalid binding");
		handle_error(strdup("bind: invalid binding"));
		return 1;
	}
	return 0;
}

static int
cmd_close(struct nsvi_state *state, int argc, char *argv[])
{
	if (argc != 1) {
		NSLOG(netsurf, ERROR, "close: unexpected argument");
		handle_error(strdup("close: unexpected argument"));
		return 1;
	}
	struct nsvi_window *win = global_state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, ERROR, "close: no active window");
		handle_error(strdup("close: no active window"));
		return 1;
	}

	nsurl *url;
	struct gui_window *gw = win->tabs[win->tab];

	if (browser_window_get_url(gw->bw, true, &url) == NSERROR_OK) {
		nsvi_undo_tab_new(url);
	}

	browser_window_destroy(gw->bw);
	return 0;
}

static int
cmd_exec(struct nsvi_state *state, int argc, char *argv[])
{
	char **subcmd = calloc(argc, sizeof(char *));
	for (int i = 1; i < argc; ++i) {
		subcmd[i - 1] = argv[i];
	}
	pid_t pid = fork();
	assert(pid != -1);
	if (pid == 0) {
		execvp(subcmd[0], subcmd);
		// TODO: Bubble these errors up to the UI somehow
		NSLOG(netsurf, ERROR, "exec: execvp failed: %s",
			strerror(errno));
		exit(1);
	}
	// TODO: Keep track of the subprocess, show in UI if it exits nonzero
	free(subcmd);
	return 0;
}

static int
cmd_exline(struct nsvi_state *state, int argc, char *argv[])
{
	struct nsvi_window *win = global_state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, ERROR, "exline: no active window");
		handle_error(strdup("exline: no active window"));
		return 1;
	}
	free(win->exline.cmd);
	if (argc > 1) {
		win->exline.cmd = join_args(argc - 1, &argv[1]);
		win->exline.cmdln = strlen(win->exline.cmd);
		win->exline.cmdsz = win->exline.cmdln + 1;
		win->exline.index = win->exline.cmdln;
	} else {
		win->exline.cmd = malloc(1024);
		win->exline.cmd[0] = 0;
		win->exline.cmdsz = 1024;
		win->exline.index = 0;
		win->exline.cmdln = 0;
	}
	win->mode = EXLINE;
	request_frame(win);
	return 0;
}

static void
follow_collect_hints(struct gui_window *win, struct box *node)
{
	struct follow_state *follow = &win->follow;
	if ((node->href || node->gadget) && node->width > 0 && node->height > 0) {
		for (size_t i = 0; i < follow->nhint; i++) {
			if (node->href == follow->hints[i].node->href
					&& node->href != NULL) {
				goto end;
			}
		}
		if (!follow->hints) {
			follow->hints = calloc(64, sizeof(struct link_hint));
			follow->nhint = 0;
			follow->zhint = 64;
		} else if (follow->nhint + 1 >= follow->zhint) {
			follow->zhint *= 2;
			follow->hints = realloc(follow->hints,
				follow->zhint * sizeof(struct link_hint));
		}
		struct link_hint *hint = &follow->hints[follow->nhint];
		hint->width = node->width, hint->height = node->height;
		box_coords(node, &hint->x, &hint->y);
		hint->node = node;
		++follow->nhint;
	}

end:
	for (struct box *child = node->children; child; child = child->next) {
		follow_collect_hints(win, child);
	}
}

static size_t
ceil_log(size_t n, size_t base)
{
	assert(n >= 1 && base >= 2);
	size_t result = 1;
	size_t accum = base;
	while (accum < n) {
		++result;
		accum *= base;
	}
	return result;
}

static char *
mkhint(size_t serial, const char *alphabet, size_t digits)
{
	char *hint = calloc(1, 32);
	size_t hintln = 0;
	size_t base = strlen(alphabet);
	while (true) {
		assert(hintln < 32);
		size_t remain = serial % base;
		memmove(&hint[1], &hint[0], hintln);
		hint[0] = alphabet[remain];
		++hintln;
		serial -= remain;
		serial /= base;
		if (serial <= 0) {
			break;
		}
	}
	while (hintln < digits) {
		assert(hintln < 31);
		memmove(&hint[1], &hint[0], hintln);
		hint[0] = alphabet[0];
		++hintln;
	}
	hint[hintln] = 0;
	return hint;
}

static uint32_t
fnv1a(uint32_t hash, unsigned char c)
{
	return (hash ^ c) * 16777619;
}


static uint32_t
fnv1a_s(uint32_t hash, const char *str)
{
	unsigned char c;
	while ((c = *str++)) {
		hash = fnv1a(hash, c);
	}
	return hash;
}

static void
follow_label_hints(struct gui_window *win)
{
	// Based on qutebrowser's "scattered" algorithm
	struct follow_state *follow = &win->follow;
	const char *alphabet = config.hints.alphabet;
	size_t alphalen = strlen(alphabet);
	size_t needed = ceil_log(follow->nhint, alphalen);
	follow->alphabet = alphabet;
	follow->needed = needed;
	follow->hintbuf = malloc(needed + 1);
	follow->hintbuf[0] = 0;
	follow->hintln = 0;

	size_t nshort = 0;
	if (needed > 1) {
		size_t space = (int)pow(alphalen, needed);
		nshort = (space - follow->nhint) / (alphalen - 1);
	}

	size_t nlong = follow->nhint - nshort;

	size_t i = 0;
	if (needed > 1) {
		for (size_t serial = 0; serial < nshort; ++serial) {
			follow->hints[i++].hint =
				mkhint(serial, alphabet, needed - 1);
		}
	}

	size_t start = nshort * alphalen;
	for (size_t serial = start; serial < start + nlong; ++serial) {
		follow->hints[i++].hint = mkhint(serial, alphabet, needed);
	}

	// Fischer-Yates shuffle
	nsurl *url;
	nserror err = browser_window_get_url(win->bw, true, &url);
	if (err == NSERROR_OK) {
		srand(fnv1a_s(0, nsurl_access(url)));
	}
	for (size_t i = follow->nhint - 1; i > 0; --i) {
		size_t j = rand() % (i + 1);
		char *a = follow->hints[i].hint, *b = follow->hints[j].hint;
		follow->hints[i].hint = b;
		follow->hints[j].hint = a;
	}
}

static int
cmd_follow(struct nsvi_state *state, int argc, char *argv[])
{
	int c;
	ns_optind = 0;
	enum follow_mode mode = FOLLOW_OPEN;
	bool background = false;
	while ((c = ns_getopt(argc, argv, "btwyY")) != -1) {
		switch (c) {
		case 'b':
			background = true;
			break;
		case 't':
			mode = FOLLOW_OPEN_TAB;
			break;
		case 'w':
			mode = FOLLOW_OPEN_WINDOW;
			break;
		case 'y':
			mode = FOLLOW_YANK;
			break;
		case 'Y':
			mode = FOLLOW_YANK_PRIMARY;
			break;
		default:
			NSLOG(netsurf, ERROR, "follow: invalid flag '%c'", c);
			char *error_message = malloc(25);
			sprintf(error_message, "follow: invalid flag '%c'", c);
			handle_error(error_message);
			return 1;
		}
	}
	if (ns_optind != argc) {
		NSLOG(netsurf, ERROR, "follow: unexpected argument");
		handle_error(strdup("follow: unexpected argument"));
		return 1;
	}

	struct nsvi_window *win = global_state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, ERROR, "follow: no active window");
		handle_error(strdup("follow: no active window"));
		return 1;
	}
	struct gui_window *gw = win->tabs[win->tab];
	struct hlcache_handle *handle = browser_window_get_content(gw->bw);
	if (!handle) {
		NSLOG(netsurf, ERROR, "follow: no active content");
		handle_error(strdup("follow: no active content"));
		return 1;
	}
	if (content_get_type(handle) != CONTENT_HTML) {
		NSLOG(netsurf, ERROR, "follow: cannot work with non-HTML content");
		handle_error(strdup("follow: cannot work with non-HTML content"));
		return 1;
	}
	struct box *tree = html_get_box_tree(handle);
	follow_collect_hints(gw, tree);
	if (gw->follow.nhint == 0) {
		NSLOG(netsurf, ERROR, "follow: no hints");
		handle_error(strdup("follow: no hints"));
		return 1;
	}
	follow_label_hints(gw);
	win->mode = FOLLOW;
	gw->follow.mode = mode;
	if (mode == FOLLOW_OPEN_TAB) {
		gw->follow.background = background;
	}
	request_frame(win);
	return 0;
}

static int
cmd_forward(struct nsvi_state *state, int argc, char *argv[])
{
	if (argc != 1) {
		NSLOG(netsurf, ERROR, "forward: unexpected argument");
		handle_error(strdup("forward: unexpected argument"));
		return 1;
	}
	struct nsvi_window *win = global_state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, ERROR, "forward: no active window");
		handle_error(strdup("forward: no active window"));
		return 1;
	}
	struct gui_window *gw = win->tabs[win->tab];
	browser_window_history_forward(gw->bw, false);
	return 0;
}

static int
cmd_fullscreen(struct nsvi_state *state, int argc, char *argv[])
{
	if (argc != 1) {
		NSLOG(netsurf, ERROR, "fullscreen: unexpected argument");
		handle_error(strdup("fullscreen: unexpected argument"));
		return 1;
	}
	struct nsvi_window *win = global_state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, ERROR, "fullscreen: no active window");
		handle_error(strdup("fullscreen: no active window"));
		return 1;
	}
	if (win->fullscreen) {
		xdg_toplevel_unset_fullscreen(win->xdg_toplevel);
	} else {
		xdg_toplevel_set_fullscreen(win->xdg_toplevel, NULL);
	}
	return 0;
}

static int
cmd_insert(struct nsvi_state *state, int argc, char *argv[])
{
	if (argc != 1) {
		NSLOG(netsurf, ERROR, "insert: unexpected argument");
		handle_error(strdup("insert: unexpected argument"));
		return 1;
	}
	struct nsvi_window *win = global_state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, ERROR, "insert: no active window");
		handle_error(strdup("insert: no active window"));
		return 1;
	}
	win->mode = INSERT;
	request_frame(win);
	return 0;
}

static int
cmd_open(struct nsvi_state *state, int argc, char *argv[])
{
	bool background = false, tab = false, window = false, filter_url = true;

	int c;
	ns_optind = 0;
	while ((c = ns_getopt(argc, argv, "btwF")) != -1) {
		switch (c) {
		case 'b':
			background = true;
			break;
		case 't':
			tab = true;
			break;
		case 'w':
			window = true;
			break;
		case 'F':
			filter_url = false;
			break;
		default:
			NSLOG(netsurf, ERROR, "open: invalid flag '%c'", c);
			char *error_message = malloc(23);
			sprintf(error_message, "open: invalid flag '%c'", c);
			handle_error(error_message);
			return 1;
		}
	}
	if (ns_optind >= argc) {
		NSLOG(netsurf, ERROR, "open: expected an argument");
		handle_error(strdup("open: expected an argument"));
		return 1;
	}

	char *target = argv[ns_optind];
	if (ns_optind + 1 < argc) {
		target = join_args(argc - ns_optind, &argv[ns_optind]);
	}

	int ret = 0;
	nsurl *url = NULL;
	nserror error;

	if (filter_url) {
		error = nsvi_filter_url(&global_state->filter, target, &url);
	} else {
		error = nsurl_create(target, &url);
	}

	if (error != NSERROR_OK) {
		NSLOG(netsurf, ERROR, "open: invalid url");
		handle_error(strdup("open: invalid url"));
		ret = 1;
		goto exit;
	}

	struct nsvi_window *win = global_state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, ERROR, "open: no active window");
		handle_error(strdup("open: no active window"));
		ret = 1;
		goto exit;
	}
	struct gui_window *gw = win->tabs[win->tab];
	if (tab) {
		enum browser_window_create_flags flags =
			BW_CREATE_HISTORY | BW_CREATE_TAB;
		if (!background) {
			flags |= BW_CREATE_FOREGROUND;
		}
		error = browser_window_create(flags, url, NULL, gw->bw, NULL);
	} else if (window) {
		error = browser_window_create(
			BW_CREATE_HISTORY, url, NULL, NULL, NULL);
	} else {
		error = browser_window_navigate(gw->bw, url, NULL,
			BW_NAVIGATE_HISTORY, NULL, NULL, NULL);
	}
	nsurl_unref(url);
	if (error != NSERROR_OK) {
		NSLOG(netsurf, ERROR, "open: browser_window_navigate failed");
		handle_error(strdup("open: browser_window_navigate failed"));
		ret = 1;
		goto exit;
	}
exit:
	if (ns_optind + 1 < argc) {
		free(target);
	}
	return ret;
}

static int
cmd_page(struct nsvi_state *state, int argc, char *argv[])
{
	if (argc != 2) {
		NSLOG(netsurf, ERROR, "page: expected argument");
		handle_error(strdup("page: expected argument"));
		return 1;
	}

	struct nsvi_window *win = global_state->keyboard_focus;

	if (!win) {
		NSLOG(netsurf, ERROR, "page: no active window");
		handle_error(strdup("page: no active window"));
		return 1;
	}

	if ((strcmp(argv[1], "next") != 0) && (strcmp(argv[1], "prev") != 0)) {
		NSLOG(netsurf, ERROR, "page: expected <next|prev>");
		handle_error(strdup("page: expected <next|prev>"));
		return 1;
	}

	struct browser_window *bw = win->tabs[win->tab]->bw;
	struct hlcache_handle *handle = browser_window_get_content(bw);

	if (!handle) {
		NSLOG(netsurf, ERROR, "page: no active content");
		handle_error(strdup("page: no active content"));
		return 1;
	}

	if (content_get_type(handle) != CONTENT_HTML) {
		NSLOG(netsurf, ERROR, "page: cannot work with non-HTML content");
		handle_error(strdup("page: cannot work with non-HTML content"));
		return 1;
	}

	struct content_rfc5988_link *link =
		hlcache_handle_get_content(handle)->links;
	bool rel_match = true;
	int ret = 0;

	lwc_string *rel;
	lwc_intern_string(argv[1], 4, &rel);

	while (link != NULL) {
		if (lwc_string_caseless_isequal(link->rel, rel, &rel_match) == lwc_error_ok && rel_match) {
			browser_window_navigate(bw, link->href, NULL,
			BW_NAVIGATE_HISTORY, NULL, NULL, NULL);
			request_frame(win);
			goto exit;
		}
		link = link->next;
	}

	NSLOG(netsurf, WARNING, "page: no <link rel=\"%s\"> found", argv[1]);
	ret = 1;
	goto exit;

exit:
	lwc_string_unref(rel);
	return ret;
}

static int
cmd_paste(struct nsvi_state *state, int argc, char *argv[])
{
	bool background = false, tab = false, window = false, primary = false;

	int c;
	ns_optind = 0;
	while ((c = ns_getopt(argc, argv, "btwp")) != -1) {
		switch (c) {
		case 'b':
			background = true;
			break;
		case 't':
			tab = true;
			break;
		case 'w':
			window = true;
			break;
		case 'p':
			primary = true;
			break;
		}
	}
	if (ns_optind != argc) {
		NSLOG(netsurf, ERROR, "paste: expected one argument");
		handle_error(strdup("paste: expected one argument"));
		return 1;
	}
	struct nsvi_window *win = state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, ERROR, "paste: no active window");
		handle_error(strdup("paste: no active window"));
		return 1;
	}

	struct wl_data_offer *offer = state->selected_offer;
	if (!offer) {
		NSLOG(netsurf, WARNING, "paste: nothing to paste");
		return 1;
	}

	int fds[2];
	pipe(fds);
	wl_data_offer_receive(offer, "text/plain", fds[1]);
	close(fds[1]);

	// TODO: Async read
	wl_display_roundtrip(state->wl_display);

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

	nsurl *url = NULL;
	nserror error = nsurl_create(data, &url);
	free(data);
	if (error != NSERROR_OK) {
		NSLOG(netsurf, ERROR, "paste: invalid url");
		handle_error(strdup("paste: invalid url"));
		return 1;
	}

	struct gui_window *gw = win->tabs[win->tab];
	if (tab) {
		enum browser_window_create_flags flags =
			BW_CREATE_HISTORY | BW_CREATE_TAB;
		if (!background) {
			flags |= BW_CREATE_FOREGROUND;
		}
		error = browser_window_create(flags, url, NULL, gw->bw, NULL);
	} else if (window) {
		error = browser_window_create(
			BW_CREATE_HISTORY, url, NULL, NULL, NULL);
	} else {
		error = browser_window_navigate(gw->bw, url, NULL,
			BW_NAVIGATE_HISTORY, NULL, NULL, NULL);
	}
	nsurl_unref(url);

	if (error != NSERROR_OK) {
		NSLOG(netsurf, ERROR, "open: browser_window_navigate failed");
		handle_error(strdup("open: browser_window_navigate failed"));
		return 1;
	}
	return 0;
}

static int
cmd_quit(struct nsvi_state *state, int argc, char *argv[])
{
	state->quit = true;
	return 0;
}

static int
cmd_reload(struct nsvi_state *state, int argc, char *argv[])
{
	bool force = 0;

	int c;
	ns_optind = 0;
	while ((c = ns_getopt(argc, argv, "f")) != -1) {
		switch (c) {
		case 'f':
			force = true;
			break;
		default:
			NSLOG(netsurf, ERROR, "reload: invalid flag '%c'", c);
			char *error_message = malloc(25);
			sprintf(error_message, "reload: invalid flag '%c'", c);
			handle_error(error_message);
			return 1;
		}
	}
	if (ns_optind < argc) {
		NSLOG(netsurf, ERROR, "reload: unexpected argument");
		handle_error(strdup("reload: unexpected argument"));
		return 1;
	}

	struct nsvi_window *win = state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, WARNING, "reload: no active window");
		return 1;
	}
	struct gui_window *gw = win->tabs[win->tab];
	browser_window_reload(gw->bw, force);
	return 0;
}

static int
cmd_scroll(struct nsvi_state *state, int argc, char *argv[])
{
	enum { HORIZ, VERT } axis = VERT;
	enum { ABS, PCT } mode = ABS;
	enum { WIN, PAGE } ref = WIN;
	bool relative = false;

	int c;
	ns_optind = 0;
	while ((c = ns_getopt(argc, argv, "hp")) != -1) {
		switch (c) {
		case 'h':
			axis = HORIZ;
			break;
		case 'p':
			ref = PAGE;
			break;
		default:
			NSLOG(netsurf, ERROR, "scroll: invalid flag '%c'", c);
			char *error_message = malloc(25);
			sprintf(error_message, "scroll: invalid flag '%c'", c);
			handle_error(error_message);
			return 1;
		}
	}
	if (ns_optind >= argc) {
		NSLOG(netsurf, ERROR, "scroll: expected argument");
		handle_error(strdup("scroll: expected argument"));
		return 1;
	}

	char *endptr;
	int amt = strtol(argv[ns_optind], &endptr, 10);
	while (endptr[0]) {
		switch (endptr[0]) {
		case '\0':
			break;
		case '%':
			mode = PCT;
			break;
		case '+':
			relative = true;
			break;
		case '-':
			relative = true;
			amt = -amt;
			break;
		default:
			NSLOG(netsurf, ERROR, "scroll: invalid argument");
			handle_error(strdup("scroll: invalid argument"));
			return 1;
		}
		++endptr;
	}

	if (!state->keyboard_focus) {
		NSLOG(netsurf, WARNING, "scroll: no active window");
		return 1;
	}

	struct nsvi_window *win = state->keyboard_focus;
	struct gui_window *gw = win->tabs[win->tab];

	int width, height;
	if (ref == PAGE) {
		browser_window_get_extents(gw->bw, true, &width, &height);
	} else {
		width = win->width;
		height = win->height - (win->status_height + win->tab_height);
	}

	switch (mode) {
	case ABS:
		amt *= win->scale;
		break;
	case PCT:
		amt = (int)(amt / 100.0 * (axis == HORIZ ? width : height));
		break;
	}

	switch (axis) {
	case HORIZ:
		if (relative) {
			gw->sx += amt;
		} else {
			gw->sx = -amt;
		}
		break;
	case VERT:
		if (relative) {
			gw->sy += amt;
		} else {
			gw->sy = -amt;
		}
		break;
	}

	gui_window_constrain_scroll(gw);
	request_frame(win);
	return 0;
}

static int
cmd_search(struct nsvi_state *state, int argc, char *argv[])
{
	// TODO: Incremental search
	struct nsvi_window *win = global_state->keyboard_focus;
	struct gui_window *gw = win->tabs[win->tab];

	int c;
	ns_optind = 0;
	while ((c = ns_getopt(argc, argv, "acnps")) != -1) {
		switch (c) {
		case 'a':
			gw->search_flags |= SEARCH_FLAG_SHOWALL;
			break;
		case 'c':
			free(gw->search);
			gw->search = NULL;
			gw->search_flags = SEARCH_FLAG_FORWARDS;
			break;
		case 'p':
			gw->search_flags &= ~SEARCH_FLAG_FORWARDS;
			gw->search_flags |= SEARCH_FLAG_BACKWARDS;
			break;
		case 'n':
			gw->search_flags &= ~SEARCH_FLAG_BACKWARDS;
			gw->search_flags |= SEARCH_FLAG_FORWARDS;
			break;
		case 's':
			gw->search_flags |= SEARCH_FLAG_CASE_SENSITIVE;
			break;
		default:
			NSLOG(netsurf, ERROR, "search: invalid flag '%c'", c);
			char *error_message = malloc(25);
			sprintf(error_message, "search: invalid flag '%c'", c);
			handle_error(error_message);
			return 1;
		}
	}

	if (argc - ns_optind == 1) {
		free(gw->search);
		gw->search = strdup(argv[ns_optind]);
	}

	browser_window_search(win->tabs[win->tab]->bw, NULL,
		gw->search_flags, gw->search);
	return 0;
}

static int
cmd_set(struct nsvi_state *state, int argc, char *argv[])
{
	if (argc != 3) {
		NSLOG(netsurf, ERROR, "set: expected two arguments");
		handle_error(strdup("set: expected two arguments"));
		return 1;
	}
	if (!nsvi_config_set(argv[1], argv[2])) {
		NSLOG(netsurf, ERROR, "set: invalid setting %s", argv[1]);
		char *error_message = malloc(strlen(argv[1]) + 22);
		sprintf(error_message, "set: invalid setting %s", argv[1]);
		handle_error(error_message);
		return 1;
	}
	if (state->keyboard_focus) {
		request_frame(state->keyboard_focus);
	}
	return 0;
}

static int
cmd_source(struct nsvi_state *state, int argc, char *argv[])
{
	if (argc < 2) {
		NSLOG(netsurf, ERROR, "source: not enough arguments");
		handle_error(strdup("source: not enough arguments"));
		return 1;
	}
	return nsvi_command_source(state, argv[1]);
}

static int
cmd_stop(struct nsvi_state *state, int argc, char *argv[])
{
	if (argc != 1) {
		NSLOG(netsurf, ERROR, "stop: unexpected argument");
		handle_error(strdup("stop: unexpected argument"));
		return 1;
	}
	struct nsvi_window *win = global_state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, ERROR, "stop: no active window");
		handle_error(strdup("stop: no active window"));
		return 1;
	}
	struct gui_window *gw = win->tabs[win->tab];
	browser_window_stop(gw->bw);
	return 0;
}

static int
cmd_tab(struct nsvi_state *state, int argc, char *argv[])
{
	if (argc != 2) {
		NSLOG(netsurf, ERROR, "tab: expected argument");
		handle_error(strdup("tab: expected argument"));
		return 1;
	}
	struct nsvi_window *win = global_state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, ERROR, "tab: no active window");
		handle_error(strdup("tab: no active window"));
		return 1;
	}
	int i = win->tab;
	if (strcmp(argv[1], "next") == 0) {
		++i;
	} else if (strcmp(argv[1], "prev") == 0) {
		--i;
	} else {
		char *endptr;
		i = strtoul(argv[1], &endptr, 10);
		if (*endptr) {
			NSLOG(netsurf, ERROR, "tab: expected <next|prev|[index]>");
			handle_error(strdup("tab: expected <next|prev|[index]>"));
			return 1;
		}
	}
	i = i % (int)win->ntab;
	if (i < 0) {
		i = win->ntab + i;
	}
	win->tab = i;
	request_frame(win);
	return 0;
}

static int
cmd_tabmove(struct nsvi_state *state, int argc, char *argv[]) {
	if (argc != 2) {
		NSLOG(netsurf, ERROR, "tabmove: expected argument");
		handle_error(strdup("tabmove: expected argument"));
		return 1;
	}
	struct nsvi_window *win = global_state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, ERROR, "tabmove: no active window");
		handle_error(strdup("tabmove: no active window"));
		return 1;
	}

	char *endptr;
	int i = win->tab;
	int j = strtoul(argv[1], &endptr, 10);
	if (*endptr) {
		NSLOG(netsurf, ERROR, "tabmove: expected [index]");
		handle_error(strdup("tabmove: expected [index]"));
		return 1;
	}

	j = j % (int)win->ntab;
	if (j < 0) {
		j = win->ntab + j;
	}

	// Swapping tab i (current tab) and tab j
	struct gui_window *current_tab = win->tabs[i];
	win->tabs[i] = win->tabs[j];
	win->tabs[j] = current_tab;

	// Refocus the previously focused tab
	win->tab = j;

	request_frame(win);
	return 0;
}

static int
cmd_unbind(struct nsvi_state *state, int argc, char *argv[])
{
	if (argc != 2) {
		NSLOG(netsurf, ERROR, "unbind: unexpected argument");
		handle_error(strdup("unbind: unexpected argument"));
		return 1;
	}
	if (nsvi_bindings_remove(&state->bindings, argv[1]) != NSERROR_OK) {
		NSLOG(netsurf, ERROR, "unbind: invalid binding");
		handle_error(strdup("unbind: invalid binding"));
		return 1;
	}
	return 0;
}

static int
cmd_undo(struct nsvi_state *state, int argc, char *argv[])
{
	if (argc != 1) {
		NSLOG(netsurf, ERROR, "undo: unexpected argument");
		handle_error(strdup("undo: unexpected argument"));
		return 1;
	}
	struct nsvi_window *win = global_state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, ERROR, "undo: no active window");
		handle_error(strdup("undo: no active window"));
		return 1;
	}
	struct nsvi_undo *undo = state->undo;
	if (!undo) {
		NSLOG(netsurf, ERROR, "undo: nothing to undo");
		handle_error(strdup("undo: nothing to undo"));
		return 1;
	}

	nserror error;
	struct gui_window *gw = win->tabs[win->tab];
	switch (undo->kind) {
	case UNDO_TAB:
		assert(undo->nurl == 1);
		error = browser_window_create(
			BW_CREATE_TAB | BW_CREATE_FOREGROUND | BW_CREATE_HISTORY,
			undo->urls[0], NULL, gw->bw, NULL);
		if (error != NSERROR_OK) {
			NSLOG(netsurf, ERROR, "undo: failed to create tab");
			handle_error(strdup("undo: failed to create tab"));
			return 1;
		}
		break;
	case UNDO_WIN:
		assert(0); // TODO
	}

	nsvi_undo_free(undo);
	return 0;
}

static int
cmd_yank(struct nsvi_state *state, int argc, char *argv[])
{
	enum {
		URL,
		TITLE,
		SELECTION,
	} object = URL;
	enum {
		CLIPBOARD,
		PRIMARY,
	} clipboard = CLIPBOARD;

	int c;
	ns_optind = 0;
	while ((c = ns_getopt(argc, argv, "pts")) != -1) {
		switch (c) {
		case 'p':
			clipboard = PRIMARY;
			break;
		case 't':
			object = TITLE;
			break;
		case 's':
			object = SELECTION;
			break;
		default:
			NSLOG(netsurf, ERROR, "yank: invalid flag '%c'", c);
			char *error_message = malloc(23);
			sprintf(error_message, "yank: invalid flag '%c'", c);
			handle_error(error_message);
			return 1;
		}
	}
	if (ns_optind < argc) {
		NSLOG(netsurf, ERROR, "yank: unexpected argument");
		handle_error(strdup("yank: unexpected argument"));
		return 1;
	}

	struct nsvi_window *win = state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, WARNING, "yank: no active window");
		return 1;
	}

	struct gui_window *gw = win->tabs[win->tab];
	const char *data;
	nserror err = NSERROR_OK;
	nsurl *url;

	switch (object) {
	case URL:
		err = browser_window_get_url(gw->bw, true, &url);
		data = nsurl_access(url);
		break;
	case TITLE:
		data = browser_window_get_title(gw->bw);
		break;
	case SELECTION:
		browser_window_key_press(gw->bw, NS_KEY_COPY_SELECTION);
		return 0;
	}
	if (err != NSERROR_OK) {
		NSLOG(netsurf, ERROR, "yank: error setting clipboard");
		handle_error(strdup("yank: error setting clipboard"));
		return 1;
	}

	switch (clipboard) {
	case CLIPBOARD:
		nsvi_set_clipboard(state, "text/plain", data, strlen(data));
		break;
	case PRIMARY:
		assert(0); // TODO
	}

	return 0;
}

static int
cmd_zoom(struct nsvi_state *state, int argc, char *argv[])
{
	// TODO: Remember zoom preference for each domain
	bool relative = false;
	if (argc < 2) {
		NSLOG(netsurf, ERROR, "zoom: expected argument");
		handle_error(strdup("zoom: expected argument"));
		return 1;
	}

	char *endptr;
	float amt = strtof(argv[1], &endptr);
	while (endptr[0]) {
		switch (endptr[0]) {
		case '\0':
			break;
		case '%':
			amt = amt / 100.0f;
			break;
		case '+':
			relative = true;
			break;
		case '-':
			relative = true;
			amt = -amt;
			break;
		default:
			NSLOG(netsurf, ERROR, "zoom: invalid argument");
			handle_error(strdup("zoom: invalid argument"));
			return 1;
		}
		++endptr;
	}

	struct nsvi_window *win = state->keyboard_focus;
	if (!win) {
		NSLOG(netsurf, WARNING, "zoom: no active window");
		return 1;
	}

	struct gui_window *gw = win->tabs[win->tab];
	nserror error = browser_window_set_scale(gw->bw, amt, !relative);
	if (error != NSERROR_OK) {
		NSLOG(netsurf, WARNING, "zoom: browser_window_set_scale failed");
		return 1;
	}
	return 0;
}

struct command {
	const char *name;
	int (*exec)(struct nsvi_state *state, int argc, char *argv[]);
};

// Alpha sorted
static const struct command commands[] = {
	{ "back", &cmd_back },
	{ "bind", &cmd_bind },
	{ "close", &cmd_close },
	{ "exec", &cmd_exec },
	{ "exline", &cmd_exline },
	{ "follow", &cmd_follow },
	{ "forward", &cmd_forward },
	{ "fullscreen", &cmd_fullscreen },
	{ "insert", &cmd_insert },
	{ "open", &cmd_open },
	{ "page", &cmd_page },
	{ "paste", &cmd_paste },
	{ "quit", &cmd_quit },
	{ "reload", &cmd_reload },
	{ "scroll", &cmd_scroll },
	{ "search", &cmd_search },
	{ "set", &cmd_set },
	{ "source", &cmd_source },
	{ "stop", &cmd_stop },
	{ "tab", &cmd_tab },
	{ "tabmove", &cmd_tabmove },
	{ "unbind", &cmd_unbind },
	{ "undo", &cmd_undo },
	{ "yank", &cmd_yank },
	{ "zoom", &cmd_zoom },
};

static int
cmd_compar(const void *a, const void *b)
{
	const char *key = a;
	const struct command *cmd = b;
	if (!key) {
		return -1;
	}
	return strcmp(key, cmd->name);
}

static void
populate_env(struct nsvi_state *state)
{
	unsetenv("url");
	unsetenv("scheme");
	unsetenv("username");
	unsetenv("host");
	unsetenv("port");
	unsetenv("path");
	unsetenv("query");
	unsetenv("fragment");
	unsetenv("title");

	struct nsvi_window *win = state->keyboard_focus;
	if (!win) {
		return;
	}

	struct gui_window *gw = win->tabs[win->tab];
	nsurl *url;
	nserror err = browser_window_get_url(gw->bw, true, &url);
	if (err == NSERROR_OK) {
		setenv("url", nsurl_access(url), 1);

		struct {
			const char *name;
			nsurl_component part;
		} urlparts[] = {
			{ "scheme", NSURL_SCHEME },
			{ "username", NSURL_USERNAME },
			{ "host", NSURL_HOST },
			{ "port", NSURL_PORT },
			{ "path", NSURL_PATH },
			{ "query", NSURL_QUERY },
			{ "fragment", NSURL_FRAGMENT },
		};

		char *part;
		size_t sz;
		for (size_t i = 0;
				i < sizeof(urlparts) / sizeof(urlparts[0]);
				++i) {
			if (nsurl_has_component(url, urlparts[i].part)) {
				nsurl_get(url, urlparts[i].part, &part, &sz);
				setenv(urlparts[i].name, part, 1);
				free(part);
			}
		}
	}
	nsurl_unref(url);
	setenv("title", browser_window_get_title(gw->bw), 1);
}

void
nsvi_command(void *user, const char *cmd)
{
	// TODO: Error handling
	struct nsvi_state *state = user;
	populate_env(state);

	wordexp_t p;
	int ret = wordexp(cmd, &p, 0);
	if (ret) {
		NSLOG(netsurf, ERROR, "Unable to expand line: '%s'", cmd);
		char *error_message = malloc(strlen(cmd) + 26);
		sprintf(error_message, "Unable to expand line: '%s'", cmd);
		handle_error(error_message);
		return;
	}
	char **argv = p.we_wordv;
	int argc = p.we_wordc;

	struct command *command = bsearch(argv[0], &commands,
			sizeof(commands) / sizeof(commands[0]),
			sizeof(commands[0]),
			cmd_compar);
	if (!command) {
		char *error_message;
		if (argv[0] == NULL) {
			error_message = strdup("No command given");
		} else {
			error_message = malloc(strlen(argv[0]) + 19);
			sprintf(error_message, "Invalid command '%s'", argv[0]);
		}
		NSLOG(netsurf, ERROR, "%s", error_message);
		handle_error(error_message);
		goto exit;
	}
	NSLOG(netsurf, INFO, "Running command %s", cmd);

	command->exec(state, argc, argv);

exit:
	wordfree(&p);
}

int
nsvi_command_source(void *user, const char *filepath) {
	FILE *f = fopen(filepath, "r");
	if (f == NULL) {
		NSLOG(netsurf, ERROR, "source: unable to open file at '%s'",
			filepath);
		return 1;
	}
	char *line = NULL;
	size_t size = 0;
	ssize_t len = 0;
	while ((len = getline(&line, &size, f)) != -1) {
		if (line[0] == '\n' || line[0] == '#') {
			continue;
		}
		line[len-1] = '\0';
		nsvi_command(user, line);
	}
	free(line);
	fclose(f);
	return 0;
}
