#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
TODO:
- Add unit tests for edge cases (NULL, empty strings, etc.).
*/

#define FNV_PRIME 16777619

uint32_t fnv1a(const char* key)
{
  if(!key){
    // TODO(malac0da): Handle null key case in diag system
    (void)fprintf(stderr, "Error key is null");
    exit(EXIT_FAILURE);
  }

  // FNV-1a offset basis
  uint32_t hash = 2166136261;
  // strlen ensures that the string is null-terminated. So checking for it is redundant
  size_t length = strlen(key);

  if (length == 0) {
    return 2166136261; // Return offset basis for empty string
  }
  
  /*  
    There is the edge case that an empty string "" is given
    by design FNV-1a returns the offset basis. We could handle this differently
  */
  for(size_t i = 0; i < length; i++){
    // Implicit promotion to unsigned char as key[i] is already char
    hash = hash ^ (unsigned char)key[i];

    hash = hash * FNV_PRIME;
  }

  return hash;
}