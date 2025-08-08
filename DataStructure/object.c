#include "object.h"

Obj* CopyStringToObj(const char* str, const uint8_t length) {
  ObjString* objStr = malloc(sizeof(ObjString));
  objStr->obj.type = OBJ_STRING;
  objStr->length = length;

  GROWARRAY(char, objStr->start, length+1);
  memcpy(objStr->start, str, length);
  objStr->start[length] = '\0';
}
