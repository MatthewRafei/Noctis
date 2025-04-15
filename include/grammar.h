#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "token.h"
#include "types.h"

enum Expr_Type {
  EXPR_TYPE_BIN,
  EXPR_TYPE_UNARY,
  EXPR_TYPE_INT_LIT,
  EXPR_TYPE_STR_LIT,
  EXPR_TYPE_IDENTIFIER,
  EXPR_TYPE_MUT,
  EXPR_TYPE_RANGE,
};

enum Stmt_Type {
  STMT_TYPE_LET,
  STMT_TYPE_FUNC,
  STMT_TYPE_WHILE,
  STMT_TYPE_FOR,
  STMT_TYPE_BLOCK,
};

///////////////////
// Forward Decls.
///////////////////

struct Expr_Range;
struct Expr_Mut;
struct Expr_Identifier;
struct Expr_Int_Lit;
struct Expr_Str_Lit;
struct Expr_Unary;
struct Expr_Binary;
struct Expr;

struct Stmt_Block;
struct Stmt_For;
struct Stmt_While;
struct Stmt_Func;
struct Stmt_Let;
struct Stmt_Func;
struct Stmt;

///////////////////
// Expressions
///////////////////

struct Expr_Mut {
  int bool;
};

struct Expr_Identifier {
  char *name;
};

struct Expr_Int_Lit {
  int value;
};

struct Expr_Str_Lit {
  char *string;
  int len;
};

struct Expr_Unary {
  int i;
};

struct Expr_Binary {
  int i;
};

struct Expr_Range {
  int i;
};

struct Expr {
  enum Expr_Type type;
};

///////////////////
// Statements
///////////////////

struct Stmt {
  enum Stmt_Type type;
};

struct Stmt_Block {
  struct Stmt base;
  struct Stmt **stmts;
  size_t len, cap;
};

struct Stmt_For {
  struct Stmt base;
  struct Token *enumerator_id;
  struct Expr_Range range;
  struct Stmt_Block *block;
};

struct Stmt_While {
  struct Stmt base;
  struct Expr *expression;
  struct Stmt_Block *block;
};

struct Stmt_Let {
  struct Stmt base;
  int mut;
  struct Token *id;
  struct Noctis_Type *type;
  struct Expr *expr;
};

struct Stmt_Func {
  struct Stmt base;
  int is_export;
  struct Token *id;
  struct Token **params;
  struct Noctis_Type **param_types;
  struct Noctis_Type *return_type;
  struct Stmt_Block *block;
};

// Root node of AST
struct Program {
  struct Stmt **stmts;
  size_t len, cap;
};

void dump_program(struct Program *p);
struct Stmt *stmt_alloc(enum Stmt_Type type);

#endif // GRAMMAR_H
