#ifndef TYPES_H
#define TYPES_H

// https://github.com/malloc-nbytes/Score/blob/toD/src/types.d

enum Noctis_Base_Type {
    NOCTIS_TYPE_INT,
    NOCTIS_TYPE_STR,
    NOCTIS_TYPE_CHAR,
    NOCTIS_TYPE_PTR,
    NOCTIS_TYPE_VOID,
};

struct Noctis_Type_Char {
  char character;
  // DEBUGING 
  int len;
  int owned;
  int mut;
};

struct Noctis_Type_Int {
  int integer;
  int mut;
};

struct Noctis_Type_Str {
  char *string;
  int len;
  int owned;
  int mut;
};

struct Noctis_Type {
    enum Noctis_Base_Type base;
    struct Noctis_Type *next;
};

#endif // TYPES_H
