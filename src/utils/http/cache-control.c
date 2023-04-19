/*
 * Copyright 2019 John-Mark Bell <jmb@netsurf-browser.org>
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

#include <limits.h>
#include <stdlib.h>

#include "utils/corestrings.h"
#include "utils/http.h"

#include "utils/http/generics.h"
#include "utils/http/primitives.h"

/**
 * Representation of a Cache-Control
 */
struct http_cache_control {
	uint32_t max_age;		/**< Max age (delta seconds) */
	bool max_age_valid;		/**< Whether max-age is valid */
	bool no_cache;			/**< Whether caching is forbidden */
	bool no_store;			/**< Whether persistent caching is forbidden */
};

/**
 * Representation of a directive
 */
typedef struct http_directive {
	http__item base;

	lwc_string *name;		/**< Parameter name */
	lwc_string *value;		/**< Parameter value (optional) */
} http_directive;


static void http_destroy_directive(http_directive *self)
{
	lwc_string_unref(self->name);
	if (self->value != NULL) {
		lwc_string_unref(self->value);
	}
	free(self);
}

static nserror http__parse_directive(const char **input,
		http_directive **result)
{
	const char *pos = *input;
	lwc_string *name;
	lwc_string *value = NULL;
	http_directive *directive;
	nserror error;

	/* token [ "=" ( token | quoted-string ) ] */

	error = http__parse_token(&pos, &name);
	if (error != NSERROR_OK)
		return error;

	http__skip_LWS(&pos);

	if (*pos == '=') {
		pos++;

		http__skip_LWS(&pos);

		if (*pos == '"')
			error = http__parse_quoted_string(&pos, &value);
		else
			error = http__parse_token(&pos, &value);

		if (error != NSERROR_OK) {
			lwc_string_unref(name);
			return error;
		}
	}

	directive = malloc(sizeof(*directive));
	if (directive == NULL) {
		if (value != NULL) {
			lwc_string_unref(value);
		}
		lwc_string_unref(name);
		return NSERROR_NOMEM;
	}

	HTTP__ITEM_INIT(directive, NULL, http_destroy_directive);
	directive->name = name;
	directive->value = value;

	*result = directive;
	*input = pos;

	return NSERROR_OK;
}

static void http_directive_list_destroy(http_directive *list)
{
	http__item_list_destroy(list);
}

static nserror http_directive_list_find_item(const http_directive *list,
		lwc_string *name, lwc_string **value)
{
	bool match;

	while (list != NULL) {
		if (lwc_string_caseless_isequal(name, list->name,
				&match) == lwc_error_ok && match)
			break;

		list = (http_directive *) list->base.next;
	}

	if (list == NULL)
		return NSERROR_NOT_FOUND;

	if (list->value != NULL) {
		*value = lwc_string_ref(list->value);
	} else {
		*value = NULL;
	}

	return NSERROR_OK;
}

static const http_directive *http_directive_list_iterate(
		const http_directive *cur,
		lwc_string **name, lwc_string **value)
{
	if (cur == NULL)
		return NULL;

	*name = lwc_string_ref(cur->name);
	if (cur->value != NULL) {
		*value = lwc_string_ref(cur->value);
	} else {
		*value = NULL;
	}

	return (http_directive *) cur->base.next;
}

static uint32_t count(const http_directive *list, lwc_string *key)
{
	uint32_t count = 0;
	bool match;

	while (list != NULL) {
		if (lwc_string_caseless_isequal(key, list->name,
				&match) == lwc_error_ok && match) {
			count++;
		}

		list = (http_directive *) list->base.next;
	}

	return count;
}

static bool check_duplicates(const http_directive *directives)
{
	bool result = true;
	const http_directive *key = directives;

	if (key == NULL) {
		/* No directives, so there can't be any duplicates */
		return true;
	}

	do {
		lwc_string *name = NULL, *value = NULL;

		key = http_directive_list_iterate(key, &name, &value);

		result &= (count(directives, name) == 1);

		lwc_string_unref(name);
		if (value != NULL) {
			lwc_string_unref(value);
		}
	} while (key != NULL);

	return result;
}

