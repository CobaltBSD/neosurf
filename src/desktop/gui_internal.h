/*
 * Copyright 2014 Vincent Sanders <vince@netsurf-browser.org>
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
 *
 * Interface to core interface table.
 *
 * \note must not be used by frontends directly.
 */

#ifndef _NETSURF_DESKTOP_GUI_INTERNAL_H_
#define _NETSURF_DESKTOP_GUI_INTERNAL_H_

#include "desktop/gui_table.h"

/**
 * The global operation table.
 */
extern struct netsurf_table *guit;

#endif
