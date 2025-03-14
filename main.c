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
  rewind(file);

  char *buffer = (char *)malloc(length + 1);
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
  (void)lexer;

  free(src);

  return 0;
}
