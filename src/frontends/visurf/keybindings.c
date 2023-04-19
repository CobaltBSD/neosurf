#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <xkbcommon/xkbcommon.h>
#include "visurf/keybindings.h"
#include "utils/errors.h"
#include "utils/log.h"

struct mod_map {
	const char *xkb;
	enum wl_keyboard_modifier mask;
};

static const struct mod_map mod_map[] = {
	{ XKB_MOD_NAME_CTRL, MOD_CTRL },
	{ XKB_MOD_NAME_ALT, MOD_ALT },
	{ XKB_MOD_NAME_LOGO, MOD_LOGO },
};

uint32_t
xkb_state_mask(struct xkb_state *state)
{
	uint32_t mask = 0;
	for (size_t i = 0; i < sizeof(mod_map) / sizeof(mod_map[0]); ++i) {
		if (xkb_state_mod_name_is_active(state,
				mod_map[i].xkb, XKB_STATE_MODS_EFFECTIVE)) {
			mask |= mod_map[i].mask;
		}
	}
	return mask;
}

static void
binding_free(struct nsvi_binding binding)
{
	if (binding.next) {
		binding_free(*binding.next);
		free(binding.next);
	}
	free(binding.command);
}

static struct nsvi_binding *
binding_parse(const char *desc)
{
	struct nsvi_binding *binding = calloc(1, sizeof(struct nsvi_binding));
	struct nsvi_binding *cur = binding;
	for (; desc[0]; ++desc) {
		char buf[64];
		xkb_keysym_t keysym;
		switch (desc[0]) {
		case '<':
			++desc;
			if (!desc[0]) {
				goto error;
			}
			if (desc[1] == '-') {
				// TODO: Other modifiers?
				switch (desc[0]) {
				case 'C':
					cur->mask |= MOD_CTRL;
					break;
				}
				desc += 2;
			}
			size_t i;
			for (i = 0; desc[i] && i < sizeof(buf) - 1; ++i) {
				if (desc[i] == '>') {
					break;
				}
			}
			strncpy(buf, desc, i);
			buf[i] = 0;
			desc += i;
			break;
		case '(':
			assert(0); // TODO: (Chord)
		default:
			buf[0] = desc[0], buf[1] = 0;
			break;
		}

		keysym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
		if (keysym == XKB_KEY_NoSymbol) {
			goto error;
		}

		cur->keysym = keysym;

		if (desc[1]) {
			cur->next = calloc(1, sizeof(struct nsvi_binding));
			cur = cur->next;
		}
	}

	return binding;
error:
	binding_free(*binding);
	free(binding);
	return NULL;
}

nserror
nsvi_bindings_new(struct nsvi_bindings *state,
		const char *desc, const char *cmd)
{
	struct nsvi_binding *binding = binding_parse(desc);
	if (binding == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	if (state->nbindings + 1 >= state->zbindings) {
		if (state->zbindings == 0) {
			state->bindings = calloc(16, sizeof(struct nsvi_binding));
			state->zbindings = 16;
		} else {
			state->zbindings *= 2;
			struct nsvi_binding *new = realloc(state->bindings,
				state->zbindings * sizeof(struct nsvi_binding));
			assert(new);
			state->bindings = new;
		}
	}

	// TODO: I'm too lazy to write a binary search or to adapt bsearch for
	// this case right now
	size_t i;
	for (i = 0; i < state->nbindings; ++i) {
		if (state->bindings[i].keysym >= binding->keysym) {
			break;
		}
	}
	memmove(&state->bindings[i+1], &state->bindings[i],
		(state->nbindings-i) * sizeof(struct nsvi_binding));
	binding->command = strdup(cmd);
	state->bindings[i] = *binding;
	++state->nbindings;
	free(binding);
	return NSERROR_OK;
}

nserror
nsvi_bindings_remove(struct nsvi_bindings *state, const char *desc)
{
	struct nsvi_binding *binding = binding_parse(desc);
	if (binding == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	for (size_t i = 0; i < state->nbindings; i++) {
		struct nsvi_binding *a = binding, *b = &state->bindings[i];
		while (a && b) {
			if (a->keysym != b->keysym || a->mask != b->mask) {
				break;
			}
			a = a->next;
			b = b->next;
		}
		if (!a && !b) {
			binding_free(state->bindings[i]);
			memmove(&state->bindings[i], &state->bindings[i + 1],
				(state->nbindings - i - 1) * sizeof(struct nsvi_binding));
			i--;
			state->nbindings--;
		}
	}
	binding_free(*binding);
	free(binding);
	return NSERROR_OK;
}

enum binding_state {
	STATE_PARTIAL,
	STATE_COMPLETE,
	STATE_NONE,
};

static enum binding_state
get_binding_state(struct nsvi_bindings *state, struct nsvi_binding *binding)
{
	struct nsvi_binding *cand = binding;
	size_t i = 0;
	for (; i < state->nbuf && cand; cand = cand->next, ++i) {
		if (cand->keysym != state->buffer[i].keysym) {
			return STATE_NONE;
		}
		if (cand->mask != state->buffer[i].mask) {
			return STATE_NONE;
		}
	}
	if (i == state->nbuf && cand) {
		return STATE_PARTIAL;
	}
	return STATE_COMPLETE;
}

void
nsvi_bindings_handle(struct nsvi_bindings *state,
		xkb_keycode_t keycode, bool pressed)
{
	xkb_keysym_t sym = xkb_state_key_get_one_sym(state->xkb_state, keycode);
	if (state->nbuf >= NSVI_KEYBUF_SIZE) {
		NSLOG(netsurf, ERROR, "Keyboard buffer overflowed");
		state->nbuf = 0;
		return;
	}
	if (sym >= XKB_KEY_Shift_L && sym <= XKB_KEY_Hyper_R) {
		return;
	}
	if (!pressed) {
		// TODO: More sophisticated press/release semantics; wait for
		// key release before recording next key (or add first-class
		// chording?)
		return;
	}
	state->buffer[state->nbuf++] = (struct nsvi_key_event){
		.keysym = sym,
		.mask = xkb_state_mask(state->xkb_state),
	};

	// TODO: Binary search
	size_t pending = 0;
	for (size_t i = 0; i < state->nbindings; ++i) {
		struct nsvi_binding *cand = &state->bindings[i];
		const char *cmd = cand->command;
		enum binding_state bstate = get_binding_state(state, cand);
		switch (bstate) {
		case STATE_COMPLETE:
			NSLOG(netsurf, INFO, "Executing binding '%s'", cmd);
			state->nbuf = 0;
			state->exec(state->user, cmd);
			return;
		case STATE_PARTIAL:
			++pending;
			break;
		case STATE_NONE:
			break;
		}
	}

	if (pending == 0) {
		NSLOG(netsurf, INFO, "No bindings for this key sequence, discarding");
		state->nbuf = 0;
	} else {
		NSLOG(netsurf, INFO, "No candidate bindings (yet)");
	}
}

void
nsvi_bindings_finish(struct nsvi_bindings *state)
{
	for (size_t i = 0; i < state->nbindings; i++) {
		binding_free(state->bindings[i]);
	}
	free(state->bindings);
}
