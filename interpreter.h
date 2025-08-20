#ifndef GOBLIN_INTERPRETER_H
#define GOBLIN_INTERPRETER_H

typedef enum {
  INTERPRETER_OK,
  INTERPRETER_COMPILE_ERROR,
  INTERPRETER_RUNTIME_ERROR
} InterpreterResult;

InterpreterResult interpreter(const char* source);

#endif
