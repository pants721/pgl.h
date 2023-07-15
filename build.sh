#!/bin/sh

set -xe

CC="${CXX:-cc}"
PKGS="sdl2"
CFLAGS="-Wall -Wextra -std=c2x -pedantic -ggdb -O3"
LIBS=-lm
SRC="demo.c"

if [ `uname` = "Darwin" ]; then
    CFLAGS+=" -framework OpenGL"
fi

$CC $CFLAGS `pkg-config --cflags $PKGS` -o pgl $SRC $LIBS `pkg-config --libs $PKGS`
