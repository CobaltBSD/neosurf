CFLAGS += -DNDEBUG -DWITH_OPENSSL -DWITH_BMP -DWITH_GIF -DWITH_NS_SVG -D_XOPEN_SOURCE=700 -D_DEFAULT_SOURCE -DNEOSURF_VERSION=16
CFLAGS += -std=c99 -Wno-unused-parameter -Wno-deprecated-declarations -Wno-strict-prototypes -Wno-unused-but-set-variable -Wno-int-conversion

GPERF = gperf
PYTHON3 = python3
PKG_CONFIG = pkgconf
M4 = m4

all: libcss
	@echo TEST $(CFLAGS) ABC

libcss:
	$(MAKE) -C contrib/libcss

clean:
	$(MAKE) -C contrib/libcss clean
