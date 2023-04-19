/*
 * Copyright 2009 Paul Blokus <paul_pl@users.sourceforge.net>
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

/** \file
 * Cookies (interface).
 */

#ifndef __NSGTK_COOKIES_H__
#define __NSGTK_COOKIES_H__

/**
 * make the cookie window visible.
 *
 * \return NSERROR_OK on success else appropriate error code on faliure.
 */
nserror nsgtk_cookies_present(const char *search_term);

/**
 * Free any resources allocated for the cookie window.
 *
 * \return NSERROR_OK on success else appropriate error code on faliure.
 */
nserror nsgtk_cookies_destroy(void);


#endif /* __NSGTK_COOKIES_H__ */
