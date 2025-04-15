#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "token.h"
#include "grammar.h"
#include "utils.h"

#define safe_peek(l, n) peek(l, n) && peek(l, n) // they both have to be true?

// Returns token at param current index
struct Token *peek(struct Lexer *l, size_t index)
{
  struct Token *t = l->hd;
  if(index <= l->size){
    for(size_t i = 0; i < index; i++){
      t = t->next;
    }
  }
  return t;
}

// Discard token in cases of unneeded keywords
void discard(struct Lexer *l) {
  assert(l->hd);
  l->hd = l->hd->next;
}

struct Token *expect(struct Lexer *l, enum Token_Type exp) {
  if (!l->hd) {
    err("expect: hd is null");
  }

  struct Token *t = l->hd;
  if (t->type != exp) {
    err_wargs("Expected %s but got %s", enum_to_str(exp), enum_to_str(t->type));
  }

  l->hd = l->hd->next;
  return t;
}

struct Token *advance(struct Lexer *l)
{
  assert(l->hd);
  struct Token *t = l->hd;
  l->hd = l->hd->next;
  return t;
}

struct Expr *parse_primary_expr(struct Lexer *l)
{
  (void)l;
  TODO;
}

struct Expr *parse_unary_expr(struct Lexer *l)
{
  (void)l;
  TODO;
}

struct Expr *parse_multiplicative_expr(struct Lexer *l)
{
  (void)l;
  TODO;
}

struct Expr *parse_additive_expr(struct Lexer *l)
{
  (void)l;
  TODO;
}

struct Expr *parse_equalitive_expr(struct Lexer *l)
{
  (void)l;
  TODO;
}

struct Expr *parse_logical_expr(struct Lexer *l)
{
  (void)l;
  TODO;
}

struct Expr *parse_assignment_expr(struct Lexer *l)
{
  (void)l;
  TODO;
}

struct Expr *parse_expr(struct Lexer *l)
{
  return parse_assignment_expr(l);
}

//let x: ptr int = &add

struct Noctis_Type *parse_type(struct Lexer *l)
{
  struct Token *cur = advance(l);
  switch (cur->type) {
    case TOKEN_INT: return noctis_type_alloc(NOCTIS_TYPE_KIND_INT);
    case TOKEN_FLOAT: TODO;
    default: assert(0 && "unhandled type in parse_type");
  }
}

struct Stmt_Let *parse_let(struct Lexer *l)
{
  int mut                  = 0;
  struct Token *id         = NULL;
  struct Noctis_Type *type = NULL;
  struct Expr *e           = NULL;

  discard(l); // let

  mut = safe_peek(l, 0)->type == TOKEN_MUT;
  if (mut) {
    discard(l); // mut
  }

  id = expect(l, TOKEN_IDENTIFIER);
  (void)expect(l, TOKEN_COLON);

  type = parse_type(l);
  (void)expect(l, TOKEN_ASSIGN);

  e = parse_expr(l);
  (void)expect(l, TOKEN_SCOLON);

  struct Stmt_Let *s = (struct Stmt_Let *)stmt_alloc(STMT_TYPE_LET);

  s->mut = mut;
  s->id = id;
  s->type = type;
  s->expr = e;
  return s;
}

struct Stmt *parse_stmt(struct Lexer *l)
{
  switch (safe_peek(l, 0)->type) {
    case TOKEN_LET: return (struct Stmt *)parse_let(l);
    case TOKEN_FUNC: TODO;
    case TOKEN_LCURLY: TODO;
    default: assert(0 && "unhandled statement in parse_stmt");
  }

  return NULL; // unreachable
}

struct Program *parse_program(struct Lexer *l)
{
  struct Program *p = malloc(sizeof(struct Program));
  p->stmts = NULL;
  p->len = 0, p->cap = 0;

  while (safe_peek(l, 0) != NULL) {
    struct Stmt *s = parse_stmt(l);
    da_append(p->stmts, p->len, p->cap, s);
  }

  return p;
}
