#include "chunk.h"
#include "../Base/debug.h"

void InitChunk(Chunk* chunk) {
  chunk->counter = 0;
  chunk->capacity = 0;
  chunk->lines = NULL;
  chunk->code = NULL;
  InitValueArray(&chunk->constants);
}

void FreeChunk(Chunk* chunk) {
  chunk->counter = 0;
  chunk->capacity = 0;
  chunk->lines = FREE_ARRAY(uint32_t, chunk->lines);
  chunk->code = FREE_ARRAY(uint8_t, chunk->code);
  FreeValueArray(&chunk->constants);
}

void WriteChunk(Chunk* chunk, uint32_t line, uint8_t byte) {
  if (chunk->counter >= chunk->capacity) {
    chunk->capacity = GROW_CAPACITY(chunk->capacity);
    chunk->lines = GROW_ARRAY(uint32_t, chunk->lines, chunk->capacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, chunk->capacity);
  }

  chunk->lines[chunk->counter] = line;
  chunk->code[chunk->counter] = byte;
  chunk->counter++;
}

ConstIndex AddConstant(Chunk* chunk, Value value) {
  if (chunk->constants.counter >= CONST_MAX)
    ErrorSystem("Too much constants");

  if (IS_OBJECT(value))
    RecordObj(GET_OBJECT(value));

  AddValue(&chunk->constants, value);
  return (ConstIndex)(chunk->constants.counter-1);
}

static Value GetConstant(Chunk* chunk, ConstIndex index) {
  if (index > chunk->constants.counter)
    ErrorSystem("Try to get non-exist constant");

  return chunk->constants.values[index];
}

void PrintChunk(Chunk* chunk) {
  const char* OpCodeNames[] = {
    [OP_NUM_DIV]      = "OP_NUM_DIV",
    [OP_MODULE]       = "OP_MODULE",
    [OP_ADD]          = "OP_ADD",
    [OP_SUB]          = "OP_SUB",
    [OP_MUL]          = "OP_MUL",
    [OP_DIV]          = "OP_DIV",
    [OP_POWER]        = "OP_POWER",
    [OP_NEGATIVE]     = "OP_NEGATIVE",

    [OP_OR]           = "OP_OR",
    [OP_AND]          = "OP_AND",
    [OP_BITWISE]      = "OP_BITWISE",
    [OP_XOR]          = "OP_XOR",
    [OP_LEFT_SHIFT]   = "OP_LEFT_SHIFT",
    [OP_RIGHT_SHIFT]  = "OP_RIGHT_SHIFT",

    [OP_NOT]          = "OP_NOT",
    [OP_OR_OR]        = "OP_OR_OR",
    [OP_AND_AND]      = "OP_AND_AND",
    [OP_EQUAL_EQUAL]  = "OP_EQUAL_EQUAL",
    [OP_NOT_EQUAL]    = "OP_NOT_EQUAL",
    [OP_LESS]         = "OP_LESS",
    [OP_LESS_EQUAL]   = "OP_LESS_EQUAL",
    [OP_BIGGER]       = "OP_BIGGER",
    [OP_BIGGER_EQUAL] = "OP_BIGGER_EQUAL",

    [OP_DECLARE_VAR]  = "OP_DECLARE_VAR",
    [OP_GET_VAR]      = "OP_GET_VAR",

    [OP_CONSTANT]     = "OP_CONSTANT",
    [OP_RETURN]       = "OP_RETURN",
  };

  for (uint32_t i=0; i<chunk->counter; i++) {
    printf("[Line %d] %s",
      chunk->lines[i],
      OpCodeNames[chunk->code[i]]
    );

    if (
      chunk->code[i] == OP_DECLARE_VAR ||
      chunk->code[i] == OP_GET_VAR ||
      chunk->code[i] == OP_CONSTANT
    ) {
      i++;
      printf(" %u   ", chunk->code[i]);
      PrintValue(
        GetConstant(
          chunk,
          chunk->code[i]
        )
      );
    } else printf("\n");
  }
}
