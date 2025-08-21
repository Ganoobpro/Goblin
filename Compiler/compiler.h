#ifndef GOBLIN_COMPILER_H
#define GOBLIN_COMPILER_H

#include "../VM/chunk.h"

typedef struct {
  Token previous;
  Token current;
  bool hadError;
  bool panicMode;
} Parser;

typedef void (*ParseFn)();

typedef enum {
  PREC_NONE,         //
  PREC_ASSIGNMENT,   //
  PREC_BIT_SHIFT,    // << >>
  PREC_BIT_OP,       // & | ~ ^
  PREC_OR,           // ||
  PREC_AND,          // &&
  PREC_EQUALITY,     // == !=
  PREC_COMPARISON,   // > >= < <=
  PREC_SPECIAL_DIV,  // % //
  PREC_TERM,         // + -
  PREC_FACTOR,       // * /
  PREC_POWER,        // **
  PREC_UNARY,        // - !
  PREC_PRIMARY       // Literals
} Precedence;

typedef struct {
  ParseFn prefix;
  ParseFn infix;
  Precedence precedence;
} ParseRules;

bool Compile(Chunk* currentChunk, const char* source);

#endif
