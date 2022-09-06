#!/usr/bin/env bash
# -* - coding: UTF-8 -* -

rm -f kylin-music.pro.user \
    debian/files

make clean 

rm -rf .qmake.stash \
	Makefile \
	debian/.debhelper/ \
	debian/debhelper-build-stamp \
	debian/kylin-music.debhelper.log \
	debian/kylin-music.substvars \
	debian/kylin-music/ \
	kylin-music \
	third_party/build/
