#include "chunk.h"
#include "../DataStructure/value.h"

void InitChunk(Chunk* chunk) {
  chunk->counter = 0;
  chunk->capacity = 0;
  chunk->lines = NULL;
  chunk->code = NULL;
  InitConstantArray(&chunk->constants);
}

void FreeChunk(Chunk* chunk) {
  chunk->counter = 0;
  chunk->capacity = 0;
  FREEARRAY(chunk->lines);
  FREEARRAY(chunk->code);
  FreeValueArray(&chunk->constants);
}

ConstIndex AddConstants(Chunk* chunk, Value value) {
  if (chunk->constants.counter >= CONST_MAX) {
    fprintf(stderr, "[System] Too much constants.\n");
    exit(1); // TODO: Add correct exit code later
  }

  AddValue(&chunk->constants, value);
  return (ConstIndex)(chunk->constants.counter-1);
}
