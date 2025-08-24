#ifndef GOBLIN_OBJECT_H
#define GOBLIN_OBJECT_H

typedef enum {
  OBJ_STRING,
  OBJ_LIST
} ObjType;

typedef struct Obj {
  ObjType type;
  struct Obj* next;
} Obj;

typedef struct ObjString {
  Obj obj;
  uint32_t hash;
  uint32_t length;
  char* start;
} ObjString;

typedef struct ObjList {
  Obj obj;
  uint32_t counter;
  uint32_t capacity;
  Value* list;
} ObjList;

Obj* CopyStringToObj(const char* strValue, const uint32_t length);

#define MAKE_STRING(value, strlen) \
  ((Value){OBJECT,{.obj=CopyStringToObj((value),(strlen))}})

#define IS_STRING(value)    (IsObjType(value, OBJ_STRING))
#define IS_LIST(value)      (IsObjType(value, OBJ_LIST))

#define GET_STRING(value)   ((ObjString*)GET_OBJECT(value))
#define GET_CHAR(value)     (GET_STRING(value)->start)
#define GET_OBJ_LIST(value) ((ObjList*)GET_OBJECT(value))
#define GET_LIST(value)     (GET_OBJ_LIST(value)->list)

#endif
