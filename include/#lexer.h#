#ifndef LEXER_H
#define LEXER_H

#include "token.h"

struct Lexer {
  struct Token *hd, *tl;
};

struct Lexer lex_file(char *src, const char *fp);
void lexer_dump(const struct Lexer *l);

#endif // LEXER_H
