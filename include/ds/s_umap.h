#ifndef DS_S_UMAP_H
#define DS_S_UMAP_H

#include <stddef.h>

typedef unsigned long (*s_umap_hash)(const char *);

// Basically linked list node
struct S_Umap_Node {
  struct S_Umap_Node *next;
  char *key; // Because collisions
  void *value;
};

// Struct in a struct for dot notation 
struct S_Umap {
  struct {
    // Array of nodes
    struct S_Umap_Node **nodes;
    size_t len, cap;
  } tbl;

  // Hash function pointer
  s_umap_hash hash;

  size_t nodev_stride;
};

struct S_Umap s_umap_create(s_umap_hash hash, size_t nodev_stride);
struct S_Umap_Node* s_umap_node_create(char* key, void* value, size_t nodev_stride);
void s_umap_insert(struct S_Umap *map, char *key, void *value);
void s_umap_free(struct S_Umap *map);
void s_umap_print(struct S_Umap *map, void (*vp)(const void *));
void *s_umap_get(const struct S_Umap *map, const char *key);
int s_umap_contains(const struct S_Umap *map, const char *key);



#endif // DS_S_UMAP_H
