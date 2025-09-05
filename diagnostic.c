#include "diagnostic.h"
#include "context.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
Something unrecoverable that prevents even further analysis (e.g. "out of memory", "could not open source file"). Compilation must stop immediately.
*/

// TODO(malac0da): SAVE as many INFO,WARNINGs, and ERRORS into a list as possible to print all at once when compilation fails or is finished.
// TODO(malac0da): Finish dynamic array
// TODO(malac0da): Build function that turns enum types to strings or hard code strings? idk what is best yet.

#define DYNARR_INITIAL_SIZE 16

struct DiagnosticMessage *inital_diagnostic_system(void)
{
    struct DiagnosticMessage *message_array = create_diagnostic_message_dynarray();
    return message_array;
}

void report_error(const char *file, const size_t line, const size_t col,
                  const enum ErrorLevel level, const char *fmt, struct CompilerContext *context)
{
    struct DiagnosticMessage message = create_message(file, line, col, level, fmt);

    push_error(context->message_array, message, context->num_of_errors);

    context->num_of_errors += 1;
}

/*
TODO(malac0da): Implement this struct to hold source location information so we dont have to pass in so many arguments

struct SourceLocation {
    const char *file;
    size_t line;
    size_t col;
};

create_message((struct SourceLocation){ "main.c", 42, 17 }, ERROR_FATAL, "oops");
*/
struct DiagnosticMessage create_message(const char *file, const size_t line, const size_t col,
                                        const enum ErrorLevel level, const char *fmt)
{
    struct DiagnosticMessage message;

    message.file = file;
    message.line = line;
    message.col = col;

    message.level = level;
    message.fmt = fmt;

    return message;
}

struct DiagnosticMessage *create_diagnostic_message_dynarray(void)
{
    return (struct DiagnosticMessage *) malloc(DYNARR_INITIAL_SIZE *
                                               sizeof(struct DiagnosticMessage));
}

void push_error(struct DiagnosticMessage *message_array, struct DiagnosticMessage message,
                const size_t number_of_errors)
{
    message_array[number_of_errors] = message;
}

void free_diagnostic_message_dynarray(struct DiagnosticMessage *message_array)
{
    free(message_array);
}

void print_diagnostic_messages(const struct DiagnosticMessage *message_array,
                               const size_t num_of_errors)
{
    printf("Total Errors: %zu\n", num_of_errors);
    for (size_t i = 0; i < num_of_errors; ++i) {
        (void) fprintf(stderr, "Error: %s:%zu:%zu: %s\n", message_array[i].file,
                       message_array[i].line, message_array[i].col, message_array[i].fmt);
    }
}
