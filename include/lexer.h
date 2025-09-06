#ifndef LEXER_H
#define LEXER_H

#include "context.h"
#include "token.h"

enum Lexer_Status {
    LEXER_OK,
    LEXER_ERROR,
};

struct Lexer {
    struct Token *hd, *tl;
    size_t size;
    enum Lexer_Status status;
};

struct S_Umap init_sym_keyword_tbl(struct CompilerContext *context);
struct Lexer lex_file(char *src, const char *fp, struct CompilerContext *context,
                      enum Lexer_Status status);
void lexer_dump(const struct Lexer *l);
void lexer_free(struct Lexer *l);
char *enum_lexer_status_to_str(enum Lexer_Status status);

#endif                          // LEXER_H
