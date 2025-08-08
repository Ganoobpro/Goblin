#ifndef GOBLIN_OBJECT_H
#define GOBLIN_OBJECT_H

typedef enum {
  ObjString,
  ObjList
} ObjType;

typedef struct {
  ObjType type;
} Obj;

#endif
