#ifndef GOBLIN_VM_H
#define GOBLIN_VM_H

#include "chunk.c"
#include "../DataStructure/table.h"

#define STACK_MAX 256

typedef struct {
  Chunk* chunk;
  uint8_t* ip;
  Value stack[STACK_MAX];
  Value* stackTop;
  Table* globals;

  // Prepare for Garbage Collector
  Table* strings;
  Obj* objects;
} VM;

void InitVM();
void FreeVM();
ConstIndex MakeConstant(Value value);
void RecordObj(Obj* obj);

#endif
