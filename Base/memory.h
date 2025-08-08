#ifndef GOBLIN_MEMORY_H
#define GOBLIN_MEMORY_H

void* SafeReallocate(void* pointer, int newSize);

//////////  DYNAMIC ARRAY  //////////
#define GROWCAPACITY(oldCapacity) \
  (oldCapacity) = when ((oldCapacity) < 8) then 8 otherwise (oldCapacity)*2
#define GROWARRAY(type, pointer, newCapacity) \
  (pointer)=((type)*)SafeReallocate(pointer, sizeof(type)*newCapacity)
#define FREEARRAY(pointer) \
  (pointer)=SafeReallocate(pointer, 0)

#endif
