/*
 * This file is part of LibParserUtils.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <iconv.h>

#include <parserutils/charset/mibenum.h>
#include <parserutils/charset/codec.h>

#include "input/filter.h"
#include "utils/utils.h"

/** Input filter */
struct parserutils_filter {
	iconv_t cd;			/**< Iconv conversion descriptor */
	uint16_t int_enc;		/**< The internal encoding */

	struct {
		uint16_t encoding;	/**< Input encoding */
	} settings;			/**< Filter settings */
};

static parserutils_error filter_set_defaults(parserutils_filter *input);
static parserutils_error filter_set_encoding(parserutils_filter *input,
		const char *enc);

/**
 * Create an input filter
 *
 * \param int_enc  Desired encoding of document
 * \param filter   Pointer to location to receive filter instance
 * \return PARSERUTILS_OK on success,
 *         PARSERUTILS_BADPARM on bad parameters,
 *         PARSERUTILS_NOMEM on memory exhausion,
 *         PARSERUTILS_BADENCODING if the encoding is unsupported
 */
parserutils_error parserutils__filter_create(const char *int_enc,
		parserutils_filter **filter)
{
	parserutils_filter *f;
	parserutils_error error;

	if (int_enc == NULL || filter == NULL)
		return PARSERUTILS_BADPARM;

	f = malloc(sizeof(parserutils_filter));
	if (f == NULL)
		return PARSERUTILS_NOMEM;

	f->cd = (iconv_t) -1;
	f->int_enc = parserutils_charset_mibenum_from_name(
			int_enc, strlen(int_enc));
	if (f->int_enc == 0) {
		free(f);
		return PARSERUTILS_BADENCODING;
	}

	error = filter_set_defaults(f);
	if (error != PARSERUTILS_OK) {
		free(f);
		return error;
	}

	*filter = f;

	return PARSERUTILS_OK;
}

/**
 * Destroy an input filter
 *
 * \param input  Pointer to filter instance
 * \return PARSERUTILS_OK on success, appropriate error otherwise
 */
parserutils_error parserutils__filter_destroy(parserutils_filter *input)
{
	if (input == NULL)
		return PARSERUTILS_BADPARM;

	if (input->cd != (iconv_t) -1) {
		iconv_close(input->cd);
		input->cd = (iconv_t) -1;
	}

	free(input);

	return PARSERUTILS_OK;
}

/**
 * Configure an input filter
 *
 * \param input   Pointer to filter instance
 * \param type    Input option type to configure
 * \param params  Option-specific parameters
 * \return PARSERUTILS_OK on success, appropriate error otherwise
 */
parserutils_error parserutils__filter_setopt(parserutils_filter *input,
		parserutils_filter_opttype type,
		parserutils_filter_optparams *params)
{
	parserutils_error error = PARSERUTILS_OK;

	if (input == NULL || params == NULL)
		return PARSERUTILS_BADPARM;

	switch (type) {
	case PARSERUTILS_FILTER_SET_ENCODING:
		error = filter_set_encoding(input, params->encoding.name);
		break;
	}

	return error;
}

/**
 * Process a chunk of data
 *
 * \param input   Pointer to filter instance
 * \param data    Pointer to pointer to input buffer
 * \param len     Pointer to length of input buffer
 * \param output  Pointer to pointer to output buffer
 * \param outlen  Pointer to length of output buffer
 * \return PARSERUTILS_OK on success, appropriate error otherwise
 *
 * Call this with an input buffer length of 0 to flush any buffers.
 */
parserutils_error parserutils__filter_process_chunk(parserutils_filter *input,
		const uint8_t **data, size_t *len,
		uint8_t **output, size_t *outlen)
{
	if (input == NULL || data == NULL || *data == NULL || len == NULL ||
			output == NULL || *output == NULL || outlen == NULL)
		return PARSERUTILS_BADPARM;

	if (iconv(input->cd, (void *) data, len, 
			(char **) output, outlen) == (size_t) -1) {
		switch (errno) {
		case E2BIG:
			return PARSERUTILS_NOMEM;
		case EILSEQ:
			if (*outlen < 3)
				return PARSERUTILS_NOMEM;

			(*output)[0] = 0xef;
			(*output)[1] = 0xbf;
			(*output)[2] = 0xbd;

			*output += 3;
			*outlen -= 3;

			(*data)++;
			(*len)--;

			while (*len > 0) {
				size_t ret;
				
				ret = iconv(input->cd, (void *) data, len, 
						(char **) output, outlen);
				if (ret != (size_t) -1 || errno != EILSEQ)
					break;

				if (*outlen < 3)
					return PARSERUTILS_NOMEM;

				(*output)[0] = 0xef;
				(*output)[1] = 0xbf;
				(*output)[2] = 0xbd;

				*output += 3;
				*outlen -= 3;

				(*data)++;
				(*len)--;
			}

			return errno == E2BIG ? PARSERUTILS_NOMEM 
					      : PARSERUTILS_OK;
		}
	}

	return PARSERUTILS_OK;
}

/**
 * Reset an input filter's state
 *
 * \param input  The input filter to reset
 * \return PARSERUTILS_OK on success, appropriate error otherwise
 */
parserutils_error parserutils__filter_reset(parserutils_filter *input)
{
	parserutils_error error = PARSERUTILS_OK;

	if (input == NULL)
		return PARSERUTILS_BADPARM;

	iconv(input->cd, NULL, 0, NULL, 0);

	return error;
}

/**
 * Set an input filter's default settings
 *
 * \param input  Input filter to configure
 * \return PARSERUTILS_OK on success, appropriate error otherwise
 */
parserutils_error filter_set_defaults(parserutils_filter *input)
{
	parserutils_error error;

	if (input == NULL)
		return PARSERUTILS_BADPARM;

	input->settings.encoding = 0;
	error = filter_set_encoding(input, "UTF-8");
	if (error != PARSERUTILS_OK)
		return error;

	return PARSERUTILS_OK;
}

/**
 * Set an input filter's encoding
 *
 * \param input  Input filter to configure
 * \param enc    Encoding name
 * \return PARSERUTILS_OK on success, appropriate error otherwise
 */
parserutils_error filter_set_encoding(parserutils_filter *input,
		const char *enc)
{
	parserutils_error error = PARSERUTILS_OK;
	uint16_t mibenum;

	if (input == NULL || enc == NULL)
		return PARSERUTILS_BADPARM;

	mibenum = parserutils_charset_mibenum_from_name(enc, strlen(enc));
	if (mibenum == 0)
		return PARSERUTILS_BADENCODING;

	/* Exit early if we're already using this encoding */
	if (input->settings.encoding == mibenum)
		return PARSERUTILS_OK;

	if (input->cd != (iconv_t) -1) {
		iconv_close(input->cd);
		input->cd = (iconv_t) -1;
	}

	input->cd = iconv_open(
		parserutils_charset_mibenum_to_name(input->int_enc),
		parserutils_charset_mibenum_to_name(mibenum));
	if (input->cd == (iconv_t) -1) {
		return (errno == EINVAL) ? PARSERUTILS_BADENCODING
					 : PARSERUTILS_NOMEM;
	}

	input->settings.encoding = mibenum;

	return error;

}
