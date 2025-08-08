#ifndef GOBLIN_CHUNK_H
#define GOBLIN_CHUNK_H

#include "../Base/memory.h"
#include "value.h"

#define CONST_MAX  UINT8_MAX

typedef uint8_t ConstIndex;

typedef enum {
  OP_RETURN
} OpCode;

typedef struct {
  uint8_t counter;
  uint8_t capacity;
  Value* values;
} ValueArray;

typedef struct {
  uint8_t counter;
  uint8_t capacity;
  int* lines;
  OpCode* code;
  ValueArray constants;
} Chunk;

#endif
