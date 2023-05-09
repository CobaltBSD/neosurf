/*
 * Copyright 2014 Chris Young <chris@unsatisfactorysoftware.co.uk>
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

/**
 * \file
 * NetSurf international domain name handling implementation.
 */

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "neosurf/inttypes.h"

#include "utils/errors.h"
#include "utils/idna.h"
#include "utils/idna_props.h"
#include "utils/log.h"
#include "utils/punycode.h"
#include "utils/utf8.h"
#include "utils/utils.h"


/**
 * Convert punycode status into nserror.
 *
 * \param status The punycode status to convert.
 * \return The corresponding nserror code for the status.
 */
static nserror punycode_status_to_nserror(enum punycode_status status)
{
	nserror ret = NSERROR_NOMEM;

	switch (status) {
	case punycode_success:
		ret = NSERROR_OK;
		break;

	case punycode_bad_input:
		NSLOG(neosurf, INFO, "Bad input");
		ret = NSERROR_BAD_ENCODING;
		break;

	case punycode_big_output:
		NSLOG(neosurf, INFO, "Output too big");
		ret = NSERROR_BAD_SIZE;
		break;

	case punycode_overflow:
		NSLOG(neosurf, INFO, "Overflow");
		ret = NSERROR_NOSPACE;
		break;

	default:
		break;
	}
	return ret;
}


/**
 * Convert a host label in UCS-4 to an ACE version
 *
 * \param ucs4_label UCS-4 NFC string containing host label
 * \param len Length of host label (in characters/codepoints)
 * \param ace_label ASCII-compatible encoded version
 * \param out_len Length of ace_label
 * \return NSERROR_OK on success, appropriate error otherwise
 *
 * If return value != NSERROR_OK, output will be left untouched.
 */
static nserror
idna__ucs4_to_ace(int32_t *ucs4_label,
		  size_t len,
		  char **ace_label,
		  size_t *out_len)
{
	char punycode[65]; /* max length of host label + NULL */
	size_t output_length = 60; /* punycode length - 4 - 1 */
	nserror ret;

	punycode[0] = 'x';
	punycode[1] = 'n';
	punycode[2] = '-';
	punycode[3] = '-';

	ret = punycode_status_to_nserror(punycode_encode(len,
			(const punycode_uint *)ucs4_label, NULL,
			&output_length, punycode + 4));
	if (ret != NSERROR_OK) {
		return ret;
	}

	output_length += SLEN("xn--");
	punycode[output_length] = '\0';

	*ace_label = strdup(punycode);
	*out_len = output_length;

	return NSERROR_OK;
}


/**
 * Convert a host label in ACE format to UCS-4
 *
 * \param ace_label ASCII string containing host label
 * \param ace_len Length of host label
 * \param ucs4_label Pointer to hold UCS4 decoded version
 * \param ucs4_len Pointer to hold length of ucs4_label
 * \return NSERROR_OK on success, appropriate error otherwise
 *
 * If return value != NSERROR_OK, output will be left untouched.
 */
static nserror
idna__ace_to_ucs4(const char *ace_label,
		  size_t ace_len,
		  int32_t **ucs4_label,
		  size_t *ucs4_len)
{
	int32_t *ucs4;
	nserror ret;
	size_t output_length = ace_len; /* never exceeds input length */

	/* The header should always have been checked before calling */
	assert((ace_label[0] == 'x') && (ace_label[1] == 'n') &&
		(ace_label[2] == '-') && (ace_label[3] == '-'));

	ucs4 = malloc(output_length * 4);
	if (ucs4 == NULL) {
		return NSERROR_NOMEM;
	}

	ret = punycode_status_to_nserror(punycode_decode(ace_len - 4,
		ace_label + 4, &output_length, (punycode_uint *)ucs4, NULL));
	if (ret != NSERROR_OK) {
		free(ucs4);
		return ret;
	}

	ucs4[output_length] = '\0';

	*ucs4_label = ucs4;
	*ucs4_len = output_length;

	return NSERROR_OK;
}


