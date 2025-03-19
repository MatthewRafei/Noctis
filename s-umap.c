#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "s-umap.h"
#include "token.h"
#include "utils.h"
#include "fnv-1a.h"

#define INITAL_CAPACITY 256

struct S_Umap s_umap_create(s_umap_hash hash, size_t nodev_stride)
{
  assert(hash);

  struct S_Umap map = (struct S_Umap) {
    .tbl = {
      .nodes = (struct _S_Umap_Node **)calloc(INITAL_CAPACITY,
					      sizeof(struct _S_Umap_Node *)),
      .len = 0,
      .cap = INITAL_CAPACITY,
    },
    .hash = hash,
    .nodev_stride = nodev_stride,
  };

  return map;
}

struct _S_Umap_Node* _s_umap_node_create(char* key,
					 void *value,
					 size_t nodev_stride)
{
  struct _S_Umap_Node* node = (struct _S_Umap_Node *)s_malloc
    (sizeof(struct _S_Umap_Node));
  node->next = NULL;

  node->key = strdup(key);  

  node->value = s_malloc(nodev_stride);
  (void)memcpy((uint32_t *)node->value, (uint32_t *)value, nodev_stride);

  return node;
}

// Returns the node where its key == k.
// If no such node is found, returns NULL.
static struct _S_Umap_Node *list_has_key(const struct S_Umap *m,
                                         unsigned long idx,
                                         const char *k,
                                         struct _S_Umap_Node **prev)
{
  *prev = NULL;
  struct _S_Umap_Node *it = m->tbl.nodes[idx];
  while (it) {
    if (!strcmp(it->key, k)) {
      return it;
    }
    *prev = it;
    it = it->next;
  }
  return NULL;
}

static void node_free(struct _S_Umap_Node *n)
{
  free(n->key);
  free(n->value);
  free(n);
  n = NULL;
}

void s_umap_insert(struct S_Umap *map, char *key, void *value)
{
  struct _S_Umap_Node *node = _s_umap_node_create(key, value, map->nodev_stride);

  unsigned long idx = map->hash(node->key) % map->tbl.cap;

  struct _S_Umap_Node *prev = NULL;
  struct _S_Umap_Node *it = list_has_key(map, idx, key, &prev);

  if (!it) {
    struct _S_Umap_Node *tmp = map->tbl.nodes[idx];
    map->tbl.nodes[idx] = node;
    map->tbl.nodes[idx]->next = tmp;
  } else {
    if(!prev){
      map->tbl.nodes[idx] = node;
      node->next = it;
    } else {
      prev->next = node;
      node->next = it->next;
    }
    node_free(it);
  }
  map->tbl.len += 1;
}

// We can reuse the print logic to go through it
// or maybe we can somehow hash the keywords/operators again
void s_umap_free(struct S_Umap *map)
{
  (void)map;
  assert(0 && "unimplemented");
}

void *s_umap_get(const struct S_Umap *map, char *key)
{
  unsigned long idx = map->hash(key) % map->tbl.cap;

  struct _S_Umap_Node *it = map->tbl.nodes[idx];

  while (it) {
    if (!strcmp(it->key, key)) {
      return it->value;
    }
    it = it->next;
  }

  return NULL;
}

int s_umap_contains(const struct S_Umap *map, char *key)
{
  return s_umap_get(map, key) != NULL;
}

/*
  consider using hashes to print only the indices you
  know for a fact are being used.
*/
void s_umap_print(struct S_Umap *map, void (*vp)(const void *))
{
  int len = map->tbl.cap;
  printf("The len of the table: %ld\n", map->tbl.len);
  for(int i = 0; i <= len; i++){
    if(map->tbl.nodes[i]){
      struct _S_Umap_Node *tmp = map->tbl.nodes[i];
      printf("Keys for nodes[%d]: ", i);
      while(tmp){
        printf("(\"%s\"", tmp->key);
        if(vp) {
          printf(", ");
          vp(tmp->value);
        }
        tmp = tmp->next;
        if (tmp) {
          printf(") -> ");
        } else {
          printf(")\n");
        }
      }
      //printf("NULL\n");
    }
  }
}
