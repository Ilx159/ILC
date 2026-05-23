#ifndef ILCARENA_H
#define ILCARENA_H

#include "ilcTypes.h"
#include <string.h>

typedef struct {
  u8 *arena;
  size_t size;
  size_t offset;
} arena_t;


arena_t arenaCreate(size_t size);
void *arenaAlloc(arena_t *a, size_t size);
void arenaReset(arena_t *a);
void arenaDestroy(arena_t *a);

#ifdef ILCARENA_IMPLEMENTATION
  arena_t arenaCreate(size_t size){
    arena_t arena = {malloc(size), size, 0};
    return arena;
  }
  void *arenaAlloc(arena_t *a, size_t size){
    assert(a->offset + size <= a->size);
    void *p = a->arena + a->offset;
    a->offset += size;
    return p;
  }
  void arenaReset(arena_t *a){
    memset(a->arena, 0, a->size);
    a->offset = 0;
  }
  void arenaDestroy(arena_t *a){
    free(a->arena);
    a->arena = NULL;
    a->offset = 0;
    a->size = 0;
  }
#endif
#endif
