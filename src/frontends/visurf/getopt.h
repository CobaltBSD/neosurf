#ifndef NETSURF_GETOPT_H
#define NETSURF_GETOPT_H

extern char *ns_optarg;
extern int ns_opterr, ns_optind, ns_optopt;

int ns_getopt(int argc, char * const argv[], const char *optstring);

#endif
