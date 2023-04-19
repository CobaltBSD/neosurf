/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *		  http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"
#include "select/propset.h"
#include "select/propget.h"
#include "utils/utils.h"

#include "select/properties/properties.h"
#include "select/properties/helpers.h"

css_error css__cascade_overflow_x(uint32_t opv, css_style *style,
		css_select_state *state)
{
	uint16_t value = CSS_OVERFLOW_INHERIT;

	UNUSED(style);

	if (hasFlagValue(opv) == false) {
		switch (getValue(opv)) {
		case OVERFLOW_VISIBLE:
			value = CSS_OVERFLOW_VISIBLE;
			break;
		case OVERFLOW_HIDDEN:
			value = CSS_OVERFLOW_HIDDEN;
			break;
		case OVERFLOW_SCROLL:
			value = CSS_OVERFLOW_SCROLL;
			break;
		case OVERFLOW_AUTO:
			value = CSS_OVERFLOW_AUTO;
			break;
		}
	}

	if (css__outranks_existing(getOpcode(opv), isImportant(opv), state,
			getFlagValue(opv))) {
		return set_overflow_x(state->computed, value);
	}

	return CSS_OK;
}

css_error css__set_overflow_x_from_hint(const css_hint *hint,
		css_computed_style *style)
{
	return set_overflow_x(style, hint->status);
}

css_error css__initial_overflow_x(css_select_state *state)
{
	return set_overflow_x(state->computed, CSS_OVERFLOW_VISIBLE);
}

css_error css__copy_overflow_x(
		const css_computed_style *from,
		css_computed_style *to)
{
	if (from == to) {
		return CSS_OK;
	}

	return set_overflow_x(to, get_overflow_x(from));
}

css_error css__compose_overflow_x(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	uint8_t type = get_overflow_x(child);

	return css__copy_overflow_x(
			type == CSS_OVERFLOW_INHERIT ? parent : child,
			result);
}

