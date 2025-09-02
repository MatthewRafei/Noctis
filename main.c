#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "context.h"
#include "diagnostic.h"
#include "io.h"

// dependency: libubsan

/*
TODO:
- Refactor error handling for consistency (prefer returning error codes or using EXIT_FAILURE).
- Use fprintf(stderr, ...) for error output.

- Fix typos and use more formal comments.

- Implement multi-file support
*/

int main(int argc, char *argv[])
{
  struct CompilerContext context = create_compiler_context();

  if (argc < 2) {
    // fprintf(stderr, "Usage: noctis <filepath>\n");
    // help();
    exit(1);
  }

  const char *fp = argv[1];
  char *src = file_to_str(fp);

  // check source before calling lex_file
  if(!src){
    // Replace with better error handling
    return 1; // Exit gracefully.
  }

  // Lexer
  struct Lexer lexer = lex_file(src, fp, context);
  lexer_dump(&lexer);
  printf("\nWhat is lexer size: %ld\n", lexer.size);

  // Temp diagnostic error messages
  printf("Warning: %s:%zu:%zu: %s\n", context.message_array[0].file, context.message_array[0].line, context.message_array[0].col, context.message_array[0].fmt);

  lexer_free(&lexer);
  free_diagnostic_message_dynarray(context.message_array);
  free(src);
  return 0;
}