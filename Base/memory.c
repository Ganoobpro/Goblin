#include "memory.h"

void* SafeReallocate(void* pointer, int newSize) {
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }

  void* newPointer = realloc(pointer, newSize);
  if (newPointer == NULL) {
    fprintf(stderr, "[System] Out of RAM.\n");
    exit(12);
  }
  return newPointer;
}
