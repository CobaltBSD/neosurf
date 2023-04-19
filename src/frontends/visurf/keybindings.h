#ifndef NETSURF_VI_KEYBINDINGS
#define NETSURF_VI_KEYBINDINGS
#include <stdbool.h>
#include <stdint.h>
#include <xkbcommon/xkbcommon.h>
#include "utils/errors.h"

#define NSVI_KEYBUF_SIZE 8

enum wl_keyboard_modifier {
	MOD_SHIFT = 1 << 0,
	MOD_CAPS  = 1 << 1,
	MOD_CTRL  = 1 << 2,
	MOD_ALT   = 1 << 3,
	MOD_MOD2  = 1 << 4,
	MOD_MOD3  = 1 << 5,
	MOD_LOGO  = 1 << 6,
	MOD_MOD5  = 1 << 7,
};

struct nsvi_binding {
	// Linked list of keysyms in order
	xkb_keysym_t keysym;
	uint32_t mask;
	char *command;
	struct nsvi_binding *next;
};

struct nsvi_key_event {
	xkb_keysym_t keysym;
	uint32_t mask;
};

struct nsvi_bindings {
	struct xkb_state *xkb_state;
	struct nsvi_key_event buffer[NSVI_KEYBUF_SIZE];
	size_t nbuf;

	// Ordered by keysym ascending for bsearch
	struct nsvi_binding *bindings;
	size_t nbindings, zbindings;

	void (*exec)(void *user, const char *command);
	void *user;
};

nserror nsvi_bindings_new(struct nsvi_bindings *state,
		const char *desc, const char *cmd);
nserror nsvi_bindings_remove(struct nsvi_bindings *state, const char *desc);
void nsvi_bindings_finish(struct nsvi_bindings *state);
void nsvi_bindings_handle(struct nsvi_bindings *state,
		xkb_keycode_t keycode, bool pressed);
uint32_t xkb_state_mask(struct xkb_state *state);

#endif
