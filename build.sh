#!/bin/bash

# This script is a “do-your-best” guardrail.

# Dependencies: gcc, clang-tidy (optional), cppcheck (optional)
# Gnu extensions are currently being used

# TODO(malac0da): Learn how to implement unit tests and fuzzers into the build process

# -- Flags --
set -xe

MODE=${1:-DEBUG}

CC=cc
INCLUDE="-Iinclude/"
COMMON_FLAGS="-Wall -Wextra -Wshadow -Wconversion -Wundef -Werror -pedantic -fanalyzer"
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

# --- Compile step ---
$CC $INCLUDE $CFLAGS -o $OUT $SRC

# --- Static analysis section ---
# Run cppcheck if installed
if command -v cppcheck &> /dev/null; then
    echo "Running cppcheck..."
    cppcheck \
        --enable=warning,style,performance,portability \
        --check-level=exhaustive \
        --inconclusive \
        --std=gnu17 --language=c --force \
        -Iinclude \
        $SRC
fi

# -- Formatting --
#if command -v clang-format &> /dev/null; then
#    echo "Checking formatting..."
#    clang-format --dry-run --Werror *.c include/*.h
#fi