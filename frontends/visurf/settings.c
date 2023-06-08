#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "visurf/settings.h"
#include "visurf/visurf.h"

struct nsvi_config config = {
	.caret = {
		.color = 0x000000FF,
	},

	// strdup()'d in nsvi_config_init
	.font = (char *)"monospace 10",

	.hints = {
		.alphabet = (char *)"asdfjkl",
		.bg = 0xEE2222FF,
		.border = {
			.color = 0xFFFFFFFF,
			.width = 0,
		},
		.fg = 0xFFFFFFFF,
		.like = 0x000000FF,
	},

	.home = (char *)"about:welcome",

	.margin = 3,

	.status = {
		.exline = {
			.fg = 0xFFFFFFFF,
			.bg = 0x222222FF,
		},
		.follow = {
			.fg = 0xFFFFFFFF,
			.bg = 0x0087BDFF,
		},
		.insert = {
			.fg = 0xFFFFFFFF,
			.bg = 0x008800FF,
		},
		.normal = {
			.fg = 0xFFFFFFFF,
			.bg = 0x111111FF,
		},
		.error = {
			.fg = 0xFFFFFFFF,
			.bg = 0xFF0000FF,
		},
	},

	.tabs = {
		.selected = {
			.fg = 0xFFFFFFFF,
			.bg = 0x223366FF,
		},
		.throbber =  {
			.loaded = 0x008000FF,
			.loading = 0x8888FFFF,
			.width = 5,
		},
		.unselected = {
			.fg = 0xFFFFFFFF,
			.bg = 0x222222FF,
		},
	},
};

enum nsvi_config_kind {
	CONFIG_COLOR,
	CONFIG_STRING,
	CONFIG_URLFILTER,
	CONFIG_UINT,
};

struct nsvi_config_info {
	const char *name;
	enum nsvi_config_kind kind;
	union {
		uint32_t *color;
		uint32_t *uint;
		char **string;
	};
};

// Alpha-sorted by name
static struct nsvi_config_info config_info[] = {
	{
		.name = "caret.color",
		.kind = CONFIG_COLOR,
		.color = &config.caret.color,
	},
	{
		.name = "font",
		.kind = CONFIG_STRING,
		.string = &config.font,
	},
	{
		.name = "hints.alphabet",
		.kind = CONFIG_STRING,
		.string = &config.hints.alphabet,
	},
	{
		.name = "hints.bg",
		.kind = CONFIG_COLOR,
		.color = &config.hints.bg,
	},
	{
		.name = "hints.border.color",
		.kind = CONFIG_COLOR,
		.color = &config.hints.border.color,
	},
	{
		.name = "hints.border.width",
		.kind = CONFIG_UINT,
		.uint = &config.hints.border.width,
	},
	{
		.name = "hints.fg",
		.kind = CONFIG_COLOR,
		.color = &config.hints.fg,
	},
	{
		.name = "hints.like",
		.kind = CONFIG_COLOR,
		.color = &config.hints.like,
	},
	{
		.name = "home",
		.kind = CONFIG_STRING,
		.string = &config.home,
	},
	{
		.name = "margin",
		.kind = CONFIG_UINT,
		.uint = &config.margin,
	},
	{
		.name = "status.exline.bg",
		.kind = CONFIG_COLOR,
		.color = &config.status.exline.bg,
	},
	{
		.name = "status.exline.fg",
		.kind = CONFIG_COLOR,
		.color = &config.status.exline.fg,
	},
	{
		.name = "status.follow.bg",
		.kind = CONFIG_COLOR,
		.color = &config.status.follow.bg,
	},
	{
		.name = "status.follow.fg",
		.kind = CONFIG_COLOR,
		.color = &config.status.follow.fg,
	},
	{
		.name = "status.insert.bg",
		.kind = CONFIG_COLOR,
		.color = &config.status.insert.bg,
	},
	{
		.name = "status.insert.fg",
		.kind = CONFIG_COLOR,
		.color = &config.status.insert.fg,
	},
	{
		.name = "status.normal.bg",
		.kind = CONFIG_COLOR,
		.color = &config.status.normal.bg,
	},
	{
		.name = "status.normal.fg",
		.kind = CONFIG_COLOR,
		.color = &config.status.normal.fg,
	},
	{
		.name = "tabs.selected.bg",
		.kind = CONFIG_COLOR,
		.color = &config.tabs.selected.bg,
	},
	{
		.name = "tabs.selected.fg",
		.kind = CONFIG_COLOR,
		.color = &config.tabs.selected.fg,
	},
	{
		.name = "tabs.throbber.loaded",
		.kind = CONFIG_COLOR,
		.color = &config.tabs.throbber.loaded,
	},
	{
		.name = "tabs.throbber.loading",
		.kind = CONFIG_COLOR,
		.color = &config.tabs.throbber.loading,
	},
	{
		.name = "tabs.throbber.width",
		.kind = CONFIG_UINT,
		.uint = &config.tabs.throbber.width,
	},
	{
		.name = "tabs.unselected.bg",
		.kind = CONFIG_COLOR,
		.color = &config.tabs.unselected.bg,
	},
	{
		.name = "tabs.unselected.fg",
		.kind = CONFIG_COLOR,
		.color = &config.tabs.unselected.fg,
	},
	{
		.name = "urlfilter",
		.kind = CONFIG_URLFILTER,
	},
};

