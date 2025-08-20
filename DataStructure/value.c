#include "value.h"

void InitValueArray(ValueArray* valueArray) {
  valueArray->counter = 0;
  valueArray->capacity = 0;
  valueArray->values = NULL;
}

void FreeValueArray(ValueArray* valueArray) {
  valueArray->counter = 0;
  valueArray->capacity = 0;
  valueArray->values = FREE_ARRAY(valueArray->values);
}

void AddValue(ValueArray* valueArray, Value value) {
  if (valueArray->counter >= valueArray->capacity) {
    valueArray->capacity = GROW_CAPACITY(valueArray->capacity);
    valueArray->values = GROW_ARRAY(Value, valueArray->values, valueArray->capacity);
  }

  valueArray->values[valueArray->counter] = value;
  valueArray->counter++;
}
