#include "value.h"

void PrintObject(Obj* obj);

void InitValueArray(ValueArray* valueArray) {
  valueArray->counter = 0;
  valueArray->capacity = 0;
  valueArray->values = NULL;
}

void FreeValueArray(ValueArray* valueArray) {
  valueArray->counter = 0;
  valueArray->capacity = 0;
  valueArray->values = FREE_ARRAY(Value, valueArray->values);
}

void AddValue(ValueArray* valueArray, Value value) {
  if (valueArray->counter >= valueArray->capacity) {
    valueArray->capacity = GROW_CAPACITY(valueArray->capacity);
    valueArray->values = GROW_ARRAY(Value, valueArray->values, valueArray->capacity);
  }

  valueArray->values[valueArray->counter] = value;
  valueArray->counter++;
}

void PrintValue(Value value) {
  switch (value.type) {
    case NILL:    printf("[NILL]\n"); break;

    case BOOLEAN:
      printf("[BOOLEAN] ");
      if (GET_BOOLEAN(value)) printf("True\n");
      else                    printf("False\n");
      break;

    case NUMBER:
      printf("[NUMBER]  %f\n", GET_NUMBER(value));
      break;

    case OBJECT:
      PrintObject(GET_OBJECT(value));
      break;

    default:
      return;
  };
}
