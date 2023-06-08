// MIT License
// 
// Copyright (c) 2018 emersion
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include <assert.h>
#include <stdio.h>
#include "visurf/getopt.h"

char *ns_optarg = NULL;
int ns_optind = 1;
int ns_opterr = 1;
int ns_optopt = 0;
int ns_optpos = 1;

// POSIX getopt cannot be reset, so we need to provide our own implementation.
int ns_getopt(int argc, char *const argv[], const char *optstring) {
	assert(argv[argc] == NULL);
	ns_optarg = NULL;

	if (ns_optind == 0) {
		ns_optind = 1;
		ns_optpos = 1;
	}

	if (ns_optind >= argc) {
		return -1;
	}

	if (argv[ns_optind][0] != '-') {
		return -1;
	}

	if (argv[ns_optind][1] == '\0') {
		return -1;
	}

	if (argv[ns_optind][1] == '-') {
		ns_optind++;
		return -1;
	}

	const char *c = optstring;
	if (*c == ':') {
		c++;
	}

	ns_optopt = 0;
	int opt = argv[ns_optind][ns_optpos];
	for (; *c != '\0'; c++) {
		if (*c != opt) {
			continue;
		}

		if (c[1] != ':') {
			if (argv[ns_optind][ns_optpos + 1] == '\0') {
				ns_optind++;
				ns_optpos = 1;
			} else {
				ns_optpos++;
			}
			return opt;
		}

		if (argv[ns_optind][ns_optpos + 1] != '\0') {
			ns_optarg = &argv[ns_optind][ns_optpos + 1];
		} else {
			if (ns_optind + 2 > argc) {
				ns_optopt = opt;
				if (ns_opterr != 0 && optstring[0] != ':') {
					fprintf(stderr, "%s: Option '%c' requires an argument.\n",
						argv[0], ns_optopt);
				}

				return optstring[0] == ':' ? ':' : '?';
			}

			ns_optarg = argv[++ns_optind];
		}

		ns_optind++;
		return opt;
	}

	if (ns_opterr != 0 && optstring[0] != ':') {
		fprintf(stderr, "%s: Option '%c' not found.\n", argv[0], opt);
	}

	return '?';
}
