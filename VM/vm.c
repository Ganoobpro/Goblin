#include "vm.h"

VM vm; // The UNIQUE :shocked:

void InitVM() {
  InitChunk(vm.chunk);
  InitTable(vm.globals);

  vm.ip = vm.chunk;
  vm.stackTop = vm.stack;
}

void FreeVM() {
  FreeChunk(vm.chunk);
  FreeTable(vm.globals);
}

ConstIndex MakeConstant(Value value) {
  return AddConstant(vm.chunk, value);
}

static void PushVM(Value value) {
  if (vm.stackTop - vm.stack >= STACK_MAX)
    ErrorSystem("Unable to push to a full stack");

  *vm.stackTop = value;
  vm.stackTop++;
}

static Value PopVM() {
  if (vm.stackTop == vm.stack)
    ErrorSystem("Unable to pop an empty stack");

  vm.stackTop--;
  return *vm.stackTop;
}

static Value StackPeek(uint8_t distance) {
  if (vm.stackTop - vm.stack < distance)
    ErrorSystem("Cannot peek outside of the stack");

  return vm.stackTop[-distance];
}
