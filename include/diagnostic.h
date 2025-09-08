#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include "context.h"

#include <stdlib.h>

struct CompilerContext;

enum ErrorLevel {
    INTERNAL,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

// Absoultely stupid to forget to track capacity so now we gotta rewrite
// TODO(malac0da): Impliment this struct and rewrite the system.
struct DiagnosticArray {
    struct DiagnosticMessage *data;
    size_t len;                 // number of messages actually stored
    size_t capacity;            // allocated slots
};

struct DiagnosticMessage {
    const char *file;
    size_t line;
    size_t col;
    enum ErrorLevel level;
    const char *fmt;
};

char *enum_error_to_str(enum ErrorLevel level);
void report_error(const enum ErrorLevel level, const char *fmt, struct CompilerContext *context);
struct DiagnosticArray *inital_diagnostic_system(void);
struct DiagnosticMessage create_message(const enum ErrorLevel level, const char *fmt,
                                        struct CompilerContext *context);
void push_error(struct DiagnosticArray *array, struct DiagnosticMessage message,
                const size_t number_of_errors);
struct DiagnosticArray *create_diagnostic_array(void);
void free_diagnostic_array(struct DiagnosticArray *array);
void print_diagnostic_messages(const struct DiagnosticArray *array, const size_t num_of_errors);

#endif                          // DIAGNOSTIC_H
