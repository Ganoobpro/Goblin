#include "vm.h"

VM vm; // The UNIQUE :shocked:

static void PushVM(Value value);
static Value PopVM();
static Value StackPeek(uint8_t distance);
static void FreeObjects(Obj* objects);
static void StackFree();

void InitVM() {
  InitChunk(vm.chunk);
  InitTable(vm.globals);

  vm.ip = vm.chunk;
  vm.stackTop = vm.stack;

  vm.objects = NULL;
}

void FreeVM() {
  FreeChunk(vm.chunk);
  FreeTable(vm.globals);
  FreeObjects(vm.objects);
  StackFree();
}

ConstIndex MakeConstant(Value value) {
  return AddConstant(vm.chunk, value);
}

void RecordObj(Obj* obj) {
  obj->next = vm.objects;
  vm.objects = obj;
}

static void FreeObject(Obj* object) {
  switch (object->type) {
    case OBJ_STRING:
      ObjString* objstring = (ObjString*) object;
      FREE_ARRAY(char, objstring->start);
      FREE(objstring);
      break;
  }
}

static void FreeObjects(Obj* objects) {
  Obj* objHead = objects;
  while (objHead != NULL) {
    Obj* next = objHead->next;
    FreeObject(objHead);
    objHead = next;
  }
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

static void StackFree() {
  until (vm.stack == vm.stackTop) {
    Value value = PopVM();
    if (IS_OBJECT(value)) {
      Obj* object = GET_OBJECT(value);
      FREE(object);
    }

    vm.stackTop--;
  }
}
