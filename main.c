#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

/*
TODO:
- Refactor error handling for consistency (prefer returning error codes or using EXIT_FAILURE).
- Fix typos and use more formal comments.
- CHECK: Check ftell(file) for < 0 before casting to size_t.
- Use fprintf(stderr, ...) for error output.
- Consider future multi-file support in argument parsing.

- Implement multi-file support

*/

char *file_to_str(const char *fp)
{
  FILE *file = fopen(fp, "rb");

  if (!file) {
    perror("Failed to open file");
    exit(1);
  }

  if (fseek(file, 0, SEEK_END) != 0){
    perror("Failed to seek file");
    fclose(file);
    return NULL;
  }

  if (ftell(file) < 0) {
    perror("Failed to determine file length");
    fclose(file);
    return NULL;
  }

  size_t length = ftell(file);

  // Some file-systems might return -1
  if(length == (size_t)-1){
    printf("Failed to determine file length\n");
    fclose(file);
    return NULL;
  }

  rewind(file);

  // No need to cast char* as malloc's return type
  // is already void *
  char *buffer = malloc(length + 1);
  if (!buffer) {
    perror("Failed to allocate memory");
    fclose(file);
    return NULL;
  }

  //Free buffer if fread fails
  size_t read_size = fread(buffer, 1, length, file);
  if(read_size != (size_t)length){
    perror("Failed to read file");
    free(buffer);
    fclose(file);
    return NULL;
  }

  buffer[length] = '\0';
  fclose(file);
  return buffer;
}

int help(void)
{
  printf("Usage: noctis <filepath>\n");
  //return 1;
  exit(1);
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    help();
  }

  const char *fp = argv[1];
  char *src = file_to_str(fp);

  // check source before calling lex_file
  if(!src){
    // Maybe replace with better error handling
    return 1; // Exit gracefully, you OG
  }

  // Lexer
  struct Lexer lexer = lex_file(src, fp);
  lexer_dump(&lexer);
  printf("\nWhat is lexer size: %ld\n", lexer.size);

  lexer_free(&lexer);
  free(src);
  return 0;
}