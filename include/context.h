#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdlib.h>

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
  struct DiagnosticMessage* message_array;
  size_t num_of_errors;
};

struct CompilerContext create_compiler_context();
void modify_compiler_context_stage(struct CompilerContext *context, enum CurrentStage stage);

#endif // CONTEXT_H