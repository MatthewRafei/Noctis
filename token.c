#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "utils.h"

char *enum_to_str(enum Token_Type type) {
  switch(type) {
  case TOKEN_LPAREN:
    return "TOKEN_LPAREN";
  case TOKEN_RPAREN:
    return "TOKEN_RPAREN";
  case TOKEN_LCURLY:
    return "TOKEN_LCURLY";
  case TOKEN_RCURLY:
    return "TOKEN_RCURLY";
  case TOKEN_LSQUAR:
    return "TOKEN_LSQUAR";
  case TOKEN_RSQUAR:
    return "TOKEN_RSQUAR";
  case TOKEN_COMMA:
    return "TOKEN_COMMA";
  case TOKEN_DOT:
    return "TOKEN_DOT";
  case TOKEN_PLUS:
    return "TOKEN_PLUS";
  case TOKEN_MINUS:
    return "TOKEN_MINUS";
  case TOKEN_STAR:
    return "TOKEN_STAR";
  case TOKEN_SLASH:
    return "TOKEN_SLASH";
  case TOKEN_CARET:
    return "TOKEN_CARET";
  case TOKEN_MOD:
    return "TOKEN_MOD";
  case TOKEN_COLON:
    return "TOKEN_COLON";
  case TOKEN_QUESTION:
    return "TOKEN_QUESTION";
  case TOKEN_NOT:
    return "TOKEN_NOT";
  case TOKEN_GT:
    return "TOKEN_GT";
  case TOKEN_LT:
    return "TOKEN_LT";
  case TOKEN_GE:
    return "TOKEN_GE";
  case TOKEN_LE:
    return "TOKEN_LE";
  case TOKEN_NE:
    return "TOKEN_NE";
  case TOKEN_EQ:
    return "TOKEN_EQ";
  case TOKEN_ASSIGN:
    return "TOKEN_ASSIGN";
  case TOKEN_GTGT:
    return "TOKEN_GTGT";
  case TOKEN_LTLT:
    return "TOKEN_LTLT";
  case TOKEN_IDENTIFIER:
    return "TOKEN_IDENTIFIER";
  case TOKEN_STRING:
    return "TOKEN_STRING";
  case TOKEN_INT:
    return "TOKEN_INT";
  case TOKEN_FLOAT:
    return "TOKEN_FLOAT";
  case TOKEN_IF:
    return "TOKEN_IF";
  case TOKEN_THEN:
    return "TOKEN_THEN";
  case TOKEN_ELSE:
    return "TOKEN_ELSE";
  case TOKEN_TRUE:
    return "TOKEN_TRUE";
  case TOKEN_FALSE:
    return "TOKEN_FALSE";
  case TOKEN_AND:
    return "TOKEN_AND";
  case TOKEN_OR:
    return "TOKEN_OR";
  case TOKEN_WHILE:
    return "TOKEN_WHILE";
  case TOKEN_DO:
    return "TOKEN_DO";
  case TOKEN_FOR:
    return "TOKEN_FOR";
  case TOKEN_FUNC:
    return "TOKEN_FUNC";
  case TOKEN_NULL:
    return "TOKEN_NULL";
  case TOKEN_RETURN:
    return "TOKEN_RETURN";
  case TOKEN_SCOLON:
    return "TOKEN_SEMICOLON";
  case TOKEN_IMPORT:
    return "TOKEN_IMPORT";
  case TOKEN_EXPORT:
    return "TOKEN_EXPORT";
  case TOKEN_FLOAT_LIT:
    return "TOKEN_FLOAT_LIT";
  case TOKEN_INT_LIT:
    return "TOKEN_INT_LIT";
  case TOKEN_STRING_LIT:
    return "TOKEN_STRING_LIT";
  case TOKEN_UNIT:
    return "TOKEN_UNIT";
  case TOKEN_LET:
    return "TOKEN_LET";
  case TOKEN_MUT:
    return "TOKEN_MUT";
  case TOKEN_IN:
    return "TOKEN_IN";
  case TOKEN_FOR_RANGE:
    return "TOKEN_FOR_RANGE";    
  default:
    // GCC will complain 'TOKEN_SYMBOL_LEN'
    // and 'TOKEN_KEYWORD_LEN' are not accounted
    return "UNKNOWN_TOKEN or you forgot to add it to enum_to_str";
  }
}

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
    printf("[lexeme: %-20s \t type: %-20s \t filepath: %-30s \t row: %-3zu \t col: %-3zu]\n",
           t->lexeme, enum_to_str(t->type), t->fp, t->row, t->col);
}

