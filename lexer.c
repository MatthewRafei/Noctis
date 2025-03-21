#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "keywords.h"
#include "ds/s-umap.h"
#include "token.h"
#include "utils.h"
#include "fnv-1a.h"

static struct S_Umap init_sym_keyword_tbl(void) {
  struct S_Umap tbl = s_umap_create(fnv1a, sizeof(enum Token_Type));

  char *syms[] = {
    "(",  ")",           // TOKEN_LPAREN, TOKEN_RPAREN
    "{",  "}",           // TOKEN_LCURLY, TOKEN_RCURLY
    "[",  "]",           // TOKEN_LSQUAR, TOKEN_RSQUAR
    ",",  ".",           // TOKEN_COMMA, TOKEN_DOT
    "+",  "-",           // TOKEN_PLUS, TOKEN_MINUS
    "*",  "/",           // TOKEN_STAR, TOKEN_SLASH
    "^",  "%",           // TOKEN_CARET, TOKEN_MOD
    ":",  "?",           // TOKEN_COLON, TOKEN_QUESTION
    "~",                 // TOKEN_NOT
    ">",  "<",           // TOKEN_GT, TOKEN_LT
    ";", "&",          // TOKEN_SCOLON, TOKEN_AND
    ">=", "<=",          // TOKEN_GE, TOKEN_LE
    "~=", "=",           // TOKEN_NE, TOKEN_EQ
    ":=",                // TOKEN_ASSIGN
    ">>", "<<",          // TOKEN_GTGT, TOKEN_LTLT          
  };

  for (size_t i = 0; i < (sizeof(syms)/sizeof(*syms)); ++i) {
    s_umap_insert(&tbl, syms[i], (void*)&(enum Token_Type){(enum Token_Type)i});
  }
  
  // Keywords
  char *kws[] = KEYWORD_ASCPL;

  /*
    for (size_t i = 0; i < (sizeof(syms)/sizeof(*syms)); ++i) {
    printf("Symbol: %s -> Token: %d\n", syms[i], *(enum Token_Type *)s_umap_get(&tbl, syms[i]));
    }
    for (size_t i = 0; i < sizeof(kws)/sizeof(*kws); ++i) {
    printf("Keyword: %s -> Token: %d\n", kws[i], *(enum Token_Type *)s_umap_get(&tbl, kws[i]));
    }
  */
  
  assert(sizeof(kws)/sizeof(*kws) == (TOKEN_KEYWORD_LEN - TOKEN_SYMBOL_LEN) - 1);
  
  for (size_t i = 0; i < sizeof(kws)/sizeof(*kws); ++i) {
    /*
      Because s_umap_insert() takes void * as the value, we need to
      get the appropriate Token_Type that matches up with the appropriate
      keyword (see keywords.h) and making a variable in-place and taking
      the address of it to pass to s_umap_insert.
      The i+(int)TOKEN_SYMBOL_LEN+1 is for making
      sure we are "indexing" the Token_Type enum correctly.
    */
    // Might be compiler specific because of compound literal lifetimes
    s_umap_insert(&tbl, kws[i], (void*)&(enum Token_Type){(enum Token_Type)(i+(int)TOKEN_SYMBOL_LEN+1)});
  }
  
  return tbl;
}

static void lexer_append(struct Lexer *l, struct Token *t)
{
  if (!l->hd || !l->tl) {
    l->hd = l->tl = t;
  }
  else {
    l->tl->next = t;
    l->tl = l->tl->next;
  }
}

void lexer_dump(const struct Lexer *l)
{
  struct Token *node = l->hd;
  while(node){
    token_dump(node);
    node = node->next;
  }
}

// consume_while returns a counter to the number of indexes
// we need to skip ahead until the while loop is broken by the predicate.
// Predicate being a function pointer.
size_t consume_while(char *s, int (*pred)(int))
{
  size_t counter = 0;
  while(s[counter] && pred(s[counter])){
    counter += 1;
  }
  return counter;
}

// Example predicate
int not_newline(int c) 
{
  return (char)c != '\n' && (char)c != '\r';
}

int not_RPAREN(int c)
{
  return (char)c != ')';
}

int not_whitespace(int c)
{
  return (char)c != ' ';
}

