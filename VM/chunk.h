#ifndef GOBLIN_CHUNK_H
#define GOBLIN_CHUNK_H

typedef enum {
  OP_RETURN
} OpCode;

typedef struct {
  int counter;
  int capacity;
  OpCode* code;
} Chunk;

#endif
