#include <stdio.h>
#include <stdlib.h>

enum Token_type{
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


struct Lexer{
  int start_character;
  int current_character;
  int current_line;
  struct Token*[] ;
};


struct Token{
  enum Token_Type type;
  
};


void lexer(char* file_name)
{
  FILE *fptr = fopen(file_name, "r");
  if(fptr == NULL){
    printf("File could not open\n");
    exit(1);
  }

  char c = '\0';
  while((c = fgetc(fptr)) != EOF){
    
  }
}

int main(int argc, char *argv[])
{
  if(argc != 2){
    printf("Error, 2 arguments are required\n");
    exit(1);
  }

  printf("Print file name: %s\n", argv[1]);

  // Pass source file
  lexer(argv[1]);
  
  return 0;
}
