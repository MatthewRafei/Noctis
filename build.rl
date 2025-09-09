#!/usr/bin/env earl

module Build

import "std/script.rl";
import "std/colors.rl";

set_flag("-e");

fn ok(msg)   { println(Colors::Tfc.Green, Colors::Te.Bold, "* ", msg, Colors::Te.Reset); }
fn info(msg) { println(Colors::Tfc.Yellow, "* ", msg, Colors::Te.Reset); }
fn bad(msg)  { println(Colors::Tfc.Red, Colors::Te.Bold, "* ", msg, Colors::Te.Reset); }

@world fn cppcheck() {
    if Script::program_exists("cppcheck") {
        info("Running cppcheck...");
        with checkflags = f"--enable=warning,style,performance,portability --check-level=exhaustive --inconclusive -{ccver} --language=c --force"
        in $f"cppcheck {checkflags} {include} {src}";
    } else {
        info("cppcheck not found, skipping static analysis.");
    }
}

@world fn indent() {
    if Script::program_exists("indent") {
        info("Formatting code with indent...");
        $"mkdir -p .indent_backup";
        $"indent *.c include/*.h include/ds/*.h";
        $"mv *.c~ include/*.h~ include/ds/*.h~ .indent_backup/";
    } else {
        info("indent not found, skipping code formatting.");
    }
}

@world fn compile() {
    if Script::program_exists(cc) {
        $f"{cc} {ccver} {include} {cflags} -o {out} {src}";
        ok("Compilation successful.");
    } else {
        bad("Compilation failed.");
        exit(1);
    }
}

let release = false;
try     { release = argv()[1] == "RELEASE"; }
catch _ { set_flag("-x"); }

# Build information
@const let cc, ccver = ("cc", "-std=gnu17");

# Flags
@const let include, common_flags, sanatize = (
    "-Iinclude/",
    f"{ccver} -Wall -Wextra -Wshadow -Wconversion -Wundef -Werror -pedantic -fanalyzer",
    "-fsanitize=address,undefined",
);

@const let cflags = f"{common_flags} -std=gnu17 " + case release of {
    true = "-O2";
    _    = f"-g -O0 {sanatize}";
};

# File information
@const let src, out = ("*.c", "noctis");

compile();
cppcheck();
indent();
