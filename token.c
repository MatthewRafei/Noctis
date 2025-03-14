#include <assert.h>

#include "token.h"

struct Token *tokenize(enum Token_Type type,
                       char *lexeme,
                       const char *fp,
                       size_t row, size_t col)
{
  (void)type;
  (void)lexeme;
  (void)fp;
  (void)row;
  (void)col;
  assert(0 && "unimplemented");
}