#include "diagnostic.h"
#include "context.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
TODO(malac0da):
- Add unit tests for edge cases (NULL, empty strings, etc.).
*/

#define FNV_PRIME 16777619
#define FNV_OFFSET_BASIS 2166136261

uint32_t fnv1a(const char *key)
{
    if (!key) {                 // TODO(malac0da): Report error for null key
        return 0;
    }
    // FNV-1a offset basis
    uint32_t hash = FNV_OFFSET_BASIS;
    // strlen ensures that the string is null-terminated. So checking for it is redundant
    size_t length = strlen(key);

    if (length == 0) {
        return FNV_OFFSET_BASIS;        // Return offset basis for empty string
    }

    /*  
       There is the edge case that an empty string "" is given
       by design FNV-1a returns the offset basis. We could handle this differently
     */
    for (size_t i = 0; i < length; i++) {
        // Implicit promotion to unsigned char as key[i] is already char
        hash = hash ^ (unsigned char) key[i];

        hash = hash * FNV_PRIME;
    }

    return hash;
}
