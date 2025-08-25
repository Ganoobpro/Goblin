#ifndef GOBLIN_CHUNK_H
#define GOBLIN_CHUNK_H

#define CONST_MAX UINT8_MAX

typedef uint8_t ConstIndex;

typedef enum {
  OP_NUM_DIV, OP_MODULE,
  OP_ADD, OP_SUB,
  OP_MUL, OP_DIV,
  OP_POWER, OP_NEGATIVE,

  OP_OR, OP_AND,
  OP_BITWISE, OP_XOR,
  OP_LEFT_SHIFT, OP_RIGHT_SHIFT,

  OP_NOT, OP_OR_OR,
  OP_AND_AND, OP_EQUAL_EQUAL,
  OP_NOT_EQUAL, OP_LESS,
  OP_LESS_EQUAL, OP_BIGGER,
  OP_BIGGER_EQUAL,

  OP_DECLARE_VAR, OP_GET_VAR,

  OP_JUMP_COND, OP_JUMP,

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

void RecordObj(Obj* obj);

void InitChunk(Chunk* chunk);
void FreeChunk(Chunk* chunk);
void WriteChunk(Chunk* chunk, uint32_t line, uint8_t byte);
ConstIndex AddConstant(Chunk* chunk, Value value);

#endif
