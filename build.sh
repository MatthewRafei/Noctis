#!/bin/bash

set -xe

MODE=${1:-DEBUG}

CC=cc
INCLUDE="-Iinclude/"
COMMON_FLAGS="-Wall -Wextra -Wshadow -Wconversion -Wundef -Werror -pedantic"
SANITIZE="-fsanitize=address,undefined"

SRC="*.c"
OUT="noctis"

if [ "$MODE" = "DEBUG" ]; then
    CFLAGS="$COMMON_FLAGS -std=gnu17 -g -O0 $SANITIZE"
elif [ "$MODE" = "RELEASE" ]; then
    CFLAGS="$COMMON_FLAGS -std=gnu17 -O2"
else
    echo "Unknown mode: $MODE"
    exit 1
fi

$CC $INCLUDE $CFLAGS -o $OUT $SRC