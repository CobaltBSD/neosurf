/*
 * Copyright 2012 Vincent Sanders <vince@netsurf-browser.org>
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
 * libdom utilities (implementation).
 */

#include <assert.h>
#include <string.h>
#include <dom/dom.h>

#include "utils/config.h"
#include "utils/log.h"
#include "utils/libdom.h"

/* exported interface documented in libdom.h */
bool libdom_treewalk(dom_node *root,
		bool (*callback)(dom_node *node, dom_string *name, void *ctx),
		void *ctx)
{
	dom_node *node;
	bool result = true;

	node = dom_node_ref(root); /* tree root */

	while (node != NULL) {
		dom_node *next = NULL;
		dom_node_type type;
		dom_string *name;
		dom_exception exc;

		exc = dom_node_get_first_child(node, &next);
		if (exc != DOM_NO_ERR) {
			dom_node_unref(node);
			break;
		}

		if (next != NULL) {
			/* 1. Got children */
			dom_node_unref(node);
			node = next;
		} else {
			/* No children; siblings & ancestor's siblings */
			while (node != NULL) {
				exc = dom_node_get_next_sibling(node, &next);
				if (exc != DOM_NO_ERR) {
					dom_node_unref(node);
					node = NULL;
					break;
				}

				if (next != NULL) {
					/* 2. Got sibling */
					break;
				}

				exc = dom_node_get_parent_node(node, &next);
				if (exc != DOM_NO_ERR) {
					dom_node_unref(node);
					node = NULL;
					break;
				}

				/* 3. Try parent */
				dom_node_unref(node);
				node = next;
			}

			if (node == NULL)
				break;

			dom_node_unref(node);
			node = next;
		}

		assert(node != NULL);

		exc = dom_node_get_node_type(node, &type);
		if ((exc != DOM_NO_ERR) || (type != DOM_ELEMENT_NODE))
			continue;

		exc = dom_node_get_node_name(node, &name);
		if (exc != DOM_NO_ERR)
			continue;

		result = callback(node, name, ctx);

		dom_string_unref(name);

		if (result == false) {
			break; /* callback caused early termination */
		}

	}
	return result;
}


/* libdom_treewalk context for libdom_find_element */
struct find_element_ctx {
	lwc_string *search;
	dom_node *found;
};

/* libdom_treewalk callback for libdom_find_element */
static bool libdom_find_element_callback(dom_node *node, dom_string *name,
		void *ctx)
{
	struct find_element_ctx *data = ctx;

	if (dom_string_caseless_lwc_isequal(name, data->search)) {
		/* Found element */
		data->found = node;
		return false; /* Discontinue search */
	}

	return true; /* Continue search */
}


/* exported interface documented in libdom.h */
dom_node *libdom_find_element(dom_node *node, lwc_string *element_name)
{
	struct find_element_ctx data;

	assert(element_name != NULL);

	if (node == NULL)
		return NULL;

	data.search = element_name;
	data.found = NULL;

	libdom_treewalk(node, libdom_find_element_callback, &data);

	return data.found;
}


/* exported interface documented in libdom.h */
dom_node *libdom_find_first_element(dom_node *parent, lwc_string *element_name)
{
	dom_node *element;
	dom_exception exc;
	dom_string *node_name = NULL;
	dom_node_type node_type;
	dom_node *next_node;

	exc = dom_node_get_first_child(parent, &element);
	if ((exc != DOM_NO_ERR) || (element == NULL)) {
		return NULL;
	}

	/* find first node thats a element */
	do {
		exc = dom_node_get_node_type(element, &node_type);

		if ((exc == DOM_NO_ERR) && (node_type == DOM_ELEMENT_NODE)) {
			exc = dom_node_get_node_name(element, &node_name);
			if ((exc == DOM_NO_ERR) && (node_name != NULL)) {
				if (dom_string_caseless_lwc_isequal(node_name,
						     element_name)) {
					dom_string_unref(node_name);
					break;
				}
				dom_string_unref(node_name);
			}
		}

		exc = dom_node_get_next_sibling(element, &next_node);
		dom_node_unref(element);
		if (exc == DOM_NO_ERR) {
			element = next_node;
		} else {
			element = NULL;
		}
	} while (element != NULL);

	return element;
}

/* exported interface documented in libdom.h */
/* TODO: return appropriate errors */
nserror libdom_iterate_child_elements(dom_node *parent,
		libdom_iterate_cb cb, void *ctx)
{
	dom_nodelist *children;
	uint32_t index, num_children;
	dom_exception error;

	error = dom_node_get_child_nodes(parent, &children);
	if (error != DOM_NO_ERR || children == NULL)
		return NSERROR_NOMEM;

	error = dom_nodelist_get_length(children, &num_children);
	if (error != DOM_NO_ERR) {
		dom_nodelist_unref(children);
		return NSERROR_NOMEM;
	}

	for (index = 0; index < num_children; index++) {
		dom_node *child;
		dom_node_type type;

		error = dom_nodelist_item(children, index, &child);
		if (error != DOM_NO_ERR) {
			dom_nodelist_unref(children);
			return NSERROR_NOMEM;
		}

		error = dom_node_get_node_type(child, &type);
		if (error == DOM_NO_ERR && type == DOM_ELEMENT_NODE) {
			nserror err = cb(child, ctx);
			if (err != NSERROR_OK) {
				dom_node_unref(child);
				dom_nodelist_unref(children);
				return err;
			}
		}

		dom_node_unref(child);
	}

	dom_nodelist_unref(children);

	return NSERROR_OK;
}

