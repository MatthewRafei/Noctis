#ifndef CONTEXT_H
#define CONTEXT_H

#include "diagnostic.h"

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
};

struct CompilerContext {
    enum CurrentStage stage;
    struct DiagnosticArray *array;
    struct SourceLocation source;
};

struct CompilerContext create_compiler_context(enum CurrentStage stage);
void modify_compiler_context_stage(struct CompilerContext *context, enum CurrentStage stage);

#endif                          // CONTEXT_H
