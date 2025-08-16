#ifndef GOBLIN_VM_H
#define GOBLIN_VM_H

#include "chunk.h"

#define STACK_MAX 256

typedef enum {
  OP_ADD, OP_SUB,
  OP_MUL, OP_DIV,
  OP_NUM_DIV, OP_MODULE,

  OP_CONSTANT,
  OP_RETURN
} OpCode;

typedef struct {
  Chunk chunk;
  uint8_t ip;
  Value stack[STACK_MAX];
  Value* stackTop;
  Table globals;

  // Prepare for Garbage Collector
  Obj* objects;
} VM;

#endif
