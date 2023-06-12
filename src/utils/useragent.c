/*
 * Copyright 2007 Daniel Silverstone <dsilvers@digital-scurf.org>
 * Copyright 2007 Rob Kendrick <rjek@netsurf-browser.org>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <neosurf/utils/config.h>
#include "utils/utsname.h"
#include <neosurf/desktop/version.h>
#include <neosurf/utils/log.h>
#include "utils/useragent.h"

static const char *core_user_agent_string = NULL;

#ifndef NEOSURF_UA_FORMAT_STRING
#define NEOSURF_UA_FORMAT_STRING "Mozilla/5.0 (%s) NetSurf/%d"
#endif

/**
 * Prepare core_user_agent_string with a string suitable for use as a
 * user agent in HTTP requests.
 */
static void
user_agent_build_string(void)
{
        struct utsname un;
        const char *sysname = "Unknown";
        char *ua_string;
        int len;

        if (uname(&un) >= 0) {
                sysname = un.sysname;
		if (strcmp(sysname, "Linux") == 0) {
			/* Force desktop, not mobile */
			sysname = "X11; Linux";
		}
	}

	len = snprintf(NULL, 0, NEOSURF_UA_FORMAT_STRING,
                       sysname,
                       neosurf_version);
        ua_string = malloc(len + 1);
        if (!ua_string) {
                /** \todo this needs handling better */
                return;
        }
        snprintf(ua_string, len + 1,
                 NEOSURF_UA_FORMAT_STRING,
		 sysname,
                 neosurf_version);

        core_user_agent_string = ua_string;

        NSLOG(neosurf, INFO, "Built user agent \"%s\"",
              core_user_agent_string);
}

/* This is a function so that later we can override it trivially */
const char *
user_agent_string(void)
{
        if (core_user_agent_string == NULL)
                user_agent_build_string();
	return core_user_agent_string;
}

/* Public API documented in useragent.h */
void
free_user_agent_string(void)
{
	if (core_user_agent_string != NULL) {
		/* Nasty cast because we need to de-const it to free it */
		free((void *)core_user_agent_string);
		core_user_agent_string = NULL;
	}
}
