struct Parser{
  struct Lexer lexer;
  size_t lexer_length;
  size_t current_token;
};

enum AST_Token_Type {
  AST_TOKEN_INT
};

struct AST_Token{
  int i;
};

struct AST{
  int i;
};
