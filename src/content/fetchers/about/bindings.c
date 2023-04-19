/*
 * Copyright 2020 Vincent Sanders <vince@netsurf-browser.org>
 * Copyright 2022 Steef Hegeman <mail@steefhegeman.com>
 *
 * This file is part of NetSurf.
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
 * content generator for the about scheme bindings page
 */

#include <stdbool.h>
#include <stdio.h>
#include <xkbcommon/xkbcommon.h>

#include "utils/errors.h"
#include "utils/utils.h"
#include "visurf/visurf.h"

#include "private.h"
#include "bindings.h"

static int snprint_binding(char *buf, size_t size, struct nsvi_binding *binding)
{
	size_t len = 0;
	int klen;
	char tmp[64];
	do {
		klen = xkb_keysym_get_name(binding->keysym, tmp, sizeof tmp);
		if (klen <= 0) {
			klen = snprintf(tmp, sizeof tmp, "<i>invalid</i>");
		}

		if (binding->mask) {
			klen++;
		}

		if (klen == 1) {
			len += snprintf(buf + len, size - len, tmp);
		} else {
			len += snprintf(buf + len, size - len, "&lt;");
			// TODO: Other modifiers?
			if (binding->mask & MOD_CTRL) {
				len += snprintf(buf + len, size - len, "C-");
			}
			len += snprintf(buf + len, size - len, "%s&gt;", tmp);
		}
	} while ((binding = binding->next));
	return len;
}

/**
 * Handler to generate about scheme bindings page
 *
 * \param ctx The fetcher context.
 * \return true if handled false if aborted.
 */
bool fetch_about_bindings_handler(struct fetch_about_context *ctx)
{
/*	char buf[256];
	nserror res;
	bool even = false;
	const char * const ODD_EVEN[] = {"odd", "even"};
	struct nsvi_bindings *bindings;

	if (global_state == NULL) {
		goto fetch_about_bindings_handler_aborted;
	}
	bindings = &global_state->bindings;

	fetch_about_set_http_code(ctx, 200);
	if (fetch_about_send_header(ctx, "Content-Type: text/html")) {
		goto fetch_about_bindings_handler_aborted;
	}

	res = fetch_about_ssenddataf(ctx,
			"<html>\n<head>\n"
			"<title>ViSurf Key Bindings</title>\n"
			"<link rel=\"stylesheet\" type=\"text/css\" "
			"href=\"resource:internal.css\">\n"
			"</head>\n"
			"<body "
				"id =\"configlist\" "
				"class=\"ns-even-bg ns-even-fg ns-border\" "
				"style=\"overflow: hidden;\">\n"
			"<h1 class=\"ns-border\">ViSurf Key Bindings</h1>\n"
			"<table class=\"config\">\n"
			"<tr><th>Key(s)</th>"
			"<th>Command</th></tr>\n");
	if (res != NSERROR_OK) {
		goto fetch_about_bindings_handler_aborted;
	}

	for (size_t i = 0; i < bindings->nbindings; ++i, even = !even) {
		struct nsvi_binding *binding = &bindings->bindings[i];
		// TODO: Gently handle length > sizeof buf (realloc?)
		if (snprint_binding(buf, sizeof buf, binding) < 0) {
			goto fetch_about_bindings_handler_aborted;
		}

		res = fetch_about_ssenddataf(ctx,
				"<tr class=\"ns-%s-bg\">"
					"<th class=\"ns-border\">%s</th>"
					"<td class=\"ns-border\">%s</td>"
				"</tr>\n",
				ODD_EVEN[even], buf, binding->command);
		if (res != NSERROR_OK) {
			goto fetch_about_bindings_handler_aborted;
		}
	}

	res = fetch_about_ssenddataf(ctx, "</table>\n</body>\n</html>\n");
	if (res != NSERROR_OK) {
		goto fetch_about_bindings_handler_aborted;
	}
	fetch_about_send_finished(ctx);
*/
	fetch_about_set_http_code(ctx, 200);
	fetch_about_ssenddataf(ctx, "disabled temporarily");
	fetch_about_send_header(ctx, "Content-Type: text/html");
	fetch_about_send_finished(ctx);
	return true;

fetch_about_bindings_handler_aborted:
	return false;
}
