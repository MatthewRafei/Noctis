#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "utils.h"

struct Token *token_alloc(enum Token_Type type,
                          char *lexeme,
                          size_t end,
                          const char *fp,
                          size_t row, size_t col)
{
  struct Token *token = s_malloc(sizeof(struct Token));

  token->type = type;
  token->lexeme = strndup(lexeme, end);
  token->fp = fp;
  token->row = row;
  token->col = col;

  return token;
}

void token_dump(const struct Token *t)
{
  printf("[lexeme: %s, type: %d, fp: %s, r: %zu, c: %zu]\n",
          t->lexeme, t->type, t->fp, t->row, t->col);
}

// TODO : Enum number to string name for better lexer output. 
char *enum_to_str(enum Token_Type type)
{ 
  (void)type;
  //switch()


  return '\0';
}