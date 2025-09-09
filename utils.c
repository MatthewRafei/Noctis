// Global utility functions 
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Consider adding calling functions name
    as a parameter so we can track which function
    failed to allocate memory?

*/
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
    char *p = s_malloc(len);
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
    char *p = s_malloc(len + 1);
    memcpy(p, s, len);
    p[len] = '\0';
    return p;
}
