#!/usr/local/bin/earl

module Build

set_flag("-xe");

let run = len(argv()) > 1 && argv()[1] == "run";

$"cc -Iinclude/ -Wextra -Wall -Werror -pedantic -ggdb -O0 -o noctis *.c";

if run {
    $"./noctis " + argv()[2:].fold(|k, acc| { acc + " " + k; }, "");
}