int not_sym(int c)
{
  return !isalnum(c) && c != '_' && c != ' ' && c != '\n' && c != '\t' && c != '\r';
}

int is_identifier(int c)
{
  return isalnum(c) || c == '_';
}

enum Token_Type *determine_symbol(const char *s, size_t op_len, struct S_Umap *sym_keyword_tbl, size_t *actual_len)
{
  *actual_len = 0;

  // Fix this with dynamically allocated 
  char buf[256] = {0};
  strncpy(buf, s, op_len);
  
  for (int i = (int)op_len-1; i >= 0; --i) {
    if (s_umap_contains(sym_keyword_tbl, buf)) {
      *actual_len = (size_t)i+1;
      return s_umap_get(sym_keyword_tbl, buf);
    }
    buf[i] = '\0';
  }
  return (enum Token_Type *)NULL;
}

int not_quote(int c)
{
  return c != '"' && c != '\'';
}

struct Lexer lex_file(char *src, const char *fp)
{
  struct S_Umap sym_keyword_tbl = init_sym_keyword_tbl();

  struct Lexer lexer = (struct Lexer) {
    .hd = NULL,
    .tl = NULL,
  };

  size_t i = 0, row = 1, col = 1;
  while(src[i] != '\0'){
    char ch = src[i];

    // Skip whitespace and tabs
    if(ch == ' ' || ch == '\t') {
      i += 1, col += 1;
    }

    // Skip newline and carriage return
    else if(ch == '\n' || ch == '\r') {
      i += 1, row += 1, col = 1;
    }

    // Multi-line comment
    else if(ch == '(' && SAFE_PEEK(src, i+1, '*')){

      size_t counter = 0, r_counter = 0, c_counter = 0;

      while(src[i+counter] && src[i+counter] != ')'){
        if(src[i+counter] == '\n' || src[i+counter] == '\r'){
          r_counter += 1;
          c_counter = 0;
        }
        counter += 1;
        c_counter += 1;
      }

      // 1 because we need to skip ')'
      i += counter + 1;
      row += r_counter;
      col = 1 + c_counter;
    }

    // Single-line comment
    else if(ch == '-' && SAFE_PEEK(src, i+1, '-')){
      size_t len = consume_while(src + i, not_newline);
      // No need to update to row
      // because skip newline will take care of it
      i += len, col = 1;
    }

    // Identifiers and keywords
    else if(isalpha(ch) || ch == '_'){
      size_t len = consume_while(src + i, is_identifier);

      // Use len to create a substring to search
      // for keywords in the hash table
      char substring[len + 1];
      memcpy(substring, src + i, len);
      substring[len] = '\0';

      enum Token_Type *type = (enum Token_Type *)s_umap_get(&sym_keyword_tbl, substring);

      if (type != NULL) {
        struct Token *t = token_alloc(*type, src+i, len, fp, row, col);
        lexer_append(&lexer, t);
        i += len, col += len;
      }
      else{
        struct Token *t = token_alloc(TOKEN_IDENTIFIER, src+i, len, fp, row, col);
        lexer_append(&lexer, t);
        i += len, col += len;
      }
    }

    // TODO: Make it so that character literals are their own thing.
    else if (ch == '"' || ch == '\'') {
      i += 1, col += 1; // " or '
      size_t len = consume_while(src+i, not_quote);
      struct Token *t = token_alloc(TOKEN_STRING_LIT, src+i, len, fp, row, col);
      lexer_append(&lexer, t);
      i += len + 1, col += len + 1; // " or ' + length of string
    }

    // Numbers (floats unimplemented)
    else if(isdigit(ch)) {
      size_t len = consume_while(src + i, isdigit);

      struct Token *t = token_alloc(TOKEN_INTEGER, src+i, len, fp, row, col);
      lexer_append(&lexer, t);
      i += len, col += len;
    }

    // Operators
    else {
      size_t op_len = consume_while(src + i, not_sym);
      size_t len = 0;
      enum Token_Type *type = determine_symbol(src+i, op_len, &sym_keyword_tbl, &len);

      if (!type) {
        err_wargs("invalid type at: %s", src+i);
      }

      struct Token *t = token_alloc(*type, src + i, len, fp, row, col);
      lexer_append(&lexer, t);

      i += len, col += len;
    }
  }
  return lexer;
}
