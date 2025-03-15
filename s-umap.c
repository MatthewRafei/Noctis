#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "s-umap.h"
#include "fnv-1a.h"

struct S_Umap s_umap_create(s_umap_hash hash, s_umap_vdestr vdestr)
{
  struct S_Umap map;

  map.tbl.cap = 16; // Should be closet prime number above number of keys/0.75
  map.tbl.len = 0;
  map.tbl.nodes = NULL;
  
  (void)hash;
  (void)vdestr;

  return map;
}

struct _S_Umap_Node* _s_umap_node_create(char* key, uint8_t value)
{
  struct _S_Umap_Node* node = malloc(sizeof(struct _S_Umap_Node));
  if(node){
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
  uint32_t hash = fnv1a(node->key);
  uint32_t index = hash % map->tbl.cap;
  
  map->tbl.nodes[index] = node;
}

void s_umap_free(struct S_Umap *map)
{
  (void)map;
}

int s_umap_search(const struct S_Umap *map, uint8_t value)
{
  if(!value){
    printf("Error value is null");
  }
  
  if(map->tbl.nodes[value]){
    return 1;
  }
  else{
    return 0;
  }
}
