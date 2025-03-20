#!/bin/bash

set -xe

cc -Iinclude/ -Wextra -Wall -Werror -pedantic -g -O0 -o pinky *.c