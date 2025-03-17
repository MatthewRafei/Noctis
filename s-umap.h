#ifndef S_UMAP_H
#define S_UMAP_H

#include <stddef.h>

typedef unsigned long (*s_umap_hash)(const char *);

// Basically linked list node
struct _S_Umap_Node {
  struct _S_Umap_Node *next;
  char *key; // Because collisions
  void *value;
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

  size_t nodev_stride;
};

struct S_Umap s_umap_create(s_umap_hash hash, size_t nodev_stride);
struct _S_Umap_Node* _s_umap_node_create(char* key, void* value, size_t nodev_stride);
void s_umap_insert(struct S_Umap *map, char *key, void *value);
void s_umap_free(struct S_Umap *map);

#endif // S_UMAP_H
