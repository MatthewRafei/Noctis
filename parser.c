#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "token.h"

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

int expect(struct Lexer *l, size_t current, enum Token_Type exp_typ)
{
  struct Token *t = l->hd;
  if(current <= l->size){
    for(size_t i = 0; i < current; i++){
      t = t->next;
    }
    // Go one ahead
    t = t->next;
  }

  if(t->type == exp_typ){
    return 1;
  }

  return 0; 
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



