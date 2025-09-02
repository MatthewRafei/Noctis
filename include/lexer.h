#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "context.h"

struct Lexer {
  struct Token *hd, *tl;
  size_t size;
};

struct Lexer lex_file(char *src, const char *fp, struct CompilerContext context);
void lexer_dump(const struct Lexer *l);
void lexer_free(struct Lexer *l);

#endif // LEXER_H