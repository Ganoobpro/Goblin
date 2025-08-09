#ifndef GOBLIN_CHUNK_H
#define GOBLIN_CHUNK_H

#include "../Base/memory.h"
#include "value.h"

#define CONST_MAX UINT8_MAX

typedef uint8_t ConstIndex;

typedef enum {
  OP_RETURN
} OpCode;

typedef struct {
  uint32_t counter;
  uint32_t capacity;
  uint32_t* lines;
  uint8_t* code;
  ValueArray constants;
} Chunk;

void InitChunk(Chunk* chunk);
void FreeChunk(Chunk* chunk);
ConstIndex AddConstants(Chunk* chunk, Value value);

#endif
