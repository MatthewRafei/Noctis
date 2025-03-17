#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

// FNV-1a prime
const uint32_t prime = 16777619;

unsigned long fnv1a(const char* key)
{
  if(!key){
    printf("Error key is null");

    /*
      this will need to change when program gets bigger
      as stuff needs to get cleaned up. and error messages should be better
    */
    exit(1);
  }

  // FNV-1a offset basis
  unsigned long hash = 2166136261;

  int length = strlen(key);

  if (key[length] != '\0') {
    printf("Error: String is not properly null-terminated\n");
    exit(1);
  }

  /*  
    There is the edge case that an empty string "" is given
    by design FNV-1a returns the offset basis. We could handle this differently
  */
  for(int i = 0; i < length; i++){
    // Implicit promotion to unsigned char as key[i] is already char
    printf("key[i]: %c\n", key[i]);

    // commenting this line also fixes the seg fault
    hash = hash ^ (unsigned char)key[i];

    // Potential int overflow if hash was uint32_t
    // So I changed it to uint64_t
    hash = hash * prime;
  }
  if(!hash){
    printf("Hash is null\n");
    exit(1);
  }

  return hash;
}
