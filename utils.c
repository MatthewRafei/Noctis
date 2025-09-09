// Global utility functions 
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char *my_strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *p = malloc(len);
    if (p)
        memcpy(p, s, len);
    return p;
}

size_t my_strnlen(const char *s, size_t maxlen)
{
    size_t i;
    for (i = 0; i < maxlen && s[i]; i++);
    return i;
}

char *my_strndup(const char *s, size_t n)
{
    size_t len = my_strnlen(s, n);
    char *p = malloc(len + 1);
    if (!p)
        return NULL;
    memcpy(p, s, len);
    p[len] = '\0';
    return p;
}
