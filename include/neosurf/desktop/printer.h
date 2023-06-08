/*
 * Copyright 2008 Adam Blokus <adamblokus@gmail.com>
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
 * Printer interface.
 *
 * Printer interface to generic plotters, initialization, handling
 * pages and cleaning up.
 */

#ifndef NETSURF_DESKTOP_PRINTER_H
#define NETSURF_DESKTOP_PRINTER_H

struct plotter_table;
struct print_settings;

/** Printer interface */
struct printer{
	const struct plotter_table *plotter;

	bool (*print_begin) (struct print_settings*);

	bool (*print_next_page)(void);

	void (*print_end)(void);
};

#endif
