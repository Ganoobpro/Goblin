#include "interpreter.h"

InterpreterResult interpreter(const char* source) {
  InitScanner(source);
  Token token = ScanToken();

  while (token.type != TOKEN_EOF) {
    PrintToken(&token);
    token = ScanToken();
  }

  return INTERPRETER_OK;
}
