// Global utility functions 
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

void *s_malloc(size_t bytes)
{
    void *p = malloc(bytes);
    if (!p) {
        perror("s_malloc");
        exit(1);
    }
    return p;
}
