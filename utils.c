// Global utility functions 
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

void *s_malloc(size_t bytes)
{
    void *p = malloc(bytes);
    if (!p) {
        // TODO(malac0da): better error handling
        perror("s_malloc");
        exit(1);
    }
    return p;
}
