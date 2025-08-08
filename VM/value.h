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

#define MAKE_BOOLEAN(value) ((Value){BOOLEAN, {.boolean=(value)}})
#define MAKE_NUMBER(value)  ((Value){NUMBER, {.number=(value)}})
#define MAKE_OBJECT(value)  ((Value){OBJECT, {.obj=(value)}})

#endif
