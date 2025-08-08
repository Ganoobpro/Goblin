#include "value.h"

void InitValueArray(ValueArray* constants) {
  constants->counter = 0;
  constants->capacity = 0;
  constants->values = NULL;
}

void FreeValueArray(ValueArray* constants) {
  constants->counter = 0;
  constants->capacity = 0;
  FREEARRAY(constants->values);
}

void AddValue(ValueArray* constants, Value value) {
  if (constants->counter >= constants->capacity) {
    GROWCAPACITY(constants->capacity);
    GROWARRAY(Value, constants->values, constants->capacity);
  }

  constants->values[constants->counter] = value;
  constants->counter++;
}
