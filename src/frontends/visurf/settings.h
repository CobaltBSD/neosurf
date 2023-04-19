#ifndef NETSURF_VI_SETTINGS
#define NETSURF_VI_SETTINGS
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

struct nsvi_config {
	struct {
		uint32_t color;
	} caret;

	char *font;

	struct {
		char *alphabet;
		uint32_t bg;
		struct {
			uint32_t color;
			uint32_t width;
		} border;
		uint32_t fg;
		uint32_t like;
	} hints;

	char *home;

	uint32_t margin;

	struct {
		struct {
			uint32_t bg;
			uint32_t fg;
		} exline;
		struct {
			uint32_t bg;
			uint32_t fg;
		} follow;
		struct {
			uint32_t bg;
			uint32_t fg;
		} insert;
		struct {
			uint32_t bg;
			uint32_t fg;
		} normal;
		struct {
			uint32_t bg;
			uint32_t fg;
		} error;
	} status;

	struct {
		struct {
			uint32_t bg;
			uint32_t fg;
		} selected;
		struct {
			uint32_t loaded;
			uint32_t loading;
			uint32_t width;
		} throbber;
		struct {
			uint32_t bg;
			uint32_t fg;
		} unselected;
	} tabs;
};

extern struct nsvi_config config;

void nsvi_config_init(void);
void nsvi_config_finish(void);
bool nsvi_config_set(const char *name, const char *value);
#endif
