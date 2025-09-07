#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include "context.h"

enum ErrorLevel {
    INTERNAL,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

// Absoultely stupid to forget to track capacity so now we gotta rewrite
// TODO(malac0da): Impliment this struct and rewrite the system.
/*
struct DiagnosticArray {
    struct DiagnosticMessage *data;
    size_t len;      // number of messages actually stored
    size_t capacity; // allocated slots
};
*/

struct DiagnosticMessage {
    const char *file;
    size_t line;
    size_t col;
    enum ErrorLevel level;
    const char *fmt;
};

char *enum_error_to_str(enum ErrorLevel level);
void report_error(const enum ErrorLevel level, const char *fmt, struct CompilerContext *context);
struct DiagnosticMessage *inital_diagnostic_system(void);
struct DiagnosticMessage create_message(const enum ErrorLevel level, const char *fmt,
                                        struct CompilerContext *context);
void push_error(struct DiagnosticMessage *message_array, struct DiagnosticMessage message,
                const size_t number_of_errors);
struct DiagnosticMessage *create_diagnostic_message_dynarray(void);
void free_diagnostic_message_dynarray(struct DiagnosticMessage *message_array);
void print_diagnostic_messages(const struct DiagnosticMessage *message_array,
                               const size_t num_of_errors);

#endif                          // DIAGNOSTIC_H
