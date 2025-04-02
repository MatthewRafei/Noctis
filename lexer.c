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
    ";",  "&",           // TOKEN_SCOLON, TOKEN_AND
    ">=", "<=",          // TOKEN_GE, TOKEN_LE
    "~=", "==",          // TOKEN_NE, TOKEN_EQ
    "=",                 // TOKEN_ASSIGN
    ">>", "<<",          // TOKEN_GTGT, TOKEN_LTLT
    "->",                // TOKEN_ARROW
  };

  for (size_t i = 0; i < (sizeof(syms) / sizeof(*syms)); ++i) {
    enum Token_Type token = (enum Token_Type)i;
    s_umap_insert(&tbl, syms[i], (void*)&token);
  }

  // Keywords
  char *kws[] = KEYWORD_ASCPL;
  
  /// DEBUG PRINT
  // maybe we move debug printing to utils.c
  for (size_t i = 0; i < (sizeof(syms)/sizeof(*syms)); ++i) {
    printf("Symbol: %s -> Token: %d\n",
	   syms[i],
	   *(enum Token_Type *)s_umap_get(&tbl, syms[i]));
  }
  printf("\n");
  /// DEBUG PRINT END
  
  assert(sizeof(kws)/sizeof(*kws) == (TOKEN_KEYWORD_LEN - TOKEN_SYMBOL_LEN) - 1);

  for (size_t i = 0; i < sizeof(kws)/sizeof(*kws); ++i) {
    /*
      Because s_umap_insert() takes void* as the value, we need to create a variable of 
      the appropriate Token_Type to match the keyword.

      The addition of `+1` and the `i + (int)TOKEN_SYMBOL_LEN` ensures that we're 
      correctly indexing into the Token_Type enum for the corresponding keyword.
    */
    enum Token_Type token = (enum Token_Type)(i + (int)TOKEN_SYMBOL_LEN) + 1;
    s_umap_insert(&tbl, kws[i], (void*)&token);
  }

  /// DEBUG PRINT
  for (size_t i = 0; i < sizeof(kws)/sizeof(*kws); ++i) {
    printf("Keyword: %s -> Token: %d\n",
           kws[i],
           *(enum Token_Type *)s_umap_get(&tbl, kws[i])
    );
  }
  printf("\n");
  /// DEBUG PRINT END
  
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
  // We should be checking to see how big the lexer is lol
  l->size += 1;
}

void lexer_dump(const struct Lexer *l)
{
  struct Token *node = l->hd;
  while(node){
    token_dump(node);
    node = node->next;
  }
}

/*
  consume_while returns a counter to the number of indexes
  we need to skip ahead until the while loop is broken by the predicate.
  Predicate being a function pointer that
  returns a character, escape seq, or something we want to stop at
*/
// Could potentially be replaced with strspn if source files ever get so big this becomes a performance issue.
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

// might not fully capture cases like <=, >=, or ==.
// You may want a specialized predicate for operators instead of relying on not_sym.
int not_sym(int c)
{
  return !isalnum(c) && c != '_' && c != ' ' && c != '\n' && c != '\t' && c != '\r';
}

int is_identifier(int c)
{
  return isalnum(c) || c == '_';
}