static nserror parse_max_age(lwc_string *value, uint32_t *result)
{
	const char *pos = lwc_string_data(value);
	const char *end = pos + lwc_string_length(value);
	uint32_t val = 0;

	/* 1*DIGIT */

	if (pos == end) {
		/* Blank value */
		return NSERROR_NOT_FOUND;
	}

	while (pos < end) {
		if ('0' <= *pos && *pos <= '9') {
			uint32_t nv = val * 10 + (*pos - '0');
			if (nv < val) {
				val = UINT_MAX;
			} else {
				val = nv;
			}
		} else {
			/* Non-digit */
			return NSERROR_NOT_FOUND;
		}

		pos++;
	}

	*result = val;

	return NSERROR_OK;
}

/* See cache-control.h for documentation */
nserror http_parse_cache_control(const char *header_value,
		http_cache_control **result)
{
	const char *pos = header_value;
	http_cache_control *cc;
	http_directive *first = NULL;
	http_directive *directives = NULL;
	lwc_string *value_str = NULL;
	uint32_t max_age = 0;
	bool max_age_valid = false;
	bool no_cache = false;
	bool no_store = false;
	nserror error;

	/* 1#cache-directive */

	http__skip_LWS(&pos);

	error = http__parse_directive(&pos, &first);
	if (error != NSERROR_OK) {
		return error;
	}

	http__skip_LWS(&pos);

	if (*pos == ',') {
		error = http__item_list_parse(&pos,
				http__parse_directive, first, &directives);
		if (error != NSERROR_OK) {
			if (directives != NULL) {
				http_directive_list_destroy(directives);
			}
			return error;
		}
	} else {
		directives = first;
	}

	/* Each directive must only appear once */
	if (check_duplicates(directives) == false) {
		http_directive_list_destroy(directives);
		return NSERROR_NOT_FOUND;
	}

	/* Find max-age */
	error = http_directive_list_find_item(directives,
			corestring_lwc_max_age, &value_str);
	if (error == NSERROR_OK && value_str != NULL) {
		error = parse_max_age(value_str, &max_age);
		max_age_valid = (error == NSERROR_OK);
		lwc_string_unref(value_str);
	}

	/* Find no-cache */
	error = http_directive_list_find_item(directives,
			corestring_lwc_no_cache, &value_str);
	if (error == NSERROR_OK) {
		no_cache = true;
		if (value_str != NULL) {
			lwc_string_unref(value_str);
		}
	}

	/* Find no-store */
	error = http_directive_list_find_item(directives,
			corestring_lwc_no_store, &value_str);
	if (error == NSERROR_OK) {
		no_store = true;
		if (value_str != NULL) {
			lwc_string_unref(value_str);
		}
	}

	http_directive_list_destroy(directives);

	cc = malloc(sizeof(*cc));
	if (cc == NULL) {
		return NSERROR_NOMEM;
	}

	cc->max_age = max_age;
	cc->max_age_valid = max_age_valid;
	cc->no_cache = no_cache;
	cc->no_store = no_store;

	*result = cc;

	return NSERROR_OK;
}

/* See cache-control.h for documentation */
void http_cache_control_destroy(http_cache_control *victim)
{
	free(victim);
}

/* See cache-control.h for documentation */
bool http_cache_control_has_max_age(http_cache_control *cc)
{
	return cc->max_age_valid;
}

/* See cache-control.h for documentation */
uint32_t http_cache_control_max_age(http_cache_control *cc)
{
	return cc->max_age;
}

/* See cache-control.h for documentation */
bool http_cache_control_no_cache(http_cache_control *cc)
{
	return cc->no_cache;
}

/* See cache-control.h for documentation */
bool http_cache_control_no_store(http_cache_control *cc)
{
	return cc->no_store;
}
