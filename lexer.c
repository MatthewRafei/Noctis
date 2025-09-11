#include "context.h"
#include "diagnostic.h"
#include "ds/s_umap.h"
#include "fnv-1a.h"
#include "keywords.h"
#include "lexer.h"
#include "token.h"
#include "utils.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
TODO(malac0da):

- Add more unit tests for edge cases (unterminated strings, comments, etc.).

- Fix floating point detection logic, you can use below text for sample input to see logic errors
1..2
3...4
5..  6

- Fix ambiguous operator logic, use this evil operator prefix to find errors
= == === ==== =>

- Support escape sequences in string and character literals.
- Make character literals distinct from string literals.

- Benchmark and possibly replace consume_while with strspn for performance.
- Consider dynamic allocation for buffers (e.g., in determine_symbol).
*/
/*
- Common escape sequences to support:
\n: Represents a newline character.
\t: Represents a horizontal tab character.
\r: Represents a carriage return character.
\\: Represents a literal backslash character.
\": Represents a literal double quote character within a string enclosed in double quotes. 
\': Represents a literal single quote character within a string.
\b: Represents a backspace character.
\f: Represents a form feed character.
\v: Represents a vertical tab character.
*/

struct S_Umap init_sym_keyword_tbl(struct CompilerContext *context)
{
    struct S_Umap tbl = s_umap_create(fnv1a, sizeof(enum Token_Type), context);

    if (!tbl.tbl.nodes) {
        report_error(FATAL, "Failed to create symbol/keyword table in lexer.\n", context);
        return (struct S_Umap) { 0 };
    }

    char *syms[] = {
        "(", ")",               // TOKEN_LPAREN, TOKEN_RPAREN
        "{", "}",               // TOKEN_LCURLY, TOKEN_RCURLY
        "[", "]",               // TOKEN_LSQUAR, TOKEN_RSQUAR
        ",", ".",               // TOKEN_COMMA, TOKEN_DOT
        "+", "-",               // TOKEN_PLUS, TOKEN_MINUS
        "*", "/",               // TOKEN_STAR, TOKEN_SLASH
        "^", "%",               // TOKEN_CARET, TOKEN_MOD
        ":", "?",               // TOKEN_COLON, TOKEN_QUESTION
        "!",                    // TOKEN_NOT
        ">", "<",               // TOKEN_GT, TOKEN_LT
        ";", "&",               // TOKEN_SCOLON, TOKEN_AND
        ">=", "<=",             // TOKEN_GE, TOKEN_LE
        "~=", "==",             // TOKEN_NE, TOKEN_EQ
        "=", "!=",              // TOKEN_ASSIGN, TOKEN_NOTEQ
        ">>", "<<",             // TOKEN_GTGT, TOKEN_LTLT
        "->",                   // TOKEN_ARROW
    };

    for (size_t i = 0; i < (sizeof(syms) / sizeof(*syms)); ++i) {
        enum Token_Type token = (enum Token_Type) i;
        s_umap_insert(&tbl, syms[i], (void *) &token);
    }

    // Keywords
    char *kws[] = KEYWORD_ASCPL;

    // If this fails its probably because you removed something in either keywords.h, token.h, or token.c
    // And did not make that change across all those three files
    assert(sizeof(kws) / sizeof(*kws) == (TOKEN_KEYWORD_LEN - TOKEN_SYMBOL_LEN) - 1);

    for (size_t i = 0; i < sizeof(kws) / sizeof(*kws); ++i) {
        /*
           Because s_umap_insert() takes void* as the value, we need to create a variable of 
           the appropriate Token_Type to match the keyword.

           The addition of `+1` and the `i + (int)TOKEN_SYMBOL_LEN` ensures that we're 
           correctly indexing into the Token_Type enum for the corresponding keyword.
         */
        enum Token_Type token = (enum Token_Type) (i + TOKEN_SYMBOL_LEN) + 1;
        s_umap_insert(&tbl, kws[i], (void *) &token);
    }

    // TODO(malac0da): Please move this to a debug or find a way to only run functions with
    printf("\nThere are %zu symbols in the language\n\n", (sizeof(syms) / sizeof(*syms)));
    for (size_t i = 0; i < (sizeof(syms) / sizeof(*syms)); i++) {
        printf("Symbol: %s -> Token: %d\n",
               syms[i], *(enum Token_Type *) s_umap_get(&tbl, syms[i]));
    }

    printf("\n");

