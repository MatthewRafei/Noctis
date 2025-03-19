#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

// Macro to safely look at an index in an array safely
// by checking to see if the item at that index is NULL
// or not. The parenthesis are there to make sure that
// you are doing the correct expression.
#define SAFE_PEEK(arr, i, el) ((arr)[(i)] && (arr)[(i)] == (el))

#define err_wargs(msg, ...) \
    do { \
        fprintf(stderr, "[Error]: " msg "\n", __VA_ARGS__); \
        exit(1); \
    } while (0)

#define err(msg) \
    do { \
        fprintf(stderr, "[Error]: " msg "\n"); \
        exit(1); \
    } while (0)

#define halt (exit(0))

void *s_malloc(size_t bytes);

#endif // UTILS_H