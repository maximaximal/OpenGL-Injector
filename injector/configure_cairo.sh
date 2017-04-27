#!/usr/bin/env sh

# This script correctly configures cairo to use the static variant of pixman.

source_dir=$1;
prefix_dir=$2;

export pixman_CFLAGS=-I$2/include/pixman-1
export pixman_LIBS="-L$2/libs -Wl -Bstatic $2/lib/libpixman-1.a"

$1/configure --enable-static --enable-shared CFLAGS=-g LDLAGS=-static --prefix=$2
