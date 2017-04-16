#!/usr/bin/env sh

# This script correctly configures cairo to use the static variant of pixman.
# And adds -fPIC to CC.

source_dir=$1;
prefix_dir=$2;

export CFLAGS="$CFLAGS -fPIC"
$1/configure --enable-static LDLAGS=-static --prefix=$2
