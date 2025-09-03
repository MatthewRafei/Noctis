#include <stdio.h>
#include <stdlib.h>

// TODO(malac0da): Replace all error messages with diagnostic reporting

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

  size_t length = (size_t)ftell(file);

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