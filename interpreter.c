#include "interpreter.h"

VM vm; // The UNIQUE

// TODO: Fix compiler
InterpreterResult Interpreter(const char* source) {
  InitVM();
  if (Compile(vm.chunk, source)) {
    PrintChunk(vm.chunk);
    FreeChunk(vm.chunk);
    return INTERPRETER_OK;
  }

  FreeChunk(vm.chunk);
  return INTERPRETER_COMPILE_ERROR;
}
