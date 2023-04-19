/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *		  http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <assert.h>
#include <string.h>

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"
#include "parse/properties/properties.h"
#include "parse/properties/utils.h"

/**
 * Parse border-color shorthand
 *
 * \param c	  Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx	  Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *	   CSS_NOMEM on memory exhaustion,
 *	   CSS_INVALID if the input is not valid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *		   If the input is invalid, then \a *ctx remains unchanged.
 */
css_error css__parse_border_color(css_language *c,
		const parserutils_vector *vector, int *ctx,
		css_style *result)
{
	int orig_ctx = *ctx;
	int prev_ctx;
	const css_token *token;
	uint16_t side_val[4];
	uint32_t side_color[4];
	uint32_t side_count = 0;
	enum flag_value flag_value;
	css_error error;

	/* Firstly, handle inherit */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	flag_value = get_css_flag_value(c, token);

	if (flag_value != FLAG_VALUE__NONE) {
		error = css_stylesheet_style_flag_value(result, flag_value,
				CSS_PROP_BORDER_TOP_COLOR);
		if (error != CSS_OK)
			return error;

		error = css_stylesheet_style_flag_value(result, flag_value,
				CSS_PROP_BORDER_RIGHT_COLOR);
		if (error != CSS_OK)
			return error;

		error = css_stylesheet_style_flag_value(result, flag_value,
				CSS_PROP_BORDER_BOTTOM_COLOR);
		if (error != CSS_OK)
			return error;

		error = css_stylesheet_style_flag_value(result, flag_value,
				CSS_PROP_BORDER_LEFT_COLOR);
		if (error == CSS_OK)
			parserutils_vector_iterate(vector, ctx);

		return error;
	}

	/* Attempt to parse up to 4 colours */
	do {
		prev_ctx = *ctx;

		if ((token != NULL) && is_css_inherit(c, token)) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		error = css__parse_colour_specifier(c, vector, ctx, &side_val[side_count], &side_color[side_count]);
		if (error == CSS_OK) {
			side_count++;

			consumeWhitespace(vector, ctx);

			token = parserutils_vector_peek(vector, *ctx);
		} else {
			/* Forcibly cause loop to exit */
			token = NULL;
		}
	} while ((*ctx != prev_ctx) && (token != NULL) && (side_count < 4));


#define SIDE_APPEND(OP,NUM)								\
	error = css__stylesheet_style_appendOPV(result, (OP), 0, side_val[(NUM)]);	\
	if (error != CSS_OK)								\
		break;									\
	if (side_val[(NUM)] == BORDER_COLOR_SET)					\
		error = css__stylesheet_style_append(result, side_color[(NUM)]);		\
	if (error != CSS_OK)								\
		break;

	switch (side_count) {
	case 1:
		SIDE_APPEND(CSS_PROP_BORDER_TOP_COLOR, 0);
		SIDE_APPEND(CSS_PROP_BORDER_RIGHT_COLOR, 0);
		SIDE_APPEND(CSS_PROP_BORDER_BOTTOM_COLOR, 0);
		SIDE_APPEND(CSS_PROP_BORDER_LEFT_COLOR, 0);
		break;
	case 2:
		SIDE_APPEND(CSS_PROP_BORDER_TOP_COLOR, 0);
		SIDE_APPEND(CSS_PROP_BORDER_RIGHT_COLOR, 1);
		SIDE_APPEND(CSS_PROP_BORDER_BOTTOM_COLOR, 0);
		SIDE_APPEND(CSS_PROP_BORDER_LEFT_COLOR, 1);
		break;
	case 3:
		SIDE_APPEND(CSS_PROP_BORDER_TOP_COLOR, 0);
		SIDE_APPEND(CSS_PROP_BORDER_RIGHT_COLOR, 1);
		SIDE_APPEND(CSS_PROP_BORDER_BOTTOM_COLOR, 2);
		SIDE_APPEND(CSS_PROP_BORDER_LEFT_COLOR, 1);
		break;
	case 4:
		SIDE_APPEND(CSS_PROP_BORDER_TOP_COLOR, 0);
		SIDE_APPEND(CSS_PROP_BORDER_RIGHT_COLOR, 1);
		SIDE_APPEND(CSS_PROP_BORDER_BOTTOM_COLOR, 2);
		SIDE_APPEND(CSS_PROP_BORDER_LEFT_COLOR, 3);
		break;
	default:
		error = CSS_INVALID;
		break;
	}

	if (error != CSS_OK)
		*ctx = orig_ctx;

	return error;
}
