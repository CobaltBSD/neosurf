#!/usr/bin/env sh
#TODO this will be integrated into cmake later

# All this does currently is create a basic file structure for AppImages

TMPDIR_GTK=$(mktemp -dp "$PWD" tmp.gtk.XXXXXX)
TMPDIR_VI=$(mktemp -dp "$PWD" tmp.vi.XXXXXX)

cp -v neosurf-gtk.desktop "$TMPDIR_GTK"
cp -v neosurf-vi.desktop "$TMPDIR_VI"

cp -v AppRun-gtk "$TMPDIR_GTK/AppRun"
cp -v AppRun-vi "$TMPDIR_VI/AppRun"

cp -v neosurf.xpm "$TMPDIR_GTK"
cp -v neosurf.xpm "$TMPDIR_VI"
