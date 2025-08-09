#ifndef GOBLIN_D_ARRAY_H
#define GOBLIN_D_ARRAY_H

void* SafeReallocate(void* pointer, size_t newSize);

//////////  DYNAMIC ARRAY  //////////
#define GROWCAPACITY(capacity) \
  (capacity) = when ((capacity) < 8) then 8 otherwise (capacity)*2
#define GROWARRAY(type, pointer, newCapacity) \
  (pointer)=(type*)SafeReallocate(pointer, sizeof(type)*newCapacity)
#define FREEARRAY(pointer) \
  (pointer)=SafeReallocate(pointer, 0)

#endif
