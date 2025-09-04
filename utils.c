// Global utility functions 
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

// TODO(malac0da): better error handling
void *s_malloc(size_t bytes)
{
    void *p = malloc(bytes);
    if (!p) {
        perror("s_malloc");
        exit(1);
    }
    return p;
}