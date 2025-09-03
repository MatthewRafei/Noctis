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


void print_ascii_logo(void) {
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

int main(int argc, char *argv[])
{
  // All fun and games
  print_ascii_logo();

  struct CompilerContext context = create_compiler_context();
  modify_compiler_context_stage(&context, MAIN);

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
  modify_compiler_context_stage(&context, LEXING);
  struct Lexer lexer = lex_file(src, fp, &context, LEXER_OK);
  printf("Lexer Status: %d\n", lexer.status);
  if(lexer.status == LEXER_ERROR){
    print_diagnostic_messages(context.message_array, context.num_of_errors);
    lexer_free(&lexer);
    free_diagnostic_message_dynarray(context.message_array);
    free(src);
    return 1; // Exit gracefully.
  }



  lexer_dump(&lexer);
  printf("\nLexer size: %ld\n\n", lexer.size);

  lexer_free(&lexer);
  free_diagnostic_message_dynarray(context.message_array);
  free(src);
  return 0;
}