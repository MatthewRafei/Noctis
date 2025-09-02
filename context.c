#include <stdlib.h>

#include "context.h"
#include "diagnostic.h"


struct CompilerContext create_compiler_context(enum CurrentStage stage)
{
    struct CompilerContext context;
    context.stage = stage;
    context.message_array = inital_diagnostic_system();
    context.num_of_errors = 0;

    return context;

}

void modify_compiler_context_stage(struct CompilerContext *context, enum CurrentStage stage)
{
    context->stage = stage;
}