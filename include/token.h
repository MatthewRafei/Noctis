#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>
#include <stddef.h>

enum Token_Type {
  // Single-character tokens
  TOKEN_LPAREN = 0,// (
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
  TOKEN_SCOLON,    // ;
  TOKEN_AND,       // &

  // Multi-character tokens
  TOKEN_GE,        // >=
  TOKEN_LE,        // <=
  TOKEN_NE,        // ~=
  TOKEN_EQ,        // ==
  TOKEN_ASSIGN,    // =
  TOKEN_GTGT,      // >>
  TOKEN_LTLT,      // <<
  TOKEN_FOR_RANGE, // {num}..{num}
  TOKEN_SYMBOL_LEN,// DO NOT USE

  // Keywords
  TOKEN_IF,
  TOKEN_THEN,
  TOKEN_ELSE,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_OR,
  TOKEN_WHILE,
  TOKEN_DO,
  TOKEN_FOR,
  TOKEN_FUNC,
  TOKEN_NULL,
  TOKEN_RETURN,
  TOKEN_IMPORT,
  TOKEN_EXPORT,
  TOKEN_FLOAT,
  TOKEN_INT,
  TOKEN_CHAR,
  TOKEN_STRING,
  TOKEN_UNIT,
  TOKEN_LET,
  TOKEN_MUT,
  TOKEN_IN,
  TOKEN_KEYWORD_LEN, // DO NOT USE

  // Literals
  TOKEN_IDENTIFIER,
  TOKEN_STRING_LIT,
  TOKEN_INT_LIT,
  TOKEN_FLOAT_LIT,
};

struct Token {
  enum Token_Type type;
  char *lexeme;
  const char *fp;
  size_t row, col;
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
