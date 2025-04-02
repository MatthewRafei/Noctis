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


struct Token *advance(struct Lexer *l, size_t *current)
{
  struct Token *t = l->hd;
  if(*current <= l->size){
    for(size_t i = 0; i < *current; i++){
      t = t->next;
    }
  }
  // consume a token
  current += 1;
  return t;
}


struct Token_Type is_next(struct Lexer *l, size_t current)
{
  enum Token_Type t;
  if(current < l->size){
    
  }
  else{
    return NULL;
  }

  
}
