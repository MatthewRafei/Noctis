#include "ds/s_umap.h"
#include "fnv-1a.h"
#include "token.h"
#include "utils.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
TODO(malac0da):
- Use fprintf(stderr, ...) for error messages.

- Fix typo: INITIAL_CAPACITY → INITIAL_CAPACITY.

- Only increment map->tbl.len for new keys, not replacements.
- Check for allocation failures in s_umap_node_create and handle gracefully.

- Remove unnecessary casts in memcpy (use void*).

- Improve documentation for ownership and parameters.

- Add unit tests for edge cases and resizing.
*/

#define INITIAL_CAPACITY 1024

struct S_Umap s_umap_create(s_umap_hash hash, size_t nodev_stride)
{
  assert(hash);

  struct S_Umap map = (struct S_Umap){
      .tbl = {
          .nodes = (struct S_Umap_Node **)calloc(INITIAL_CAPACITY,
                                                  sizeof(struct S_Umap_Node *)),
          .len = 0,
          .cap = INITIAL_CAPACITY,
      },
      .hash = hash,
      .nodev_stride = nodev_stride,
  };

  return map;
}

struct S_Umap_Node *s_umap_node_create(const char *key,
                                         const void *value,
                                         size_t nodev_stride)
{
  struct S_Umap_Node *node = (struct S_Umap_Node *)s_malloc(sizeof(struct S_Umap_Node));
  node->next = NULL;

  node->key = strdup(key);

  node->value = s_malloc(nodev_stride);
  (void)memcpy((uint32_t *)node->value, (uint32_t *)value, nodev_stride);

  return node;
}

// Returns the node where its key == k.
// If no such node is found, returns NULL.
static struct S_Umap_Node *list_has_key(const struct S_Umap *m,
                                         unsigned long idx,
                                         const char *k,
                                         struct S_Umap_Node **prev)
{
  *prev = NULL;
  struct S_Umap_Node *it = m->tbl.nodes[idx];
  while (it)
  {
    if (!strcmp(it->key, k))
    {
      return it;
    }
    *prev = it;
    it = it->next;
  }
  return NULL;
}

static void node_free(struct S_Umap_Node *n)
{
  free(n->key);
  free(n->value);
  free(n);
}

void s_umap_insert(struct S_Umap *map, const char *key, const void *value)
{
  struct S_Umap_Node *node = s_umap_node_create(key, value, map->nodev_stride);

  unsigned long idx = map->hash(node->key) % map->tbl.cap;

  struct S_Umap_Node *prev = NULL;
  struct S_Umap_Node *it = list_has_key(map, idx, key, &prev);

  if (!it)
  {
    struct S_Umap_Node *tmp = map->tbl.nodes[idx];
    map->tbl.nodes[idx] = node;
    map->tbl.nodes[idx]->next = tmp;
  }
  else
  {
    if (!prev)
    {
      map->tbl.nodes[idx] = node;
      node->next = it;
    }
    else
    {
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
  for (size_t i = 0; i < map->tbl.cap; ++i)
  {
    struct S_Umap_Node *node = map->tbl.nodes[i];
    while (node)
    {
      struct S_Umap_Node *next = node->next;
      node_free(node);
      node = next;
    }
  }
  free(map->tbl.nodes);
}

void *s_umap_get(const struct S_Umap *map, const char *key)
{
  unsigned long idx = map->hash(key) % map->tbl.cap;

  struct S_Umap_Node *it = map->tbl.nodes[idx];

  while (it)
  {
    if (!strcmp(it->key, key))
    {
      return it->value;
    }
    it = it->next;
  }

  return NULL;
}

int s_umap_contains(const struct S_Umap *map, const char *key)
{
  return s_umap_get(map, key) != NULL;
}

/*
  consider using hashes to print only the indices you
  know for a fact are being used.
*/
void s_umap_print(struct S_Umap *map, void (*vp)(const void *))
{
  size_t len = map->tbl.cap;
  printf("\n");
  printf("The len of the hash table: %zu\n", map->tbl.len);
  printf("\n");
  for (size_t i = 0; i < len; i++)
  {
    if (map->tbl.nodes[i])
    {
      struct S_Umap_Node *tmp = map->tbl.nodes[i];
      printf("Keys for nodes[%zu]: ", i);
      while (tmp)
      {
        printf("(\"%s\"", tmp->key);
        if (vp)
        {
          printf(", ");
          vp(tmp->value);
        }
        tmp = tmp->next;
        if (tmp)
        {
          printf(") -> ");
        }
        else
        {
          printf(")\n");
        }
      }
    }
  }
  printf("\n");
}