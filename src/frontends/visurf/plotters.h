/*
 * Copyright 2005 James Bursa <bursa@users.sourceforge.net>
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file
 * Target independent plotting GTK+ interface.
 */

#ifndef NETSURF_VI_PLOTTERS_H
#define NETSURF_VI_PLOTTERS_H 1
#include "netsurf/types.h"

struct plotter_table;

extern const struct plotter_table nsvi_plotters;

void nsvi_set_colour(colour c);
void nsvi_plot_caret(int x, int y, int h);

#endif /* NETSURF_GTK_PLOTTERS_H */

