#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

// FNV-1a prime
const uint32_t prime = 16777619;

uint64_t fnv1a(char* key)
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
  uint64_t hash = 2166136261;

  int length = strlen(key);

  /*
    There is the edge case that an empty string "" is given
    by design FNV-1a returns the offset basis. We could handle this differently
  */
  for(int i = 0; i < length; i++){
    // Implicit promotion to unsigned char as key[i] is already char
    hash = hash ^ key[i];

    // Potential int overflow if hash was uint32_t
    // So I changed it to uint64_t
    hash = hash * prime;
  }
  
  return hash;
}
