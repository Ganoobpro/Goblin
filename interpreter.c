#include "interpreter.h"

Chunk chunkPtr;

InterpreterResult Interpreter(const char* source) {
  InitChunk(&chunkPtr);
  if (Compile(&chunkPtr, source)) {
    PrintChunk(&chunkPtr);
    FreeChunk(&chunkPtr);
    return INTERPRETER_OK;
  }

  FreeChunk(&chunkPtr);
  return INTERPRETER_COMPILE_ERROR;
}
