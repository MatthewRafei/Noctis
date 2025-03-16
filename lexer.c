#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "utils.h"

// TODO : Fix row counter

static void lexer_append(struct Lexer *l, struct Token *t)
{
    if (!l->hd || !l->tl) {
        l->hd = l->tl = t;
    }
    else {
        l->tl->next = t;
        l->tl = l->tl->next;
    }
}

void lexer_dump(const struct Lexer *l)
{
    struct Token *node = l->hd;
    while(node){
        token_dump(node);
        node = node->next;
    }
}

// consume_while returns a counter to the number of indexes
// we need to skip ahead until the while loop is broken by the predicate.
// Predicate being a function pointer.
size_t consume_while(char *s, int (*pred)(int))
{
    size_t counter = 0;
    while(s[counter] && pred(s[counter])){
        counter++;
    }
    return counter;
}

// Example predicate
int not_newline(int c) 
{
    return (char)c != '\n' && (char)c != '\r';
}

// Used for multi-line comments
int not_RPAREN(int c)
{
    return (char)c != ')';
}

struct Lexer lex_file(char *src, const char *fp)
{
    struct Lexer lexer = (struct Lexer) {
        .hd = NULL,
        .tl = NULL,
    };

    size_t i = 0, row = 1, col = 1;
    while(src[i] != '\0'){
        char ch = src[i];

        // Skip whitespace and tabs
        if(ch == ' ' || ch == '\t') {
            i += 1, col += 1;
        }

        // Skip newline and carriage return
        else if(ch == '\n' || ch == '\r') {
            i += 1, row += 1, col = 1;
        }

        // Multi-line comment
        else if(ch == '(' && SAFE_PEEK(src, i+1, '*')){
            size_t len = consume_while(src + i, not_RPAREN);
            len += 1; // Skip )
            i += len, col += len;
        }

        // Single-line comment
        else if(ch == '-' && SAFE_PEEK(src, i+1, '-')){
            size_t len = consume_while(src + i, not_newline);

            // Shouldnt the row be adjusted?
            i += len, col += len;
        }

        // Identifiers/keywords
        else if(isalpha(ch) || ch == '_'){
            ;
        }

        // Numbers (floats unimplemented)
        else if(isdigit(ch)) {
            size_t len = consume_while(src + i, isdigit);

            struct Token *t = token_alloc(TOKEN_INTEGER, src+i, len, fp, row, col);
            lexer_append(&lexer, t);

            i += len, col += len;
        }

        // Operators
        else {
            ;
        }

    }
    return lexer;
}