    printf("\nThere are %zu keywords in the language\n\n", sizeof(kws) / sizeof(*kws));
    for (size_t i = 0; i < sizeof(kws) / sizeof(*kws); i++) {
        printf("Keyword: %s -> Token: %d\n", kws[i], *(enum Token_Type *) s_umap_get(&tbl, kws[i]));
    }

    return tbl;
}

//void print_keywords_and_symbols(char &syms,  )

static void lexer_append(struct Lexer *l, struct Token *t)
{
    if (!l->hd || !l->tl) {
        l->hd = l->tl = t;
    } else {
        l->tl->next = t;
        l->tl = l->tl->next;
    }
    l->size += 1;
}

void lexer_dump(const struct Lexer *l)
{
    struct Token *node = l->hd;
    while (node) {
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
    while (s[counter] && pred(s[counter])) {
        counter += 1;
    }
    return counter;
}

// Example predicate
int not_newline(int c)
{
    return (char) c != '\n' && (char) c != '\r';
}

int not_RPAREN(int c)
{
    return (char) c != ')';
}

int not_whitespace(int c)
{
    return (char) c != ' ';
}

// might not fully capture cases like <=, >=, or ==.
// May want a specialized predicate for operators instead of relying on not_sym.
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
                                  const struct S_Umap *sym_keyword_tbl, size_t *actual_len)
{
    *actual_len = 0;

    // TODO(malac0da): Fix this with dynamically allocated 
    char buf[256] = { 0 };

    if (op_len < sizeof(buf)) {
        strncpy(buf, s, op_len);
        // just incase
        buf[op_len] = '\0';
    } else {
        return NULL;
    }

    for (int i = (int)op_len - 1; i >= 0; --i) {
        if (s_umap_contains(sym_keyword_tbl, buf)) {
            *actual_len = (size_t) i + 1;
            return (enum Token_Type *) s_umap_get(sym_keyword_tbl, buf);
        }
        buf[i] = '\0';
    }
    return (enum Token_Type *) NULL;
}

int not_quote(int c)
{
    return c != '"' && c != '\'';
}

void lex_whitespace(struct Lexer_Pos *lexer_pos)
{
    lexer_pos->index += 1;
    lexer_pos->col += 1;
}

void lex_newline(struct Lexer_Pos *lexer_pos)
{
    lexer_pos->index += 1;
    lexer_pos->line += 1;
    lexer_pos->col = 1;
}

void lex_singleline_comment(struct Lexer_Pos *lexer_pos, char *src)
{
    size_t len = consume_while(src + lexer_pos->index, not_newline);
    lexer_pos->index += len;
    lexer_pos->col = 1;
}

void lex_multiline_comment(struct Lexer_Pos *lexer_pos, const char *src,
                           struct CompilerContext *context, struct Lexer *lexer)
{
    size_t counter = 2;         // start after "(*"
    size_t r_counter = 0;
    size_t c_counter = 2;       // column advanced we already consumed '(' and '*'

    while (src[lexer_pos->index + counter]) {
        char current_character = src[lexer_pos->index + counter];

        //chcek for closing "*)"
        if (current_character == '*' && SAFE_PEEK(src, lexer_pos->index + counter + 1, ')')) {
            break;
        }

        if (current_character == '\n' || current_character == '\r') {
            r_counter += 1;
            c_counter = 0;
        } else {
            c_counter += 1;
        }

        if (current_character == '(' && SAFE_PEEK(src, lexer_pos->index + counter + 1, '*')) {
            context->source.line = lexer_pos->line + r_counter;
            context->source.col = (r_counter == 0 ? lexer_pos->col + c_counter : c_counter + 1);
            report_error(FATAL, "Nested multi-line comments are not supported.\n", context);
            lexer->status = LEXER_ERROR;
            break;
        }

        counter += 1;
    }

    if (!src[lexer_pos->index + counter]) {
        context->source.line = lexer_pos->line;
        context->source.col = lexer_pos->col;
        report_error(FATAL, "Unterminated multi-line comment.\n", context);
        lexer->status = LEXER_ERROR;
    } else {
        // Skip closing "*)"
        counter += 2;
    }

    lexer_pos->index += counter;
    lexer_pos->line += r_counter;
    lexer_pos->col = (r_counter == 0 ? lexer_pos->col + c_counter : c_counter + 1);
}

