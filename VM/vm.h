#ifndef GOBLIN_VM_H
#define GOBLIN_VM_H

#include "chunk.h"

#define STACK_MAX 256

typedef struct {
  Chunk chunk;
  uint8_t ip;
  Value stack[STACK_MAX];
  Value* stackTop;
} VM;

#endif
