#include "chunk.h"

// VALUE ARRAY
static void InitValueArray(ValueArray* constants) {
  constants->counter = 0;
  constants->capacity = 0;
  constants->values = NULL;
}

static void FreeValueArray(ValueArray* constants) {
  constants->counter = 0;
  constants->capacity = 0;
  FREEARRAY(constants->values);
}

static void AddValue(ValueArray* constants, Value value) {
  if (constants->counter >= constants->capacity) {
    GROWCAPACITY(constants->capacity);
    GROWARRAY(Value, constants->values, constants->capacity);
  }

  constants->values[constants->counter] = value;
  constants->counter++;
}

// CHUNK OF CODE
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
