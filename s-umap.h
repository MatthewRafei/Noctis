#ifndef S_UMAP_H
#define S_UMAP_H

#include <stdint.h>
#include <stddef.h>

typedef unsigned long (*s_umap_hash)(const char *);
typedef void (*s_umap_vdestr)(uint8_t *);

// Basically linked list node
struct _S_Umap_Node {
  struct _S_Umap_Node *next;
  char *key; // Because collisions
  uint8_t *value;
};

// Struct in a struct for dot notation 
struct S_Umap {
  struct {
    // Array of nodes
    struct _S_Umap_Node **nodes;
    size_t len, cap;
  } tbl;

  // Hash function pointer 
  s_umap_hash hash;
  // Value destructor
  s_umap_vdestr vdestr; // Optional, can be null
};

struct S_Umap s_umap_create(s_umap_hash hash, s_umap_vdestr vdestr);
struct _S_Umap_Node* _s_umap_node_create(char* key, uint8_t value);
void s_umap_insert(struct S_Umap map, struct _S_Umap_Node* node);

//void s_umap_free(struct S_Umap *map);

#endif // S_UMAP_H