void lex_id_key_sym(struct Lexer_Pos *lexer_pos, char *src, const struct S_Umap *sym_keyword_tbl,
                    const char *fp, struct Lexer *lexer)
{
    size_t len = consume_while(src + lexer_pos->index, is_identifier);

    char *substring = s_malloc(len + 1);
    memcpy(substring, src + lexer_pos->index, len);
    substring[len] = '\0';

    const enum Token_Type *type = (enum Token_Type *) s_umap_get(sym_keyword_tbl, substring);

    if (type) {
        struct Token *tok =
            token_alloc(*type, src + lexer_pos->index, len, fp, lexer_pos->line, lexer_pos->col);
        lexer_append(lexer, tok);
    } else {
        struct Token *tok =
            token_alloc(TOKEN_IDENTIFIER, src + lexer_pos->index, len, fp, lexer_pos->line,
                        lexer_pos->col);
        lexer_append(lexer, tok);
    }

    lexer_pos->index += len;
    lexer_pos->col += len;

    free(substring);
}

void lex_strings(struct Lexer_Pos *lexer_pos, char *src, struct CompilerContext *context,
                 struct Lexer *lexer, const char *fp)
{
    char opening_quote = src[lexer_pos->index];

    // Skip " or '
    lexer_pos->index += 1;
    lexer_pos->col += 1;

    size_t len = consume_while(src + lexer_pos->index, not_quote);

    // Check for closing quote by comparing it against opening quote
    if (src[lexer_pos->index + len] != opening_quote) {
        context->source.line = lexer_pos->line;
        context->source.col = lexer_pos->col - 1;       // Adjust column to point to opening quote
        report_error(FATAL, "Unterminated string literal.\n", context);
        lexer->status = LEXER_ERROR;
        //return lexer; // Should reconsider how errors are handled maybe...
        return;
    }

    struct Token *tok =
        token_alloc(TOKEN_STRING_LIT, src + lexer_pos->index, len, fp, lexer_pos->line,
                    lexer_pos->col);
    lexer_append(lexer, tok);

    // " or ' + length of string
    lexer_pos->index += len + 1;
    lexer_pos->col += len + 1;
}

// This doesnt feel quite right, cant put my finger on it.
void lex_numbers(struct Lexer_Pos *lexer_pos, char *src, const char *fp, struct Lexer *lexer)
{
    size_t len = consume_while(src + lexer_pos->index, isdigit);

    // We found a decimal point followed by a digit, a float.
    if (SAFE_PEEK(src, lexer_pos->index + len, '.')
        && !SAFE_PEEK(src, lexer_pos->index + len + 1, '.')) {
        len += 1;               // Account for decimal
        len += consume_while(src + lexer_pos->index + len, isdigit);    // Consume decimal part
        struct Token *tok =
            token_alloc(TOKEN_FLOAT_LIT, src + lexer_pos->index, len, fp, lexer_pos->line,
                        lexer_pos->col);
        lexer_append(lexer, tok);
        lexer_pos->index += len;
        lexer_pos->col += len;
    } else {
        struct Token *tok =
            token_alloc(TOKEN_INT_LIT, src + lexer_pos->index, len, fp, lexer_pos->line,
                        lexer_pos->col);
        lexer_append(lexer, tok);
        lexer_pos->index += len;
        lexer_pos->col += len;
    }
}

// This feels hacky
void lex_arrow(struct Lexer_Pos *lexer_pos, const char *src, const char *fp, struct Lexer *lexer)
{
    struct Token *tok =
        token_alloc(TOKEN_ARROW, src + lexer_pos->index, 2, fp, lexer_pos->line, lexer_pos->col);
    lexer_append(lexer, tok);
    lexer_pos->index += 2;
    lexer_pos->col += 2;
}

// This feels hacky
void lex_float(struct Lexer_Pos *lexer_pos, char *src, const char *fp, struct Lexer *lexer)
{
    size_t len = consume_while(src + lexer_pos->index + 1, isdigit);
    struct Token *tok =
        token_alloc(TOKEN_FLOAT_LIT, src + lexer_pos->index, len, fp, lexer_pos->line,
                    lexer_pos->col);
    lexer_append(lexer, tok);
    lexer_pos->index += len;
    lexer_pos->col += 2;        // Why 2?
}

// this code makes a lot of assumptions about people being smart.
// TODO(malac0da): Fix this code so that ambiguous operator logic doesnt occur
void lex_operators(struct Lexer_Pos *lexer_pos, char *src, struct S_Umap *sym_keyword_tbl,
                   struct CompilerContext *context, struct Lexer *lexer, const char *fp)
{
    size_t operator_len = consume_while(src + lexer_pos->index, not_sym);
    size_t len = 0;
    const enum Token_Type *type =
        determine_symbol(src + lexer_pos->index, operator_len, sym_keyword_tbl, &len);

