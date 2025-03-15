#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "utils.h"

// consume_while returns a counter to the number of indexes
// we need to skip ahead until the while loop is broken by the predicate.
// Predicate being a function pointer.
size_t consume_while(char *s, int (*pred)(int))
{
    size_t counter = 0;
    while(s[counter] && pred(s[counter]))
        counter++;
    return counter;
}

// Example predicate
int not_newline(int c) {
    return (char)c != '\n' && (char)c != '\r';
}

struct Lexer lex_file(char *src, const char *fp)
{
    (void)fp;
    struct Lexer lexer = (struct Lexer) {
        .hd = NULL,
        .tl = NULL,
    };

    size_t i = 0, row = 1, col = 1;
    while(src[i] != '\0'){
        char ch = src[i];

        // Skip whitespace and tabs
        if (ch == ' ' || ch == '\t') {
            i += 1, col += 1;
        }

        // Skip newline and carriage return
        else if (ch == '\n' || ch == '\r') {
            i += 1, row += 1, col = 1;
        }

        // Single-line comment
        else if(ch == '-' && SAFE_PEEK(src, i+1, '-')){
            size_t len = consume_while(src + i, not_newline);
            assert(0 && "unimplemented");
            i += len, col += len;
        }

        // Identifiers/keywords
        else if (isalpha(ch) || ch == '_') {
            ;
        }

        // Numbers
        else if (isdigit(ch)) {
            ;
        }

        // Operators
        else {
            ;
        }
    }

    return lexer;
}
