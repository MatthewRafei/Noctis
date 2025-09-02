#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdlib.h>

enum CurrentStage {
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

#endif // CONTEXT_H