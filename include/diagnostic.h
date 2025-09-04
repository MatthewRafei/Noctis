#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include "context.h"

enum ErrorLevel {
    INFO,
    WARNING,
    ERROR,
    FATAL
};

struct DiagnosticMessage {
    const char *file;
    size_t line;
    size_t col;
    enum ErrorLevel level;
    const char *fmt;
};

void report_error(const char *file, const size_t line, const size_t col, const enum ErrorLevel level, const char *fmt, struct CompilerContext *context);
struct DiagnosticMessage* inital_diagnostic_system(void);
struct DiagnosticMessage create_message(const char *file, const size_t line, const size_t col, const enum ErrorLevel level, const char *fmt);
void push_error(struct DiagnosticMessage* message_array, struct DiagnosticMessage message, const struct CompilerContext *context);
struct DiagnosticMessage* create_diagnostic_message_dynarray(void);
void free_diagnostic_message_dynarray(struct DiagnosticMessage* message_array);
void print_diagnostic_messages(const struct DiagnosticMessage* message_array, const size_t num_of_errors);

#endif // DIAGNOSTIC_H