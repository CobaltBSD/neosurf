#ifndef NETSURF_VI_UNDO_H
#define NETSURF_VI_UNDO_H

#include "visurf/visurf.h"

enum nsvi_undo_kind {
        UNDO_TAB,
        UNDO_WIN,
};

struct nsvi_undo {
        enum nsvi_undo_kind kind;
        nsurl **urls;
        size_t nurl;
        struct nsvi_undo *next;
};

void nsvi_undo_tab_new(nsurl *url);

void nsvi_undo_window_new(void);

void nsvi_undo_free(struct nsvi_undo *undo);

void nsvi_undo_finish(void);

#endif
