#ifndef GOBLIN_CHUNK_H
#define GOBLIN_CHUNK_H

#define CONST_MAX UINT8_MAX

typedef uint8_t ConstIndex;

typedef enum {
  OP_NUM_DIV, OP_MODULE,
  OP_ADD, OP_SUB,
  OP_MUL, OP_DIV,
  OP_POWER, OP_NEGATIVE,

  OP_COND_OR, OP_COND_AND,
  OP_NOT,

  OP_CONSTANT,
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
void WriteChunk(Chunk* chunk, uint32_t line, uint8_t byte);
ConstIndex AddConstant(Chunk* chunk, Value value);

#endif
