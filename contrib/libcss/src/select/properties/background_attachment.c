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

css_error css__cascade_background_attachment(uint32_t opv, css_style *style,
		css_select_state *state)
{
	uint16_t value = CSS_BACKGROUND_ATTACHMENT_INHERIT;

	UNUSED(style);

	if (hasFlagValue(opv) == false) {
		switch (getValue(opv)) {
		case BACKGROUND_ATTACHMENT_FIXED:
			value = CSS_BACKGROUND_ATTACHMENT_FIXED;
			break;
		case BACKGROUND_ATTACHMENT_SCROLL:
			value = CSS_BACKGROUND_ATTACHMENT_SCROLL;
			break;
		}
	}

	if (css__outranks_existing(getOpcode(opv), isImportant(opv), state,
			getFlagValue(opv))) {
		return set_background_attachment(state->computed, value);
	}

	return CSS_OK;
}

css_error css__set_background_attachment_from_hint(const css_hint *hint,
		css_computed_style *style)
{
	return set_background_attachment(style, hint->status);
}

css_error css__initial_background_attachment(css_select_state *state)
{
	return set_background_attachment(state->computed,
			CSS_BACKGROUND_ATTACHMENT_SCROLL);
}

css_error css__copy_background_attachment(
		const css_computed_style *from,
		css_computed_style *to)
{
	if (from == to) {
		return CSS_OK;
	}

	return set_background_attachment(to, get_background_attachment(from));
}

css_error css__compose_background_attachment(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	uint8_t type = get_background_attachment(child);

	return css__copy_background_attachment(
			type == CSS_BACKGROUND_ATTACHMENT_INHERIT ? parent : child,
			result);
}

