#include "debug.h"

void Error(const char* message, const int line) {
  fprintf(stderr, "[Line %i] Error: %s.", line, message);
}

void ErrorChar(const char errorChar, const int line) {
  fprintf(stderr, "[Line %d] Error: Unknown character [%c].\n", line, errorChar);
}
