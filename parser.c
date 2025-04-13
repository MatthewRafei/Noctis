#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "token.h"
#include "grammar.h"


struct Token *peek(struct Lexer *l, size_t current)
{
  struct Token *t = l->hd;
  if(current <= l->size){
    for(size_t i = 0; i < current; i++){
      t = t->next;
    }
  }
  return t;
}

// consume a token
struct Token *advance(struct Lexer *l, size_t *current)
{
  struct Token *t = l->hd;
  if(*current <= l->size){
    for(size_t i = 0; i < *current; i++){
      t = t->next;
    }
  }
  current += 1;
  return t;
}

void discard(struct Lexer *l) {
  assert(l->hd);
  l->hd = l->hd->next;
}

enum Token_Type is_next(struct Lexer *l, size_t current)
{
  enum Token_Type tt;
  struct Token *t = l->hd;
  if(current+1 <= l->size){
    for(size_t i = 0; i <= current; i++){
      t = t->next;
    }
    tt = t->type;
  }
  else{
    printf("Current + 1 is greater than lexer size\n");
    exit(1);
  }
  return tt;
}

// int expect(struct Lexer *l, size_t current, enum Token_Type exp_typ)
// {
//   struct Token *t = l->hd;
//   if(current <= l->size){
//     for(size_t i = 0; i < current; i++){
//       t = t->next;
//     }
//     // Go one ahead
//     t = t->next;
//   }

//   if(t->type == exp_typ){
//     return 1;
//   }

//   return 0;
// }

struct Token *expect(struct Lexer *l, enum Token_Type exp) {
  if (!l->hd) {
    // TODO: error here
    assert(0);
  }

  struct Token *t = l->hd;
  if (t->type != exp) {
    // TODO: error here
    printf("Expected %s but got %s\n", enum_to_str(exp), enum_to_str(t->type));
    assert(0);
  }

  l->hd = l->hd->next;
  return t;
}

struct Token *previous_token(struct Lexer *l, size_t current)
{
  current = current - 1;
  struct Token *t = l->hd;
  if(current <= l->size){
    for(size_t i = 0; i < current; i++){
      t = t->next;
    }
  }
  return t;
}

// Consumes a token
int match(struct Lexer *l, size_t *current, enum Token_Type exp_typ)
{
  struct Token *t = l->hd;
  
  if(*current <= l->size){
    for(size_t i = 0; i < *current; i++){
      t = t->next;
    }
  }
  
  if(t->type == exp_typ){
    return 1;
  }
  return 0;
}

struct Noctis_Type *parse_type(struct Lexer *l) {
  (void)l;
  assert(0);
}

// ["let", "mut", "x", ":", "int", "=", "1", "+", "2", ";"]

struct Stmt_Let *parse_let(struct Lexer *l)
{
  int mut = 0;
  struct Token *id = NULL;
  struct Noctis_Type *type = NULL;
  struct Expr *e = NULL;

  discard(l); // let
  mut = peek(l, 0)->type == TOKEN_MUT;
  if (mut) {
    discard(l); // mut
  }
  id = expect(l, TOKEN_IDENTIFIER);
  (void)expect(l, TOKEN_COLON);
  //type = parse_type(l); // TODO: handle type parsing
  discard(l); // TODO:REMOVEME: handle type parsing
  (void)expect(l, TOKEN_ASSIGN);
  // e = parse_expr(l); // TODO: handle expression parsing
  discard(l); // TODO:REMOVEME: handle expression parsing
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
  if (l->hd->type == TOKEN_LET) {
    struct Stmt *s = (struct Stmt *)parse_let(l);
    return s;
  }
  assert(0);
  return NULL;
}

struct Program *parse_program(struct Lexer *l)
{
  struct Program *p = malloc(sizeof(struct Program));
  // QAD fix later
  p->stmts = malloc(sizeof(struct Stmt) * 8);
  p->len = 0, p->cap = 8;
  
  while (peek(l, 0) != NULL) {
    if (p->len != 0)
      printf("HERE: %d\n", (int)p->stmts[0]->type);
    struct Stmt *s = parse_stmt(l);
    p->stmts[p->len++] = s;
  }

  return p;
}

