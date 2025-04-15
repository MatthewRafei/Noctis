#ifndef TYPES_H
#define TYPES_H

// https://github.com/malloc-nbytes/Score/blob/toD/src/types.d

enum Noctis_Type_Kind {
  NOCTIS_TYPE_KIND_INT,
  NOCTIS_TYPE_KIND_STR,
  NOCTIS_TYPE_KIND_CHAR,
  NOCTIS_TYPE_KIND_PTR,
  NOCTIS_TYPE_KIND_VOID,
};

struct Noctis_Type {
  enum Noctis_Type_Kind kind;
  size_t sz;
};

struct Noctis_Type_Int {
  struct Noctis_Type base;
};

struct Noctis_Type_Char {
  struct Noctis_Type base;
};

struct Noctis_Type_String {
  struct Noctis_Type base;
};

struct Noctis_Type_Void {
  struct Noctis_Type base;
};

struct Noctis_Type_Ptr {
  struct Noctis_Type base;
  struct Noctis_Type *to;
};

struct Noctis_Type *noctis_type_alloc(enum Noctis_Type_Kind kind);

#endif // TYPES_H
