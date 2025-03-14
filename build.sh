#!/bin/bash

set -xe

cc -Wextra -Wall -Werror -pedantic -ggdb -O0 -o pinky *.c