#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "s-umap.h"
#include "fnv-1a.h"

#define INITAL_CAPACITY 256

struct S_Umap s_umap_create(s_umap_hash hash, s_umap_vdestr vdestr)
{
  struct S_Umap map;

  map.tbl.cap = INITAL_CAPACITY;
  map.tbl.len = 0;
  map.tbl.nodes = NULL;

  map.hash = hash;
  (void)vdestr;

  return map;
}

struct _S_Umap_Node* _s_umap_node_create(char* key, uint8_t value)
{
  struct _S_Umap_Node* node = malloc(sizeof(struct _S_Umap_Node));

  // Undefined Behavior if not using == NULL
  if(node == NULL){
    printf("Memory allocation error\n");
    exit(1);
  }

  node->next = NULL;

  // Does not need to be malloc'd
  // as string is already a pointer
  node->key = key;

  // Primitive type no malloc
  node->value = value;  

  return node;
}

void s_umap_insert(struct S_Umap *map, struct _S_Umap_Node* node)
{
  // This needs to be uint64_t
  uint64_t hash = map->hash(node->key);
  uint32_t index = hash % map->tbl.cap;

  struct _S_Umap_Node *tmp = map->tbl.nodes[index];
  (void)tmp;
  assert(0 && "unimplemented");
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