/**
 * Convert a UTF-8 string to UCS-4
 *
 * \param utf8_label	UTF-8 string containing host label
 * \param len	Length of host label (in bytes)
 * \param ucs4_label	Pointer to update with the output
 * \param ucs4_len	Pointer to update with the length
 * \return NSERROR_OK on success, appropriate error otherwise
 *
 * If return value != NSERROR_OK, output will be left untouched.
 */
static nserror
idna__utf8_to_ucs4(const char *utf8_label,
		   size_t len,
		   int32_t **ucs4_label,
		   size_t *ucs4_len)
{
	return NSERROR_NOT_IMPLEMENTED;
}


/**
 * Convert a UCS-4 string to UTF-8
 *
 * \param ucs4_label	UCS-4 string containing host label
 * \param ucs4_len	Length of host label (in bytes)
 * \param utf8_label	Pointer to update with the output
 * \param utf8_len	Pointer to update with the length
 * \return NSERROR_OK on success, appropriate error otherwise
 *
 * If return value != NSERROR_OK, output will be left untouched.
 */
static nserror
idna__ucs4_to_utf8(const int32_t *ucs4_label,
		   size_t ucs4_len,
		   char **utf8_label,
		   size_t *utf8_len)
{
	return NSERROR_NOT_IMPLEMENTED;
}


/**
 * Check if a host label is valid for IDNA2008
 *
 * \param label	Host label to check (UCS-4)
 * \param len	Length of host label (in characters/codepoints)
 * \return true if compliant, false otherwise
 */
static bool idna__is_valid(int32_t *label, size_t len)
{
	return true;
}


/**
 * Verify an ACE label is valid
 *
 * \param label	Host label to check
 * \param len	Length of label
 * \return true if valid, false otherwise
 */
static bool idna__verify(const char *label, size_t len)
{
	return true;
}


/**
 * Find the length of a host label
 *
 * \param host	String containing a host or FQDN
 * \param max_length	Length of host string to search (in bytes)
 * \return Distance to next separator character or end of string
 */
static size_t idna__host_label_length(const char *host, size_t max_length)
{
	const char *p = host;
	size_t length = 0;

	while (length < max_length) {
		if ((*p == '.') || (*p == ':') || (*p == '\0')) {
			break;
		}
		length++;
		p++;
	}

	return length;
}


/**
 * Check if a host label is LDH
 *
 * \param label	Host label to check
 * \param len	Length of host label
 * \return true if LDH compliant, false otherwise
 */
static bool idna__is_ldh(const char *label, size_t len)
{
	const char *p = label;
	size_t i = 0;

	/* Check for leading or trailing hyphens */
	if ((p[0] == '-') || (p[len - 1] == '-'))
		return false;

	/* Check for non-alphanumeric, non-hyphen characters */
	for (i = 0; i < len; p++) {
		i++;
		if (*p == '-') continue;
		if ((*p >= '0') && (*p <= '9')) continue;
		if ((*p >= 'a') && (*p <= 'z')) continue;
		if ((*p >= 'A') && (*p <= 'Z')) continue;

		return false;
	}

	return true;
}


/**
 * Check if a host label appears to be ACE
 *
 * \param label	Host label to check
 * \param len	Length of host label
 * \return true if ACE compliant, false otherwise
 */
static bool idna__is_ace(const char *label, size_t len)
{
	/* Check it is a valid DNS string */
	if (idna__is_ldh(label, len) == false) {
		return false;
	}

	/* Check the ACE prefix is present */
	if ((label[0] == 'x') && (label[1] == 'n') &&
	    (label[2] == '-') && (label[3] == '-')) {
		return true;
	}

	return false;
}


