#ifndef GOBLIN_VALUE_H
#define GOBLIN_VALUE_H

#include "object.h"

typedef enum {
  NILL,
  BOOLEAN,
  NUMBER,
  OBJECT
} ValueType;

typedef struct {
  ValueType type;
  union {
    bool boolean;
    double number;
    Obj* obj;
  } as;
} Value;

#define MAKE_NILL           ((Value){NILL, {.number=0}})
#define MAKE_BOOLEAN(value) ((Value){BOOLEAN, {.boolean=(value)}})
#define MAKE_NUMBER(value)  ((Value){NUMBER, {.number=(value)}})
#define MAKE_OBJECT(value)  ((Value){OBJECT, {.obj=(value)}})

#define IS_NILL(value)      ((value).type == NILL)
#define IS_BOOLEAN(value)   ((value).type == BOOLEAN)
#define IS_NUMBER(value)    ((value).type == NUMBER)
#define IS_OBJECT(value)    ((value).type == OBJECT)

#define GET_BOOLEAN(value)  ((value).as.boolean)
#define GET_NUMBER(value)   ((value).as.number)
#define GET_OBJECT(value)   ((value).as.obj)

typedef struct {
  uint8_t counter;
  uint8_t capacity;
  Value* values;
} ValueArray;

void InitValueArray(ValueArray* constants);
void FreeValueArray(ValueArray* constants);
void AddValue(ValueArray* constants, Value value);

#endif
