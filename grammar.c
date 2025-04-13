#include <stdio.h>
#include <assert.h>
#include "grammar.h"


//======================= PRINTERS

static void dump_stmt(struct Stmt *s, int padding);

static void spaces(int p) {
    for (int i = 0; i < p; ++i) printf("  ");
}

static void dump_stmt_let(struct Stmt_Let *s, int padding) {
    spaces(padding);
    printf("LET ");
    if(s->mut){
        printf("(MUT) ");
    }
    printf("%s: ", s->id->lexeme);
    printf("TYPE: (unimplemented) = ");
    printf("(expr unimplemented)");
    printf(";\n");
}

static void dump_stmt_func(struct Stmt_Func *s, int padding) {
    (void)s;
    (void)padding;
    assert(0);
}

static void dump_stmt_while(struct Stmt_While *s, int padding) {
    (void)s;
    (void)padding;
    assert(0);
}

static void dump_stmt_for(struct Stmt_For *s, int padding) {
    (void)s;
    (void)padding;
    assert(0);
}

static void dump_stmt_block(struct Stmt_Block *s, int padding) {
    for (size_t i = 0; i < s->len; ++i) {
        dump_stmt(s->stmts[i], padding);
    }
}

static void dump_stmt(struct Stmt *s, int padding) {
    printf("STMT: %d\n", (int)s->type);
    switch (s->type) {
        case STMT_TYPE_LET: dump_stmt_let((struct Stmt_Let *)s, padding); break;
        case STMT_TYPE_FUNC: break;
        case STMT_TYPE_WHILE: break;
        case STMT_TYPE_FOR: break;
        case STMT_TYPE_BLOCK: break;
        default: assert(0 && "unhandled statement");
    }
}

void dump_program(struct Program *p) {
    (void)dump_stmt_block;
    (void)dump_stmt_let;
    (void)dump_stmt_for;
    (void)dump_stmt_while;
    (void)dump_stmt_func;
    for (size_t i = 0; i < p->len; ++i) {
        dump_stmt(p->stmts[i], 0);
    }
}

//======================= ALLOCATORS

struct Stmt *stmt_alloc(enum Stmt_Type type)
{
    switch (type) {
        case STMT_TYPE_LET: {
            struct Stmt_Let *s = malloc(sizeof(struct Stmt_Let));
            s->base.type = STMT_TYPE_LET;
            return (struct Stmt *)s;
            
        } break;
        case STMT_TYPE_BLOCK: {
            assert(0);
        } break;
        default: {
            assert(0 && "unhandled statement");
        } break;
    }
    assert(0 && "unreachable");
    return NULL;
}