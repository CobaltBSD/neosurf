#!/usr/bin/env sh

# All this does currently is create a basic file structure for AppImages

TMPDIR_GTK=$(mktemp -dp "$PWD" tmp.gtk.XXXXXX)
TMPDIR_VI=$(mktemp -dp "$PWD" tmp.vi.XXXXXX)

cp -v neosurf-gtk.desktop "$TMPDIR_GTK"
cp -v neosurf-vi.desktop "$TMPDIR_VI"

cp -v AppRun-gtk "$TMPDIR_GTK/AppRun"
cp -v AppRun-vi "$TMPDIR_VI/AppRun"

cp -v neosurf.png "$TMPDIR_GTK"
cp -v neosurf.png "$TMPDIR_VI"

#TODO bundle libpsl, openssl
