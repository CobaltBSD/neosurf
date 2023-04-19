#include <stdlib.h>
#include "visurf/undo.h"

void nsvi_undo_tab_new(nsurl *url) {
	struct nsvi_undo *undo = calloc(1, sizeof(struct nsvi_undo));
	undo->kind = UNDO_TAB;
	undo->urls = calloc(1, sizeof(nsurl *));
	undo->nurl = 1;
	undo->urls[0] = url;
	undo->next = global_state->undo;
	global_state->undo = undo;
}

void nsvi_undo_window_new(void) {
	assert(0); // TODO
}

void nsvi_undo_free(struct nsvi_undo *undo) {
	for (size_t i = 0; i < undo->nurl; ++i) {
		nsurl_unref(undo->urls[i]);
	}

	free(undo->urls);
	global_state->undo = undo->next;
	free(undo);
}

void nsvi_undo_finish(void) {
	while (global_state->undo) {
		nsvi_undo_free(global_state->undo);
	}
}
