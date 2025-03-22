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
struct Stmt;

///////////////////
// Expressions
///////////////////

struct Expr_Mut {
  int i;
};

struct Expr_Identifier {
  int i;
};

struct Expr_Int_Lit {
  int i;
};
struct Expr_Str_Lit {
  int i;
};
struct Expr_Unary {
  int i;
};
struct Expr_Binary {
  int i;
};

struct Expr {
    enum Expr_Type type;
};

///////////////////
// Statements
///////////////////

struct Stmt_Block {
  int i;
};
struct Stmt_For {
  int i;
};
struct Stmt_While {
  int i;
};
struct Stmt_Func {
  int i;
};

struct Stmt_Let {
    struct Stmt base;
    int mut;
    struct Token *id;
    struct Noctis_Type *type;
    struct Expr *expr;
};

struct Stmt {
    enum Stmt_Type type;
};

#endif // GRAMMAR_H
