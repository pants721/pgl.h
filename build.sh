#!/bin/sh

set -xe

CC="${CXX:-cc}"
PKGS="sdl2"
CFLAGS="-Wall -Wextra -std=c2x -pedantic -ggdb -O3"
LIBS=-lm
SRC="demo.c"
OUT="out"

if [ `uname` = "Darwin" ]; then
    CFLAGS+=" -framework OpenGL"
fi

$CC $CFLAGS `pkg-config --cflags $PKGS` -o $OUT $SRC $LIBS `pkg-config --libs $PKGS`
