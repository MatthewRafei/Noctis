#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "diagnostic.h"
#include "context.h"

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
// TODO: Finish dynamic array
// TODO: Build function that turns enum types to strings or hard code strings? idk what is best yet.
// TODO: Build function that can print all error messages to stderr

#define DYNARR_INITIAL_SIZE 16

struct DiagnosticMessage* inital_diagnostic_system(void)
{
    struct DiagnosticMessage* message_array = create_diagnostic_message_dynarray();
    return message_array;
}

void report_error(const char *file, const size_t line, const size_t col, const enum ErrorLevel level, const char *fmt, struct CompilerContext *context)
{
    struct DiagnosticMessage message = create_message(file, line, col, level, fmt);

    switch (level)
    {
    case INFO:
        fprintf(stderr, fmt, file, line, col, level);
        break;
    case WARNING:
        push_error(context->message_array, message, context);
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
    
    context->num_of_errors += 1;
}

struct DiagnosticMessage create_message(const char *file, const size_t line, const size_t col, const enum ErrorLevel level, const char *fmt)
{
    struct DiagnosticMessage message;
    message.file = file;
    message.line = line;
    message.col = col;
    message.level = level;
    message.fmt = fmt;

    return message;
}

struct DiagnosticMessage* create_diagnostic_message_dynarray(void)
{
    return (struct DiagnosticMessage*)malloc(DYNARR_INITIAL_SIZE * sizeof(struct DiagnosticMessage*));
}

void push_error(struct DiagnosticMessage* message_array, struct DiagnosticMessage message, struct CompilerContext *context)
{
    message_array[context->num_of_errors] = message;
}

void free_diagnostic_message_dynarray(struct DiagnosticMessage* message_array)
{
    free(message_array);
}