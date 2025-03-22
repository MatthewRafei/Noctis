#ifndef TYPES_H
#define TYPES_H

// You can possibly use bit shifting to improve performance

enum Noctis_Base_Type {
    NOCTIS_TYPE_INT,
    NOCTIS_TYPE_STR,
    NOCTIS_TYPE_CHAR,
    NOCTIS_TYPE_PTR,
    NOCTIS_TYPE_VOID,
};

struct Noctis_Type {
    enum Noctis_Base_Type base;
    struct Noctis_Type *next;
};

#endif // TYPES_H
