#include "context.h"
#include "diagnostic.h"
#include "io.h"
#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>

/*
TODO(malac0da):
- Refactor error handling for consistency (prefer returning error codes or using EXIT_FAILURE).

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

void cleanup_and_exit(struct CompilerContext context, char *src, struct Lexer *lexer)
{
    print_diagnostic_messages(context.array, context.array->len);
    free(context.array->data);
    free_diagnostic_array(context.array);
    if (lexer) {
        lexer_free(lexer);
    }
    if (src) {
        free(src);
    }
}

int main(int argc, const char *argv[])
{
    // All fun and games
    print_ascii_logo();

    struct CompilerContext context = create_compiler_context(MAIN);

    if (argc < 2) {
        report_error(FATAL, "No file arguments - Usage: ./noctis <filepath>", &context);
        cleanup_and_exit(context, NULL, NULL);
        return EXIT_FAILURE;
    }

    const char *fp = argv[1];
    char *src = file_to_str(fp, &context);

    // check source before calling lex_file
    if (!src) {
        report_error(FATAL, "No source file/s or is null.", &context);
        cleanup_and_exit(context, NULL, NULL);
        return EXIT_FAILURE;
    }
    // Lexer
    modify_compiler_context_stage(&context, LEXING);
    struct Lexer lexer = lex_file(src, fp, &context);

    if (lexer.status == LEXER_ERROR) {  // ERROR and FATAL cause program to exit with failure
        cleanup_and_exit(context, src, &lexer);
        return EXIT_FAILURE;
    } else if (context.array->len > 0) {        // Non-fatal errors, continue but print errors
        cleanup_and_exit(context, src, &lexer);
        return EXIT_FAILURE;
    } else {                    // no errors, continue as normal
        printf("\nLexer size: %zu\n\n", lexer.size);
        lexer_dump(&lexer);
        cleanup_and_exit(context, src, &lexer);
    }

    printf("Lexer Status: %s\n", enum_lexer_status_to_str(lexer.status));

    return EXIT_SUCCESS;
}
