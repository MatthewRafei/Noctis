#!/bin/bash

# This script is a “do-your-best” guardrail.

# Dependencies: gcc, cppcheck (optional), indent (optional)

# TODO(malac0da): Learn how to implement unit tests and fuzzers into the build process

# -- Flags --
set -e

# -- Debug or Release mode --
# Usage: ./build.sh [STATIC|DEBUG|RELEASE]
# debug if no argument is given
MODE=${1:-STATIC}

# C99 and up
# Modification have been made to make this code base as portable as possible
CC=cc
INCLUDE="-Iinclude/"
COMMON_FLAGS="-Wall -Wextra -Wshadow -Wconversion -Wundef -Werror -pedantic -fanalyzer"
SANITIZE="-fsanitize=address,undefined -fno-omit-frame-pointer"

SRC="*.c"
OUT="noctis"

if [ "$MODE" = "STATIC" ]; then
    set -x
    CFLAGS="$COMMON_FLAGS -std=c17 -g -O0 $SANITIZE"
elif [ "$MODE" = "DEBUG"   ]; then
    set -x
    CFLAGS="$COMMON_FLAGS -std=c17 -g -O0"
elif [ "$MODE" = "RELEASE" ]; then
    CFLAGS="$COMMON_FLAGS -std=c17 -O3"
else
    echo "Unknown mode: $MODE"
    exit 1
fi

# --- Compile step ---
if command -v $CC &> /dev/null; then
    $CC $INCLUDE $CFLAGS -o $OUT $SRC
    echo "Compilation successful."
else
    echo "Compilation failed."
    exit 1
fi

# --- Static analysis section ---
# Run cppcheck if installed
if command -v cppcheck &> /dev/null; then
    echo "Running cppcheck..."
    cppcheck \
        --enable=warning,style,performance,portability \
        --check-level=exhaustive \
        --inconclusive \
        --std=c17 --language=c --force \
        -Iinclude \
        $SRC
else
    echo "cppcheck not found, skipping static analysis."
fi

# -- Format code --
# .indent.pro is configured to be like Kernighan & Ritchie style with modern tweaks
# -kr          K&R style
# -i4          Indent with 4 spaces
# -nut         Use spaces, not tabs
# -l100        Max line length (100 chars, adjust if you prefer 80/120)
# -ci4         Continuation indent = 4 spaces
# -cli4        Indent case labels 4 spaces under switch
# -bli0        Brace on same line
# -npsl        Don’t split function prototypes
if command -v indent &> /dev/null; then
    echo "Formatting code with indent..."
    mkdir -p .indent_backup
    indent *.c include/*.h include/ds/*.h 
    mv *.c~ include/*.h~ include/ds/*.h~ .indent_backup/
else
    echo "indent not found, skipping code formatting."
fi