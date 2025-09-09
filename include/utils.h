#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

// Macro to safely look at an index in an array safely
// by checking to see if the item at that index is NULL
// or not. The parenthesis are there to make sure that
// you are doing the correct expression.
#define SAFE_PEEK(arr, i, el) ((arr)[(i)] && (arr)[(i)] == (el))

void *s_malloc(size_t bytes);
char *my_strdup(const char *s);
char *my_strndup(const char *s, size_t n);
size_t my_strnlen(const char *s, size_t maxlen);


#endif                          // UTILS_H