/* exported interface documented in idna.h */
nserror
idna_encode(const char *host, size_t len, char **ace_host, size_t *ace_len)
{
	nserror error;
	int32_t *ucs4_host;
	size_t label_len, output_len, ucs4_len, fqdn_len = 0;
	char fqdn[256];
	char *output, *fqdn_p = fqdn;

	label_len = idna__host_label_length(host, len);
	if (label_len == 0) {
		return NSERROR_BAD_URL;
	}

	while (label_len != 0) {
		if (idna__is_ldh(host, label_len) == false) {
			/* This string is IDN or invalid */

			/* Convert to Unicode */
			error = idna__utf8_to_ucs4(host, label_len,
						   &ucs4_host, &ucs4_len);
			if (error != NSERROR_OK) {
				return error;
			}

			/* Check this is valid for conversion */
			if (idna__is_valid(ucs4_host, ucs4_len) == false) {
				free(ucs4_host);
				return NSERROR_BAD_URL;
			}

			/* Convert to ACE */
			error = idna__ucs4_to_ace(ucs4_host, ucs4_len,
						&output, &output_len);
			free(ucs4_host);
			if (error != NSERROR_OK) {
				return error;
			}
			strncpy(fqdn_p, output, output_len);
			free(output);
			fqdn_p += output_len;
			fqdn_len += output_len;
		} else {
			/* This is already a DNS-valid ASCII string */
			if ((idna__is_ace(host, label_len) == true) &&
			    (idna__verify(host, label_len) == false)) {
				NSLOG(neosurf, INFO,
				      "Cannot verify ACE label %s", host);
				return NSERROR_BAD_URL;
			}
			strncpy(fqdn_p, host, label_len);
			fqdn_p += label_len;
			fqdn_len += label_len;
		}

		*fqdn_p = '.';
		fqdn_p++;
		fqdn_len++;

		host += label_len;
		if ((*host == '\0') || (*host == ':')) {
			break;
		}
		host++;
		len = len - label_len - 1;

		label_len = idna__host_label_length(host, len);
	}

	fqdn_p--;
	*fqdn_p = '\0';
	*ace_host = strdup(fqdn);
	*ace_len = fqdn_len - 1; /* last character is NULL */

	return NSERROR_OK;
}


/* exported interface documented in idna.h */
nserror
idna_decode(const char *ace_host, size_t ace_len, char **host, size_t *host_len)
{
	nserror error;
	int32_t *ucs4_host;
	size_t label_len, output_len, ucs4_len, fqdn_len = 0;
	char fqdn[256];
	char *output, *fqdn_p = fqdn;

	label_len = idna__host_label_length(ace_host, ace_len);
	if (label_len == 0) {
		return NSERROR_BAD_URL;
	}

	while (label_len != 0) {
		if (idna__is_ace(ace_host, label_len) == true) {
			/* This string is DNS-valid and (probably) encoded */

			/* Decode to Unicode */
			error = idna__ace_to_ucs4(ace_host, label_len,
						  &ucs4_host, &ucs4_len);
			if (error != NSERROR_OK) {
				return error;
			}

			/* Convert to UTF-8 */
			error = idna__ucs4_to_utf8(ucs4_host, ucs4_len,
						   &output, &output_len);
			free(ucs4_host);
			if (error != NSERROR_OK) {
				return error;
			}

			memcpy(fqdn_p, output, output_len);
			free(output);
			fqdn_p += output_len;
			fqdn_len += output_len;
		} else {
			/* Not ACE */
			memcpy(fqdn_p, ace_host, label_len);
			fqdn_p += label_len;
			fqdn_len += label_len;
		}

		*fqdn_p = '.';
		fqdn_p++;
		fqdn_len++;

		ace_host += label_len;
		if ((*ace_host == '\0') || (*ace_host == ':')) {
			break;
		}
		ace_host++;
		ace_len = ace_len - label_len - 1;

		label_len = idna__host_label_length(ace_host, ace_len);
	}

	fqdn_p--;
	*fqdn_p = '\0';
	*host = strdup(fqdn);
	*host_len = fqdn_len - 1; /* last character is NULL */

	return NSERROR_OK;
}
