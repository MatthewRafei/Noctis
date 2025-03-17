#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "s-umap.h"
#include "utils.h"
#include "fnv-1a.h"

#define INITAL_CAPACITY 256

struct S_Umap s_umap_create(s_umap_hash hash, size_t nodev_stride)
{
  assert(hash);

  struct S_Umap map = (struct S_Umap) {
    .tbl = {
      // First one causes seg fault
      //.nodes = (struct _S_Umap_Node **)s_malloc(sizeof(struct _S_Umap_Node *) * INITAL_CAPACITY),
      .nodes = (struct _S_Umap_Node **)calloc(INITAL_CAPACITY, sizeof(struct _S_Umap_Node *)),
      .len = 0,
      .cap = INITAL_CAPACITY,
    },
    .hash = hash,
    .nodev_stride = nodev_stride,
  };

  return map;
}

struct _S_Umap_Node* _s_umap_node_create(char* key, void *value, size_t nodev_stride)
{
  struct _S_Umap_Node* node = (struct _S_Umap_Node *)s_malloc(sizeof(struct _S_Umap_Node));
  node->next = NULL;


  node->key = strdup(key);

  if(!node->key){
    printf("Key is null\n");
    exit(1);
  }
  else if(!key){
    printf("Key is null\n");
    exit(1);
  }
  

  node->value = s_malloc(nodev_stride);
  (void)memcpy((uint32_t *)node->value, (uint32_t *)value, nodev_stride);

  return node;
}

// Returns the node where its key == k. If no such
// node is found, returns NULL.
static struct _S_Umap_Node *list_has_key(const struct S_Umap *m,
                                         unsigned long idx,
                                         const char *k,
                                         struct _S_Umap_Node **prev)
{
  *prev = NULL;
  struct _S_Umap_Node *it = m->tbl.nodes[idx];
  while (it) {

    if(k == NULL){
      printf("K is null\n");
      exit(1);
    }

    if (!it) {
      printf("it is NULL\n");
      exit(1);
    }
    
    if (!strcmp(it->key, k)) {
      if(!it){
	printf("it is null\n");
	exit(1);
      }
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
}

void s_umap_free(struct S_Umap *map)
{
  (void)map;
  assert(0 && "unimplemented");
}

int s_umap_search(const struct S_Umap *map, char* key)
{
  if(!key){
    printf("Error value is null");
  }
  
  uint64_t hash = map->hash(key);
  uint32_t index = hash % map->tbl.cap;
  
  if(map->tbl.nodes[index]){
    return 1;
  }
  else{
    return 0;
  }
}
