#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "utils/ascii.h"
#include "utils/errors.h"
#include "utils/file.h"
#include "utils/filepath.h"
#include "utils/hashtable.h"
#include "utils/log.h"
#include "utils/nsurl.h"
#include "neosurf/fetch.h"
#include "visurf/fetch.h"

extern char **respaths;

#define HASH_SIZE 117
#define MAX_LINE_LEN 256

static struct hash_table *mime_hash = NULL;

void
nsvi_fetch_filetype_init(const char *mimefile)
{
	struct stat statbuf;
	FILE *fh = NULL;

	mime_hash = hash_create(HASH_SIZE);

	/* Some OSes (mentioning no Solarises) have a worthlessly tiny
	 * /etc/mime.types that don't include essential things, so we
	 * pre-seed our hash with the essentials.  These will get
	 * over-ridden if they are mentioned in the mime.types file.
	 */

	hash_add(mime_hash, "css", "text/css");
	hash_add(mime_hash, "htm", "text/html");
	hash_add(mime_hash, "html", "text/html");
	hash_add(mime_hash, "jpg", "image/jpeg");
	hash_add(mime_hash, "jpeg", "image/jpeg");
	hash_add(mime_hash, "gif", "image/gif");
	hash_add(mime_hash, "png", "image/png");
	hash_add(mime_hash, "jng", "image/jng");
	hash_add(mime_hash, "mng", "image/mng");
	hash_add(mime_hash, "webp", "image/webp");
	hash_add(mime_hash, "spr", "image/x-riscos-sprite");
	hash_add(mime_hash, "bmp", "image/bmp");

	/* first, check to see if /etc/mime.types in preference */
	if ((stat("/etc/mime.types", &statbuf) == 0) &&
	    S_ISREG(statbuf.st_mode)) {
		mimefile = "/etc/mime.types";
	}

	fh = fopen(mimefile, "r");
	if (fh == NULL) {
		NSLOG(neosurf, INFO,
		      "Unable to open a mime.types file, so using a minimal one for you.");
		return;
	}

	while (feof(fh) == 0) {
		char line[MAX_LINE_LEN], *ptr, *type, *ext;

		if (fgets(line, sizeof(line), fh) == NULL)
			break;

		if ((feof(fh) == 0) && line[0] != '#') {
			ptr = line;

			/* search for the first non-whitespace character */
			while (ascii_is_space(*ptr)) {
				ptr++;
			}

			/* is this line empty other than leading whitespace? */
			if (*ptr == '\n' || *ptr == '\0') {
				continue;
			}

			type = ptr;

			/* search for the first whitespace char or NUL or
			 * NL */
			while (*ptr && (!ascii_is_space(*ptr))) {
				ptr++;
			}

			if (*ptr == '\0' || *ptr == '\n') {
				/* this mimetype has no extensions - read next
				 * line.
				 */
				continue;
			}

			*ptr++ = '\0';

			/* search for the first non-whitespace character which
			 * will be the first filename extenion */
			while (ascii_is_space(*ptr)) {
				ptr++;
			}

			while (true) {
				ext = ptr;

				/* search for the first whitespace char or
				 * NUL or NL which is the end of the ext.
				 */
				while (*ptr && (!ascii_is_space(*ptr))) {
					ptr++;
				}

				if (*ptr == '\0' || *ptr == '\n') {
					/* special case for last extension on
					 * the line
					 */
					*ptr = '\0';
					hash_add(mime_hash, ext, type);
					break;
				}

				*ptr++ = '\0';
				hash_add(mime_hash, ext, type);

				/* search for the first non-whitespace char or
				 * NUL or NL, to find start of next ext.
				 */
				while (*ptr &&
				       (ascii_is_space(*ptr)) &&
				       *ptr != '\n') {
					ptr++;
				}
			}
		}
	}

	fclose(fh);
}

void
nsvi_fetch_filetype_fini(void)
{
	hash_destroy(mime_hash);
}

static const char *
nsvi_fetch_filetype(const char *unix_path)
{
	struct stat statbuf;
	char *ext;
	const char *ptr;
	char *lowerchar;
	const char *type;
	int l;

	/* stat the path to attempt to determine if the file is special */
	if (stat(unix_path, &statbuf) == 0) {
		/* stat suceeded so can check for directory */

		if (S_ISDIR(statbuf.st_mode)) {
			return "application/x-neosurf-directory";
		}
	}

	l = strlen(unix_path);

	/* Hacky RISC OS compatibility */
	if ((3 < l) && (strcasecmp(unix_path + l - 4, ",f79") == 0)) {
		return "text/css";
	} else if ((3 < l) && (strcasecmp(unix_path + l - 4, ",faf") == 0)) {
		return "text/html";
	} else if ((3 < l) && (strcasecmp(unix_path + l - 4, ",b60") == 0)) {
		return "image/png";
	} else if ((3 < l) && (strcasecmp(unix_path + l - 4, ",ff9") == 0)) {
		return "image/x-riscos-sprite";
	}

	if (strchr(unix_path, '.') == NULL) {
		/* no extension anywhere! */
		return "text/plain";
	}

	ptr = unix_path + strlen(unix_path);
	while (*ptr != '.' && *ptr != '/') {
		ptr--;
	}

	if (*ptr != '.') {
		return "text/plain";
	}

	ext = strdup(ptr + 1);	/* skip the . */

	/* the hash table only contains lower-case versions - make sure this
	 * copy is lower case too.
	 */
	lowerchar = ext;
	while (*lowerchar) {
		*lowerchar = ascii_to_lower(*lowerchar);
		lowerchar++;
	}

	type = hash_get(mime_hash, ext);
	free(ext);

	if (type == NULL) {
		type = "text/plain";
	}

	return type;
}

static nsurl *
nsvi_fetch_get_resource_url(const char *path)
{
	char buf[PATH_MAX];
	nsurl *url = NULL;
	neosurf_path_to_nsurl(filepath_sfind(respaths, buf, path), &url);
	return url;
}

struct gui_fetch_table vi_fetch_table = {
	.filetype = nsvi_fetch_filetype,
	.get_resource_url = nsvi_fetch_get_resource_url,
};
