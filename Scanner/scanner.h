#ifndef GOBLIN_SCANNER_H
#define GOBLIN_SCANNER_H

#include "../Base/setup.h"
#include "../Base/debug.h"

typedef enum {
  // OPERATORS
  // Math operators
  TOKEN_ADD, TOKEN_MINUS,
  TOKEN_MUL, TOKEN_DIV,
  TOKEN_NUM_DIV, TOKEN_MODULE,
  TOKEN_POWER,
  // Bit operators
  TOKEN_OR, TOKEN_AND,
  TOKEN_BITWISE, TOKEN_XOR,
  TOKEN_LEFT_SHIFT, TOKEN_RIGHT_SHIFT,
  // Condition
  TOKEN_OR_OR, TOKEN_AND_AND,
  TOKEN_EQUAL_EQUAL, TOKEN_EQUAL,
  TOKEN_LESS, TOKEN_LESS_EQUAL,
  TOKEN_BIGGER, TOKEN_BIGGER_EQUAL,
  TOKEN_NOT_EQUAL, TOKEN_NOT,

  // COMMANDS
  TOKEN_IF, TOKEN_ELSE,
  TOKEN_FOR, TOKEN_WHILE,
  TOKEN_FUNC, TOKEN_CLASS,

  // GROUPING
  TOKEN_LEFT_PARENTHESIS, TOKEN_RIGHT_PARENTHESIS,
  TOKEN_LEFT_BRACKET, TOKEN_RIGHT_BRACKET,
  TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,

  // LITERALS
  TOKEN_IDENTIFY, TOKEN_NUMBER,
  TOKEN_STRING, TOKEN_NILL,
  TOKEN_TRUE, TOKEN_FALSE,

  // OTHERS
  TOKEN_COMMA, TOKEN_DOT,
  TOKEN_SEMICOLON,

  // SYSTEM
  TOKEN_ERROR, TOKEN_EOL, TOKEN_EOF
} TokenType;

typedef struct {
  const char* start;
  uint8_t length;
  TokenType type;
  uint32_t line;
} Token;

typedef struct {
  const char* start;
  const char* current;
  uint32_t line;
} Scanner;

void InitScanner(const char* source);
void PrintToken(Token* token);
Token ScanToken();

#endif
