#ifndef PARSER_H
#define PARSER_H

// struct Parser {
//   struct Lexer lexer;
//   size_t lexer_length;
//   size_t current_token;
// };

struct Program *parse_program(struct Lexer *l);

#endif