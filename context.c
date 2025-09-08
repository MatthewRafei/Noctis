#include "context.h"
#include "diagnostic.h"

#include <stdlib.h>

struct CompilerContext create_compiler_context(enum CurrentStage stage)
{
    struct CompilerContext context;
    context.stage = stage;
    context.array = inital_diagnostic_system();

    context.array->len = 0;
    context.source = (struct SourceLocation) {
        .file = NULL,
        .line = 0,
        .col = 0
    };

    return context;
}

void modify_compiler_context_stage(struct CompilerContext *context, enum CurrentStage stage)
{
    context->stage = stage;
}
