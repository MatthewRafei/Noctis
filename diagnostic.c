#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "diagnostic.h"

/*
INFO →
Extra messages for the user (e.g. "unused variable", "compilation started").
Never stops compilation.

WARNING →
Something looks suspicious but may still be valid (e.g. "variable declared but not used").
Should not stop compilation. Collect them all and print later.

ERROR →
A definite violation of the language rules (e.g. "expected ; after statement").
Usually compilation cannot continue successfully, but you want to recover and gather more errors so the user doesn’t have to fix one at a time.
→ Standard practice: log and continue as long as possible, but prevent emitting an executable/output at the end.

FATAL →
Something unrecoverable that prevents even further analysis (e.g. "out of memory", "could not open source file").
❌ Compilation must stop immediately.
*/
// TODO: SAVE as many INFO,WARNINGs, and ERRORS into a list as possible to print all at once when compilation fails or is finished.

void report_error(const char *file, const size_t line, const size_t col, const enum ErrorLevel level, const char *fmt)
{
    switch (level)
    {
    case INFO:
        fprintf(stderr, fmt, file, line, col, level);
        break;
    case WARNING:
        fprintf(stderr, "Warning: ");
        fprintf(stderr, "%s:%zu:%zu %s\n", file, line, col, fmt);
        exit(1);
        break;
    case ERROR:
        fprintf(stderr, fmt, file, line, col, level);
        break;
    case FATAL:
        fprintf(stderr, fmt, file, line, col, level);
        break;
    default:
        fprintf(stderr, "Error: %s:%zu:%zu: %s\n", file, line, col, fmt);
        break;
    }

}