void
nsvi_config_init(void)
{
	for (size_t i = 0; i < sizeof(config_info) / sizeof(config_info[0]); i++) {
		switch (config_info[i].kind) {
		case CONFIG_COLOR:
		case CONFIG_UINT:
		case CONFIG_URLFILTER:
			break;
		case CONFIG_STRING:
			*config_info[i].string = strdup(*config_info[i].string);
			break;
		}
	}
	nsvi_config_set("urlfilter", "urlfilter");
}

void
nsvi_config_finish(void)
{
	for (size_t i = 0; i < sizeof(config_info) / sizeof(config_info[0]); i++) {
		switch (config_info[i].kind) {
		case CONFIG_COLOR:
		case CONFIG_UINT:
		case CONFIG_URLFILTER:
			break;
		case CONFIG_STRING:
			free(*config_info[i].string);
			break;
		}
	}
}

static int
config_compar(const void *a, const void *b)
{
	const char *key = a;
	const struct nsvi_config_info *info = b;
	if (!key) {
		return -1;
	}
	return strcmp(key, info->name);
}

static bool
nsvi_set_filter(struct nsvi_filter *filter, const char *command)
{
	return false; // Filters are not currently used. XXX
	/*int stdinpipe[2];
	int stdoutpipe[2];
	int errpipe[2];
	int r = pipe(stdinpipe);
	assert(r == 0);
	r = pipe(stdoutpipe);
	assert(r == 0);
	r = pipe(errpipe);
	assert(r == 0);

	pid_t pid = fork();
	assert(pid != -1);
	if (pid == 0) {
		dup2(stdinpipe[0], STDIN_FILENO);
		dup2(stdoutpipe[1], STDOUT_FILENO);
		close(stdinpipe[0]);
		close(stdinpipe[1]);
		close(stdoutpipe[0]);
		close(stdoutpipe[1]);
		close(errpipe[0]);
		fcntl(errpipe[1], F_SETFD, FD_CLOEXEC);
		execlp(command, command, (char *)NULL);
		write(errpipe[1], &errno, sizeof(int));
		_exit(1);
	}
	close(errpipe[1]);
	int err;
	if (read(errpipe[0], &err, sizeof(int)) == sizeof(int)) {
		fprintf(stderr, "starting urlfilter failed: %s\n",
				strerror(err));
		return false;
	}
	close(errpipe[0]);
	if (filter->in != NULL) {
		fclose(filter->in);
		fclose(filter->out);
		kill(filter->pid, SIGTERM);
		waitpid(filter->pid, NULL, 0);
	}
	filter->pid = pid;
	filter->in = fdopen(stdinpipe[1], "w");
	filter->out = fdopen(stdoutpipe[0], "r");
	setvbuf(filter->in, NULL, _IOLBF, 0);
	return true;*/
}

bool
nsvi_config_set(const char *name, const char *value)
{
	struct nsvi_config_info *info = bsearch(name, &config_info,
		sizeof(config_info) / sizeof(config_info[0]),
		sizeof(config_info[0]), config_compar);
	if (info == NULL) {
		return false;
	}
	char *endptr;
	unsigned long color;
	unsigned long uint;
	switch (info->kind) {
	case CONFIG_COLOR:
		color = strtoul(value, &endptr, 16);
		if (endptr == NULL || *endptr != '\0') {
			return false;
		}
		*info->color = color;
		break;
	case CONFIG_STRING:
		free(*info->string);
		*info->string = strdup(value);
		break;
	case CONFIG_URLFILTER:
		return nsvi_set_filter(&global_state->filter, value);
	case CONFIG_UINT:
		uint = strtoul(value, &endptr, 10);
		if (endptr == NULL || *endptr != '\0') {
			return false;
		}
		*info->uint = uint;
	}
	return true;
}
