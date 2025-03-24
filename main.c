#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

// Parser use type pruning

// This is to test keyword ascpl
// delete after done
#include "keywords.h"
#include "ds/s-umap.h"

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
  //exit(1);
  return 1;
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
    return 1; // Exit gracefully, g
  }

  struct Lexer lexer = lex_file(src, fp);
  lexer_dump(&lexer);

  free(src);
  return 0;
}