/* exported interface documented in libdom.h */
nserror libdom_hubbub_error_to_nserror(dom_hubbub_error error)
{
	switch (error) {

	/* HUBBUB_REPROCESS is not handled here because it can
	 * never occur outside the hubbub treebuilder
	 */

	case DOM_HUBBUB_OK:
		/* parsed ok */
		return NSERROR_OK;

	case (DOM_HUBBUB_HUBBUB_ERR | HUBBUB_PAUSED):
		/* hubbub input paused */
		return NSERROR_OK;

	case DOM_HUBBUB_NOMEM:
		/* out of memory error from DOM */
		return NSERROR_NOMEM;

	case DOM_HUBBUB_BADPARM:
		/* Bad parameter passed to creation */
		return NSERROR_BAD_PARAMETER;

	case DOM_HUBBUB_DOM:
		/* DOM call returned error */
		return NSERROR_DOM;

	case (DOM_HUBBUB_HUBBUB_ERR | HUBBUB_ENCODINGCHANGE):
		/* encoding changed */
		return NSERROR_ENCODING_CHANGE;

	case (DOM_HUBBUB_HUBBUB_ERR | HUBBUB_NOMEM):
		/* out of memory error from parser */
		return NSERROR_NOMEM;

	case (DOM_HUBBUB_HUBBUB_ERR | HUBBUB_BADPARM):
		return NSERROR_BAD_PARAMETER;

	case (DOM_HUBBUB_HUBBUB_ERR | HUBBUB_INVALID):
		return NSERROR_INVALID;

	case (DOM_HUBBUB_HUBBUB_ERR | HUBBUB_FILENOTFOUND):
		return NSERROR_NOT_FOUND;

	case (DOM_HUBBUB_HUBBUB_ERR | HUBBUB_NEEDDATA):
		return NSERROR_NEED_DATA;

	case (DOM_HUBBUB_HUBBUB_ERR | HUBBUB_BADENCODING):
		return NSERROR_BAD_ENCODING;

	case (DOM_HUBBUB_HUBBUB_ERR | HUBBUB_UNKNOWN):
		/* currently only generated by the libdom hubbub binding */
		return NSERROR_DOM;
	default:
		/* unknown error */
		/** @todo better error handling and reporting */
		return NSERROR_UNKNOWN;
	}
	return NSERROR_UNKNOWN;
}


static void ignore_dom_msg(uint32_t severity, void *ctx, const char *msg, ...)
{
}



/**
 * Dump attribute/value for an element node
 *
 * \param node  The element node to dump attribute details for
 * \param f file handle to dump to.
 * \param attribute The attribute to dump
 * \return true on success, or false on error
 */
static bool dump_dom_element_attribute(dom_node *node, FILE *f, const char *attribute)
{
	dom_exception exc;
	dom_string *attr = NULL;
	dom_string *attr_value = NULL;
	dom_node_type type;
	const char *string;
	size_t length;

	/* Should only have element nodes here */
	exc = dom_node_get_node_type(node, &type);
	if (exc != DOM_NO_ERR) {
		fprintf(f, " Exception raised for node_get_node_type\n");
		return false;
	}
	assert(type == DOM_ELEMENT_NODE);

	/* Create a dom_string containing required attribute name. */
	exc = dom_string_create_interned((uint8_t *)attribute,
					 strlen(attribute), &attr);
	if (exc != DOM_NO_ERR) {
		fprintf(f, " Exception raised for dom_string_create\n");
		return false;
	}

	/* Get class attribute's value */
	exc = dom_element_get_attribute(node, attr, &attr_value);
	if (exc != DOM_NO_ERR) {
		fprintf(f, " Exception raised for element_get_attribute\n");
		dom_string_unref(attr);
		return false;
	} else if (attr_value == NULL) {
		/* Element lacks required attribute */
		dom_string_unref(attr);
		return true;
	}

	/* Finished with the attr dom_string */
	dom_string_unref(attr);

	/* Get attribute value's string data */
	string = dom_string_data(attr_value);
	length = dom_string_byte_length(attr_value);

	/* Print attribute info */
	fprintf(f, " %s=\"%.*s\"", attribute, (int)length, string);

	/* Finished with the attr_value dom_string */
	dom_string_unref(attr_value);

	return true;
}


/**
 * Print a line in a DOM structure dump for an element
 *
 * \param node   The node to dump
 * \param f file handle to dump to.
 * \param depth  The node's depth
 * \return  true on success, or false on error
 */