enum Token_Type *determine_symbol(const char *s,
				  size_t op_len,
				  struct S_Umap *sym_keyword_tbl,
				  size_t *actual_len)
{
  *actual_len = 0;

  // Fix this with dynamically allocated 
  char buf[256] = {0};

  if(op_len < sizeof(buf)){
    strncpy(buf, s, op_len);
    // just incase
    buf[op_len] = '\0';
  }
  else{
    //Error out properly when you can 
    printf("Error at determine_symbol\n");
    exit(1);
  }
  
  for (int i = (int)op_len-1; i >= 0; --i) {
    if (s_umap_contains(sym_keyword_tbl, buf)) {
      *actual_len = (size_t)i+1;
      return (enum Token_Type *)s_umap_get(sym_keyword_tbl, buf);
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
      i += 1;
      row += 1;
      col = 1;
    }

    // Multi-line comment
    else if(ch == '(' && SAFE_PEEK(src, i+1, '*')){
      size_t counter = 0, r_counter = 0, c_counter = 0;
      while(src[i + counter] && !(src[i+counter] == '*' && src[i+counter+1] == ')')){

        if(src[i+counter] == '\n' || src[i+counter] == '\r'){
            r_counter += 1;
            c_counter = 0;
        }
    
        counter += 1;
        c_counter += 1;

        if(src[i+counter] == '(' && src[i+counter+1] == '*'){
          printf("Nested multi-comment at file: \"%s\" on row: %ld col: %ld\n", fp, row, col);
          exit(1);
        }
      }
      // What is string is not Buffer Null-Termination?
      if(!src[i+counter]){
        printf("unterminated comment at file: \"%s\" on row: %ld col: %ld\n", fp, row, col);
        exit(1);
      }

      // 2 because we need to skip '*)'
      i += counter + 2;
      row += r_counter;
      col = 1 + c_counter;
    }

    // Single-line comment
    else if(ch == '-' && SAFE_PEEK(src, i+1, '-')){
      size_t len = consume_while(src + i, not_newline);
      // No need to update to row
      // because skip newline will take care of it
      i += len;
      col = 1;
    }

    // Identifiers and keywords
    else if(isalpha(ch) || ch == '_'){
      size_t len = consume_while(src+i, is_identifier);

      // Use len to create a substring to search
      // for keywords in the hash table
      //char substring[len + 1];
      char *substring = s_malloc(len+1);
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

      free(substring);
    }

    // TODO: Make it so that character literals are their own thing.
    // TODO: Escape sequences in strings?
    else if (ch == '"' || ch == '\'') {

      i += 1, col += 1; // " or '

      size_t len = consume_while(src+i, not_quote);

      // Ensure we actually found the closing quote
      // You reference closing_quote in the string literal parsing, but it's not defined.
      // You should ensure that you compare against ch, which holds the opening quote:
      if (src[i + len] != ch) {
        printf("Unterminated string at file: \"%s\" on row: %ld col: %ld\n", fp, row, col);
        exit(1);
      }

      struct Token *t = token_alloc(TOKEN_STRING_LIT, src+i, len, fp, row, col);

      lexer_append(&lexer, t);

      // " or ' + length of string
      i += len + 1;
      col += len + 1; 
    }
    ////////////////////////////////////////////////////////////////////////////////////
    // Numbers & for loop ranges
    else if(isdigit(ch)){
      size_t len = consume_while(src+i, isdigit);
      if (SAFE_PEEK(src, i + len, '.') && !SAFE_PEEK(src, i + len + 1, '.')) {
        // We found a decimal point followed by a digit -> It's a float!
        len += 1;  // Account for the decimal point
        len += consume_while(src + i + len, isdigit);  // Consume decimal part
        struct Token *t = token_alloc(TOKEN_FLOAT_LIT, src + i, len, fp, row, col);
        lexer_append(&lexer, t);
        i += len;
        col += len;
      }
      else{
        struct Token *t = token_alloc(TOKEN_INT_LIT, src+i, len, fp, row, col);
        lexer_append(&lexer, t);
        i += len;
        col += len;
      }
    }

    else if(ch == '-' && SAFE_PEEK(src, i+1, '>')){
      struct Token *t = token_alloc(TOKEN_ARROW, src+i, 2, fp, row, col);
      lexer_append(&lexer, t);
      i += 2;
      col += 2;
    }

    // How the h*** is the arrow operator hitting this condition?
    // Added the conditional above this one so it would hit first....
    // But Im keeping my eye on you lol
    else if(ch == '.' && SAFE_PEEK(src, i+1, '.')){
      struct Token *t = token_alloc(TOKEN_FOR_RANGE, src+i, 2, fp, row, col);
      lexer_append(&lexer, t);
      i += 2;
      col += 2;
    }

    else if(ch == '.' && src[i+1] && isdigit(src[i+1])){
      size_t len = consume_while(src+i+1, isdigit);
      struct Token *t = token_alloc(TOKEN_FLOAT_LIT, src+i, len, fp, row, col);
      lexer_append(&lexer, t);
      i += len;
      col += 2;
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Operators
    else {
      size_t op_len = consume_while(src + i, not_sym);
      size_t len = 0;
      enum Token_Type *type = determine_symbol(src+i, op_len, &sym_keyword_tbl, &len);

      if (!type) {
        err_wargs("invalid type at: %s\n", src + i);
      }

      struct Token *t = token_alloc(*type, src + i, len, fp, row, col);
      lexer_append(&lexer, t);

      i += len;
      col += len;
    }
  }
  // Free hash-table
  s_umap_free(&sym_keyword_tbl);
  return lexer;
}
