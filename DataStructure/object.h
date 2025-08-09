#ifndef GOBLIN_OBJECT_H
#define GOBLIN_OBJECT_H

#include "value.h"

typedef enum {
  OBJ_STRING,
  OBJ_LIST
} ObjType;

typedef struct {
  ObjType type;
} Obj;

typedef struct {
  Obj obj;
  uint32_t hash;
  uint32_t length;
  char* start;
} ObjString;

typedef struct {
  Obj obj;
  uint32_t counter;
  uint32_t capacity;
  Value* list;
} ObjList;

Obj* CopyStringToObj(const char* str, const uint32_t length);

#define MAKE_STRING(value)  MAKE_OBJECT(CopyStringToObj((value),sizeof(value)))

#define IS_STRING(value)    (IsObjType(value, OBJ_STRING))
#define IS_LIST(value)      (IsObjType(value, OBJ_LIST))

#define GET_STRING(value)   ((ObjString*)GET_OBJECT(value))
#define GET_CHAR(value)     (GET_STRING(value)->start)
#define GET_OBJ_LIST(value) ((ObjList*)GET_OBJECT(value))
#define GET_LIST(value)     (GET_OBJ_LIST(value)->list)

#endif
