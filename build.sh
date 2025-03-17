#!/bin/bash

set -xe

cc -Wextra -Wall -Werror -pedantic -g -O0 -o pinky *.c