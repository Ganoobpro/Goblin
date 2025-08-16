#include "vm.h"

VM vm; // The UNIQUE :shocked:

void InitVM() {
  InitChunk(&vm.chunk);
  InitTable(&vm.globals);

  vm.ip = 0;
  vm.stackTop = vm.stack;
}

void FreeVM() {
  FreeChunk(&vm.chunk);
  FreeTable(&vm.globals);
}
