#include "object.h"
#include "../VM/vm.h"

// FNV-1a + MurmurHash-32
static inline uint32_t hashString(const char* strPtr) {
  // FNV-1a
  uint32_t h = 2166136261u;
  while (*strPtr) {
    h ^= (uint8_t)*strPtr;
    h *= 16777619u;
  }

  // MurmurHash-32
  h ^= h >> 16;
  h *= 0x85ebca6bU;
  h ^= h >> 13;
  h *= 0xc2b2ae35U;
  h ^= h >> 16;

  return h;
}

Obj* CopyStringToObj(const char* strValue, const uint32_t length) {
  ObjString* objStr = malloc(sizeof(ObjString));
  objStr->obj.type = OBJ_STRING;
  objStr->length = length;

  objStr->start = GROW_ARRAY(char, objStr->start, length+1);
  memcpy(objStr->start, strValue, length);
  objStr->start[length] = '\0';
  objStr->hash = hashString(objStr->start);

  RecordObj((Obj*)objStr);
  return (Obj*)objStr;
}
