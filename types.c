#include <assert.h>
#include <stdlib.h>

#include "types.h"
#include "utils.h"

struct Noctis_Type *noctis_type_alloc(enum Noctis_Type_Kind kind)
{
  switch (kind) {
  case NOCTIS_TYPE_KIND_INT: return malloc(sizeof(struct Noctis_Type_Int));
  case NOCTIS_TYPE_KIND_FLOAT: return malloc(sizeof(struct Noctis_Type_Float));
  /* case NOCTIS_TYPE_KIND_STR: TODO; */
  /* case NOCTIS_TYPE_KIND_CHAR: TODO; */
  /* case NOCTIS_TYPE_KIND_PTR: TODO; */
  /* case NOCTIS_TYPE_KIND_VOID: TODO; */
  default: assert(0 && "unhandled type in noctis_type_alloc");
  }
  return NULL;
}
