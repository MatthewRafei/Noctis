#!/bin/bash

set -xe

cc -Iinclude/ -Wextra -Wall -std=gnu17 -Werror -pedantic -g -O0 -o noctis *.c
