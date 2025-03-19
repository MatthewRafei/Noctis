#include <stddef.h>
#include <string.h>

#include "keywords.h"

int is_keyword(const char *s)
{
    const char *kws[] = KEYWORD_ASCPL;
    for (size_t i = 0; i < sizeof(kws)/sizeof(*kws); ++i) {
        if (!strcmp(s, kws[i])) {
            return 1;
        }
    }
    return 0;
}