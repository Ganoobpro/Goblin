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

  AddValue(&chunk->constants, value);
  return (ConstIndex)(chunk->constants.counter-1);
}

Value GetConstant(Chunk* chunk, ConstIndex index) {
  if (index > chunk->constants.counter)
    ErrorSystem("Try to get non-exist constant");

  return chunk->constants.values[index];
}

void PrintChunk(Chunk* chunk) {
  for (uint32_t i=0; i<chunk->counter; i++) {
    printf("[Line %d] ", chunk->lines[i]);

    switch (chunk->code[i]) {
      case OP_NEGATIVE: printf("OP_NEG\n"); break;
      case OP_ADD:      printf("OP_ADD\n"); break;
      case OP_SUB:      printf("OP_SUB\n"); break;

      case OP_CONSTANT:
        printf("OP_CONSTANT "); i++;
        printf("%i (%d)\n", (int)chunk->code[i],
                            GET_NUMBER(GetConstant(chunk, chunk->code[i])));
        break;

      case OP_RETURN:
        fprintf(stdout, "OP_RETURN\n");
        return;

      default:
        fprintf(stderr, "Unknown opcode.\n");
    };
  }
}
