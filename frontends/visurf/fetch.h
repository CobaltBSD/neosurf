#ifndef NETSURF_VI_FETCH_H
#define NETSURF_VI_FETCH_H

struct gui_fetch_table;
extern struct gui_fetch_table vi_fetch_table;

void nsvi_fetch_filetype_init(const char *mimefile);
void nsvi_fetch_filetype_fini(void);

#endif