static bool dump_dom_element(dom_node *node, FILE *f, int depth)
{
	dom_exception exc;
	dom_string *node_name = NULL;
	dom_node_type type;
	int i;
	const char *string;
	size_t length;

	/* Only interested in element nodes */
	exc = dom_node_get_node_type(node, &type);
	if (exc != DOM_NO_ERR) {
		fprintf(f, "Exception raised for node_get_node_type\n");
		return false;
	} else if (type != DOM_ELEMENT_NODE) {
		/* Nothing to print */
		return true;
	}

	/* Get element name */
	exc = dom_node_get_node_name(node, &node_name);
	if (exc != DOM_NO_ERR) {
		fprintf(f, "Exception raised for get_node_name\n");
		return false;
	} else if (node_name == NULL) {
		fprintf(f, "Broken: root_name == NULL\n");
		return false;
	}

	/* Print ASCII tree structure for current node */
	if (depth > 0) {
		for (i = 0; i < depth; i++) {
			fprintf(f, "| ");
		}
		fprintf(f, "+-");
	}

	/* Get string data and print element name */
	string = dom_string_data(node_name);
	length = dom_string_byte_length(node_name);
	fprintf(f, "[%.*s]", (int)length, string);

	if (length == 5 && strncmp(string, "title", 5) == 0) {
		/* Title tag, gather the title */
		dom_string *str;
		exc = dom_node_get_text_content(node, &str);
		if (exc == DOM_NO_ERR && str != NULL) {
			fprintf(f, " $%.*s$", (int)dom_string_byte_length(str),
				dom_string_data(str));
			dom_string_unref(str);
		}
	}

	/* Finished with the node_name dom_string */
	dom_string_unref(node_name);

	/* Print the element's id & class, if it has them */
	if (dump_dom_element_attribute(node, f, "id") == false ||
	    dump_dom_element_attribute(node, f, "class") == false) {
		/* Error occured */
		fprintf(f, "\n");
		return false;
	}

	fprintf(f, "\n");
	return true;
}


/* exported interface documented in libdom.h */
nserror libdom_dump_structure(dom_node *node, FILE *f, int depth)
{
	dom_exception exc;
	dom_node *child;
	nserror ret;
	dom_node *next_child;

	/* Print this node's entry */
	if (dump_dom_element(node, f, depth) == false) {
		/* There was an error; return */
		return NSERROR_DOM;
	}

	/* Get the node's first child */
	exc = dom_node_get_first_child(node, &child);
	if (exc != DOM_NO_ERR) {
		fprintf(f, "Exception raised for node_get_first_child\n");
		return NSERROR_DOM;
	} else if (child != NULL) {
		/* node has children;  decend to children's depth */
		depth++;

		/* Loop though all node's children */
		do {
			/* Visit node's descendents */
			ret = libdom_dump_structure(child, f, depth);
			if (ret !=NSERROR_OK) {
				/* There was an error; return */
				dom_node_unref(child);
				return NSERROR_DOM;
			}

			/* Go to next sibling */
			exc = dom_node_get_next_sibling(child, &next_child);
			if (exc != DOM_NO_ERR) {
				fprintf(f, "Exception raised for node_get_next_sibling\n");
				dom_node_unref(child);
				return NSERROR_DOM;
			}

			dom_node_unref(child);
			child = next_child;
		} while (child != NULL); /* No more children */
	}

	return NSERROR_OK;
}


/* exported interface documented in libdom.h */
nserror libdom_parse_file(const char *filename, const char *encoding, dom_document **doc)
{
	dom_hubbub_parser_params parse_params;
	dom_hubbub_error error;
	dom_hubbub_parser *parser;
	dom_document *document;
	FILE *fp = NULL;
#define BUF_SIZE 512
	uint8_t buf[BUF_SIZE];

	fp = fopen(filename, "r");
	if (fp == NULL) {
		return NSERROR_NOT_FOUND;
	}

	parse_params.enc = encoding;
	parse_params.fix_enc = false;
	parse_params.enable_script = false;
	parse_params.msg = ignore_dom_msg;
	parse_params.script = NULL;
	parse_params.ctx = NULL;
	parse_params.daf = NULL;

	error = dom_hubbub_parser_create(&parse_params, &parser, &document);
	if (error != DOM_HUBBUB_OK) {
		fclose(fp);
		return libdom_hubbub_error_to_nserror(error);
	}

	while (feof(fp) == 0) {
		size_t read = fread(buf, sizeof(buf[0]), BUF_SIZE, fp);

		error = dom_hubbub_parser_parse_chunk(parser, buf, read);
		if (error != DOM_HUBBUB_OK) {
			dom_node_unref(document);
			dom_hubbub_parser_destroy(parser);
			fclose(fp);
			return NSERROR_DOM;
		}
	}

	error = dom_hubbub_parser_completed(parser);
	if (error != DOM_HUBBUB_OK) {
		dom_node_unref(document);
		dom_hubbub_parser_destroy(parser);
		fclose(fp);
		return libdom_hubbub_error_to_nserror(error);
	}

	dom_hubbub_parser_destroy(parser);
	fclose(fp);

	*doc = document;
	return NSERROR_OK;
}