    if (!type) {
        context->source.line = lexer_pos->line;
        context->source.col = lexer_pos->col;
        report_error(FATAL, "Invalid symbol.\n", context);
        lexer->status = LEXER_ERROR;
        s_umap_free(sym_keyword_tbl);
        return;
    }

    struct Token *tok =
        token_alloc(*type, src + lexer_pos->index, len, fp, lexer_pos->line, lexer_pos->col);
    lexer_append(lexer, tok);

    lexer_pos->index += len;
    lexer_pos->col += len;
}

// Rewrote lexer into many different funtions 
// TODO(malac0da): edge.fng triggers a bug
// TODO(malac0da): row and column tracking is now broken
struct Lexer lex_file(char *src, const char *fp, struct CompilerContext *context)
{
    struct Lexer lexer = (struct Lexer) {.hd = NULL,.tl = NULL, };

    lexer.status = LEXER_OK;
    context->source.file = fp;
    struct S_Umap sym_keyword_tbl = init_sym_keyword_tbl(context);

    if (!sym_keyword_tbl.tbl.nodes) {
        report_error(FATAL, "Failed to create symbol/keyword table in lexer.\n", context);
        s_umap_free(&sym_keyword_tbl);
        lexer.status = LEXER_ERROR;
        return lexer;
    }

    struct Lexer_Pos lexer_pos = (struct Lexer_Pos) {.index = 0,.line = 1,.col = 1 };

    while (src[lexer_pos.index] != '\0') {
        char current_char = src[lexer_pos.index];

        if (isspace(current_char)) {    // Whitespace (check tabs later)
            lex_whitespace(&lexer_pos);
        } else if (current_char == '\n' || current_char == '\r') {      // Newline
            lex_newline(&lexer_pos);
        } else if (current_char == '(' && SAFE_PEEK(src, lexer_pos.index + 1, '*')) {   // Multi-line comment
            lex_multiline_comment(&lexer_pos, src, context, &lexer);
        } else if (current_char == '-' && SAFE_PEEK(src, lexer_pos.index + 1, '-')) {   // Single-line comment
            lex_singleline_comment(&lexer_pos, src);
        } else if (isalpha(current_char) || current_char == '_') {      // Identifiers and keywords
            lex_id_key_sym(&lexer_pos, src, &sym_keyword_tbl, fp, &lexer);
        } else if (current_char == '"' || current_char == '\'') {       // Strings (Make chars seperate and add escape sequence support)
            lex_strings(&lexer_pos, src, context, &lexer, fp);
        } else if (isdigit(current_char)) {     // Numbers
            lex_numbers(&lexer_pos, src, fp, &lexer);
        } else if (current_char == '-' && SAFE_PEEK(src, lexer_pos.index + 1, '>')) {   // Arrow symbol
            lex_arrow(&lexer_pos, src, fp, &lexer);
        } else if (current_char == '.' && src[lexer_pos.index + 1] && isdigit(src[lexer_pos.index + 1])) {      // Floats that start with a dot
            lex_float(&lexer_pos, src, fp, &lexer);
        } else {
            lex_operators(&lexer_pos, src, &sym_keyword_tbl, context, &lexer, fp);
        }

    }

    s_umap_print(&sym_keyword_tbl, NULL);
    s_umap_free(&sym_keyword_tbl);
    return lexer;
}

void lexer_free(struct Lexer *l)
{
    struct Token *node = l->hd;
    while (node) {
        struct Token *n = node->next;
        if (node->lexeme) {
            free(node->lexeme);
        }
        free(node);
        node = n;
    }
    l->hd = l->tl = NULL;
    l->size = 0;
}

char *enum_lexer_status_to_str(enum Lexer_Status status)
{
    switch (status) {
        case LEXER_ERROR:
            return "ERROR";
        case LEXER_OK:
            return "PYRRHIC VICTORY";
        default:
            (void) fprintf(stderr, "You found a bug in the compiler. Please report it!\n");
            (void) fprintf(stderr, "Compiler bug happened in file: %s in function: %s.\n", __FILE__,
                           __func__);
            exit(EXIT_FAILURE);
    }

    return "Compiler bug happened in \"enum_lexer_status_to_str\" function. Please report it!\n";
}
