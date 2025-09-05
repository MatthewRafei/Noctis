#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdlib.h>

struct SourceLocation {
    const char *file;
    size_t line;
    size_t col;
};

enum CurrentStage {
    MAIN,
    LEXING,
    PARSING,
    SEMANTIC_ANALYSIS,
    CODEGEN,
    OPTIMIZATION,
    ASSEMBLY,
    LINKING
};

struct CompilerContext {
    enum CurrentStage stage;
    struct DiagnosticMessage *message_array;
    size_t num_of_errors;
    struct SourceLocation source;
};

struct CompilerContext create_compiler_context(enum CurrentStage stage);
void modify_compiler_context_stage(struct CompilerContext *context, enum CurrentStage stage);

#endif                          // CONTEXT_H
