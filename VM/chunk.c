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

Value GetConstant(Chunk* chunk, ConstIndex index) {
  if (index > chunk->constants.counter)
    ErrorSystem("Try to get non-exist constant");

  return chunk->constants.values[index];
}

void PrintChunk(Chunk* chunk) {
  for (uint32_t i=0; i<chunk->counter; i++) {
    printf("[Line %d] ", chunk->lines[i]);

    switch (chunk->code[i]) {
      case OP_NUM_DIV:       printf("OP_NUM_DIV\n"); break;
      case OP_MODULE:        printf("OP_MODULE\n"); break;
      case OP_ADD:           printf("OP_ADD\n"); break;
      case OP_SUB:           printf("OP_SUB\n"); break;
      case OP_MUL:           printf("OP_MUL\n"); break;
      case OP_DIV:           printf("OP_DIV\n"); break;
      case OP_POWER:         printf("OP_POWER\n"); break;
      case OP_NEGATIVE:      printf("OP_NEGATIVE\n"); break;

      case OP_OR:            printf("OP_OR\n"); break;
      case OP_AND:           printf("OP_AND\n"); break;
      case OP_BITWISE:       printf("OP_BITWISE\n"); break;
      case OP_XOR:           printf("OP_XOR\n"); break;
      case OP_LEFT_SHIFT:    printf("OP_LEFT_SHIFT\n"); break;
      case OP_RIGHT_SHIFT:   printf("OP_RIGHT_SHIFT\n"); break;

      case OP_NOT:           printf("OP_NOT\n"); break;
      case OP_OR_OR:         printf("OP_OR_OR\n"); break;
      case OP_AND_AND:       printf("OP_AND_AND\n"); break;
      case OP_EQUAL_EQUAL:   printf("OP_EQUAL_EQUAL\n"); break;
      case OP_NOT_EQUAL:     printf("OP_NOT_EQUAL\n"); break;
      case OP_LESS:          printf("OP_LESS\n"); break;
      case OP_LESS_EQUAL:    printf("OP_LESS_EQUAL\n"); break;
      case OP_BIGGER:        printf("OP_BIGGER\n"); break;
      case OP_BIGGER_EQUAL:  printf("OP_BIGGER_EQUAL\n"); break;

      case OP_DECLARE_VAR:
        i++;
        printf("OP_DECLARE_VAR %i (%f)\n",
          (int)chunk->code[i],
          GET_NUMBER(GetConstant(chunk, chunk->code[i]))
        );
        break;

      case OP_GET_VAR:
        i++;
        printf("OP_GET_VAR %i (%f)\n",
          (int)chunk->code[i],
          GET_NUMBER(GetConstant(chunk, chunk->code[i]))
        );
        break;

      case OP_CONSTANT:
        i++;
        printf("OP_CONSTANT %i (%f)\n",
          (int)chunk->code[i],
          GET_NUMBER(GetConstant(chunk, chunk->code[i]))
        );
        break;

      case OP_RETURN:        printf("OP_RETURN\n"); break;
      default:               printf("Unknown OpCode (%i)\n", chunk->code[i]); break;
    };
  }
}
