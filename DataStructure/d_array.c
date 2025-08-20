#include "d_array.h"

void* SafeReallocate(void* pointer, size_t newSize) {
  if (newSize == 0u) {
    free(pointer);
    return NULL;
  }

  void* newPointer = realloc(pointer, newSize);
  if (empty newPointer) {
    fprintf(stderr, "[System] Out of RAM.\n");
    exit(12);
  }
  return newPointer;
}
