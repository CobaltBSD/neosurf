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

css_error css__cascade_border_spacing(uint32_t opv, css_style *style,
		css_select_state *state)
{
	uint16_t value = CSS_BORDER_SPACING_INHERIT;
	css_fixed hlength = 0;
	css_fixed vlength = 0;
	uint32_t hunit = UNIT_PX;
	uint32_t vunit = UNIT_PX;

	if (hasFlagValue(opv) == false) {
		value = CSS_BORDER_SPACING_SET;
		hlength = *((css_fixed *) style->bytecode);
		advance_bytecode(style, sizeof(hlength));
		hunit = *((uint32_t *) style->bytecode);
		advance_bytecode(style, sizeof(hunit));

		vlength = *((css_fixed *) style->bytecode);
		advance_bytecode(style, sizeof(vlength));
		vunit = *((uint32_t *) style->bytecode);
		advance_bytecode(style, sizeof(vunit));
	}

	hunit = css__to_css_unit(hunit);
	vunit = css__to_css_unit(vunit);

	if (css__outranks_existing(getOpcode(opv), isImportant(opv), state,
			getFlagValue(opv))) {
		return set_border_spacing(state->computed, value,
				hlength, hunit, vlength, vunit);
	}

	return CSS_OK;
}

css_error css__set_border_spacing_from_hint(const css_hint *hint,
		css_computed_style *style)
{
	return set_border_spacing(style, hint->status,
		hint->data.position.h.value, hint->data.position.h.unit,
		hint->data.position.v.value, hint->data.position.v.unit);
}

css_error css__initial_border_spacing(css_select_state *state)
{
	return set_border_spacing(state->computed, CSS_BORDER_SPACING_SET,
			0, CSS_UNIT_PX, 0, CSS_UNIT_PX);
}

css_error css__copy_border_spacing(
		const css_computed_style *from,
		css_computed_style *to)
{
	css_fixed hlength = 0, vlength = 0;
	css_unit hunit = CSS_UNIT_PX, vunit = CSS_UNIT_PX;
	uint8_t type = get_border_spacing(from, &hlength, &hunit,
			&vlength, &vunit);

	if (from == to) {
		return CSS_OK;
	}

	return set_border_spacing(to, type, hlength, hunit, vlength, vunit);
}

css_error css__compose_border_spacing(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed hlength = 0, vlength = 0;
	css_unit hunit = CSS_UNIT_PX, vunit = CSS_UNIT_PX;
	uint8_t type = get_border_spacing(child, &hlength, &hunit,
			&vlength, &vunit);

	return css__copy_border_spacing(
			type == CSS_BORDER_SPACING_INHERIT ? parent : child,
			result);
}
