#ifndef GOBLIN_VM_H
#define GOBLIN_VM_H

#include "value.h"

#define STACK_MAX 256

typedef struct {
  Chunk chunk;
  int ip;
  Value stack[STACK_MAX];
  Value* stackTop;
} VM;

#endif
