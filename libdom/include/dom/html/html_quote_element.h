/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 Bo Yang <struggleyb.nku@gmail.com>
 * Copyright 2014 Rupinder Singh Khokhar <rsk1coder99@gmail.com>
 */
#ifndef dom_html_quote_element_h_
#define dom_html_quote_element_h_

#include <stdbool.h>
#include <dom/core/exceptions.h>
#include <dom/core/string.h>

typedef struct dom_html_quote_element dom_html_quote_element;

dom_exception dom_html_quote_element_get_cite(
	dom_html_quote_element *element, dom_string **cite);

dom_exception dom_html_quote_element_set_cite(
	dom_html_quote_element *element, dom_string *cite);

#endif
