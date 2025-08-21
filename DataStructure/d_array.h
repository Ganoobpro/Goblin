#ifndef GOBLIN_D_ARRAY_H
#define GOBLIN_D_ARRAY_H

void* SafeReallocate(void* pointer, size_t newSize);

//////////  DYNAMIC ARRAY  //////////
#define GROW_CAPACITY(capacity) \
  (((capacity) < 8) ? 8 : ((capacity)*2))
#define GROW_ARRAY(type, pointer, newCapacity) \
  (type*)SafeReallocate(pointer, sizeof(type)*newCapacity)
#define FREE_ARRAY(type, pointer) \
  (type*)SafeReallocate(pointer, 0)
#define FREE(pointer) \
  SafeReallocate(pointer, 0)

#endif
