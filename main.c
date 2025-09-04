#include "context.h"
#include "diagnostic.h"
#include "io.h"
#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>

/*
TODO(malac0da):
- Refactor error handling for consistency (prefer returning error codes or using EXIT_FAILURE).
- Use fprintf(stderr, ...) for error output.

- Fix typos and use more formal comments.

- Implement multi-file support
*/

void print_ascii_logo(void)
{
    const char *logo[] = {
        "                                                              ",
        "███▄▄▄▄    ▄██████▄   ▄████████     ███      ▄█     ▄████████",
        "███▀▀▀██▄ ███    ███ ███    ███ ▀█████████▄ ███    ███    ███",
        "███   ███ ███    ███ ███    █▀     ▀███▀▀██ ███▌   ███    █▀",
        "███   ███ ███    ███ ███            ███   ▀ ███▌   ███        ",
        "███   ███ ███    ███ ███            ███     ███▌ ▀███████████",
        "███   ███ ███    ███ ███    █▄      ███     ███           ███",
        "███   ███ ███    ███ ███    ███     ███     ███     ▄█    ███",
        " ▀█   █▀   ▀██████▀  ████████▀     ▄████▀   █▀    ▄████████▀",
        "                                                              "
    };

    size_t lines = sizeof(logo) / sizeof(logo[0]);
    for (size_t i = 0; i < lines; i++) {
        printf("%s\n", logo[i]);
    }
}

int main(int argc, const char *argv[])
{
  // All fun and games
  print_ascii_logo();

  struct CompilerContext context = create_compiler_context(MAIN);

  if (argc < 2) {
    report_error(__FILE__, context.line, context.col, FATAL, "Usage: noctis <filepath>", &context);
    print_diagnostic_messages(context.message_array, context.num_of_errors);
    free_diagnostic_message_dynarray(context.message_array);
    return EXIT_FAILURE;
  }

  const char *fp = argv[1];
  char *src = file_to_str(fp);

  // check source before calling lex_file
  if(!src){
    // Replace with better error handling
    return EXIT_FAILURE;
  }

  // Lexer
  modify_compiler_context_stage(&context, LEXING);
  struct Lexer lexer = lex_file(src, fp, &context, LEXER_OK);
  printf("Lexer Status: %d\n", lexer.status);
  if(lexer.status == LEXER_ERROR){
    print_diagnostic_messages(context.message_array, context.num_of_errors);
    lexer_free(&lexer);
    free_diagnostic_message_dynarray(context.message_array);
    free(src);
    return EXIT_FAILURE;
  }
  else if (context.num_of_errors > 0){
    print_diagnostic_messages(context.message_array, context.num_of_errors);
  }
  else{
    printf("\nLexer size: %zu\n\n", lexer.size);
    lexer_dump(&lexer);
    free_diagnostic_message_dynarray(context.message_array);
    free(src);
  }


  return EXIT_SUCCESS;
}