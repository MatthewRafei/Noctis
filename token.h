#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

enum Token_Type {
  // Single-character tokens
  TOKEN_LPAREN,    // (
  TOKEN_RPAREN,    // )
  TOKEN_LCURLY,    // {
  TOKEN_RCURLY,    // }
  TOKEN_LSQUAR,    // [
  TOKEN_RSQUAR,    // ]
  TOKEN_COMMA,     // ,
  TOKEN_DOT,       // .
  TOKEN_PLUS,      // +
  TOKEN_MINUS,     // -
  TOKEN_STAR,      // *
  TOKEN_SLASH,     // /
  TOKEN_CARET,     // ^
  TOKEN_MOD,       // %
  TOKEN_COLON,     // :
  TOKEN_QUESTION,  // ?
  TOKEN_NOT,       // ~
  TOKEN_GT,        // >
  TOKEN_LT,        // <

  // Multi-character tokens
  TOKEN_GE,        // >=
  TOKEN_LE,        // <=
  TOKEN_NE,        // ~=
  TOKEN_EQ,        // ==
  TOKEN_ASSIGN,    // :=
  TOKEN_GTGT,      // >>
  TOKEN_LTLT,      // <<

  // Literals
  TOKEN_IDENTIFIER,
  TOKEN_STRING,
  TOKEN_INTEGER,
  TOKEN_FLOAT,

  // Keywords
  TOKEN_IF,
  TOKEN_THEN,
  TOKEN_ELSE,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_AND,
  TOKEN_OR,
  TOKEN_WHILE,
  TOKEN_DO,
  TOKEN_FOR,
  TOKEN_FUNC,
  TOKEN_NULL,
  TOKEN_END,
  TOKEN_PRINT,
  TOKEN_PRINTLN,
  TOKEN_RET
};

struct Token {
  enum Token_Type type;
  char *lexeme;
  const char *fp;
  size_t row, col;
  // Next thing in the linked list
  struct Token *next;
};

struct Token *token_alloc(enum Token_Type type,
                          char *lexeme,
                          size_t end,
                          const char *fp,
                          size_t row, size_t col);

void token_dump(const struct Token *t);
char *enum_to_str(enum Token_Type type);

#endif // TOKEN_H