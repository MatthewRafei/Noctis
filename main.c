#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

char *file_to_str(const char *fp)
{
  FILE *file = fopen(fp, "rb");

  if (!file) {
    perror("Failed to open file");
    exit(1);
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  // Some file-systems might return -1
  if(length == -1){
    printf("Failed to determine file length\n");
    fclose(file);
    return NULL;
  }

  // Go through file one to determine length
  // Then go back to beginning
  rewind(file);

  // No need to cast char* as malloc's return type
  // is already void *
  char *buffer = malloc(length + 1);
  if (!buffer) {
    perror("Failed to allocate memory");
    fclose(file);
    return NULL;
  }

  fread(buffer, 1, length, file);
  fclose(file);

  buffer[length] = '\0';
  return buffer;
}

void help(void)
{
  printf("Usage: pinky <filepath>\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    help();
  }

  const char *fp = argv[1];
  char *src = file_to_str(fp);

  struct Lexer lexer = lex_file(src, fp);
  lexer_dump(&lexer);

  free(src);

  return 0;
}
