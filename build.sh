#!/bin/bash

# Dependencies: gcc, clang-tidy (optional), cppcheck (optional)
# Gnu extensions are currently being used

# TODO : Learn how to implement unit tests and fuzzers into the build process

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

# Run clang-tidy (LLVM)
if command -v clang-tidy &> /dev/null; then
    echo "Running clang-tidy..."
    clang-tidy $SRC -- -std=gnu17 $INCLUDE
fi

# Run cppcheck
if command -v cppcheck &> /dev/null; then
    echo "Running cppcheck..."
    cppcheck \
        --enable=warning,style,performance,portability \
        --std=c11 --language=c --force \
        -Iinclude \
        --suppress=missingIncludeSystem \
        --quiet \
        $SRC
fi