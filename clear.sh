#!/bin/bash

make clean

rm -rf third_party/build/ \
    kylin-music.pro.user \
    debian/files \
    .qmake.stash \
    Makefile \
    debian/.debhelper/ \
    debian/debhelper-build-stamp \
    debian/kylin-music.debhelper.log \
    debian/kylin-music.substvars \
    debian/kylin-music/ \
    kylin-music
