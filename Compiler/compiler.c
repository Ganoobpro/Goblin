#include "compiler.h"

Parser parser;
Chunk* currentCompilerChunk;

static void ParseErrorAt(Token* token, const char* errorMessage);
static void ParseErrorAtCurr(const char* errorMessage);
static inline ParseRules* GetRule(TokenType type);
static void Advance();
static void Consume(TokenType type, const char* errorMessage);
static void ParsePrecedence(Precedence precedence);
static void EmitByte(uint8_t opcode);
static void EmitConstant(Value value);
static void EmitReturn();
static void Number();
static void Grouping();
static void Unary();
static void Binary();

ParseRules rules[] = {
  [TOKEN_IF] =                {NULL,          NULL,          PREC_NONE},
  [TOKEN_ELSE] =              {NULL,          NULL,          PREC_NONE},
  [TOKEN_FOR] =               {NULL,          NULL,          PREC_NONE},
  [TOKEN_WHILE] =             {NULL,          NULL,          PREC_NONE},
  [TOKEN_FUNC] =              {NULL,          NULL,          PREC_NONE},
  [TOKEN_CLASS] =             {NULL,          NULL,          PREC_NONE},

  [TOKEN_ADD] =               {NULL,          Binary,        PREC_TERM},
  [TOKEN_MINUS] =             {Unary,         Binary,        PREC_TERM},
  [TOKEN_MUL] =               {NULL,          Binary,        PREC_FACTOR},
  [TOKEN_DIV] =               {NULL,          Binary,        PREC_FACTOR},
  [TOKEN_NUM_DIV] =           {NULL,          Binary,        PREC_SPECIAL_DIV},
  [TOKEN_MODULE] =            {NULL,          Binary,        PREC_SPECIAL_DIV},
  [TOKEN_POWER] =             {NULL,          Binary,        PREC_POWER},

  [TOKEN_OR] =                {NULL,          NULL,          PREC_BIT_OP},
  [TOKEN_AND] =               {NULL,          NULL,          PREC_BIT_OP},
  [TOKEN_BITWISE] =           {NULL,          NULL,          PREC_BIT_OP},
  [TOKEN_XOR] =               {NULL,          NULL,          PREC_BIT_OP},
  [TOKEN_LEFT_SHIFT] =        {NULL,          NULL,          PREC_BIT_OP},
  [TOKEN_RIGHT_SHIFT] =       {NULL,          NULL,          PREC_BIT_OP},

  [TOKEN_NOT] =               {Unary,         NULL,          PREC_UNARY},
  [TOKEN_OR_OR] =             {NULL,          NULL,          PREC_OR},
  [TOKEN_AND_AND] =           {NULL,          NULL,          PREC_AND},
  [TOKEN_EQUAL_EQUAL] =       {NULL,          NULL,          PREC_EQUALITY},
  [TOKEN_NOT_EQUAL] =         {NULL,          NULL,          PREC_EQUALITY},
  [TOKEN_LESS] =              {NULL,          NULL,          PREC_COMPARISON},
  [TOKEN_LESS_EQUAL] =        {NULL,          NULL,          PREC_COMPARISON},
  [TOKEN_BIGGER] =            {NULL,          NULL,          PREC_COMPARISON},
  [TOKEN_BIGGER_EQUAL] =      {NULL,          NULL,          PREC_COMPARISON},

  [TOKEN_LEFT_PARENTHESIS] =  {Grouping,      NULL,          PREC_NONE},
  [TOKEN_RIGHT_PARENTHESIS] = {NULL,          NULL,          PREC_NONE},
  [TOKEN_LEFT_BRACKET] =      {NULL,          NULL,          PREC_NONE},
  [TOKEN_RIGHT_BRACKET] =     {NULL,          NULL,          PREC_NONE},
  [TOKEN_LEFT_BRACE] =        {NULL,          NULL,          PREC_NONE},
  [TOKEN_RIGHT_BRACE] =       {NULL,          NULL,          PREC_NONE},

  [TOKEN_IDENTIFY] =          {NULL,          NULL,          PREC_PRIMARY},
  [TOKEN_NUMBER] =            {Number,        NULL,          PREC_PRIMARY},
  [TOKEN_STRING] =            {NULL,          NULL,          PREC_PRIMARY},
  [TOKEN_NILL] =              {NULL,          NULL,          PREC_PRIMARY},
  [TOKEN_TRUE] =              {NULL,          NULL,          PREC_PRIMARY},
  [TOKEN_FALSE] =             {NULL,          NULL,          PREC_PRIMARY},

  [TOKEN_EQUAL] =             {NULL,          NULL,          PREC_ASSIGNMENT},
  [TOKEN_COMMA] =             {NULL,          NULL,          PREC_NONE},
  [TOKEN_DOT] =               {NULL,          NULL,          PREC_NONE},
  [TOKEN_SEMICOLON] =         {NULL,          NULL,          PREC_NONE},
  [TOKEN_EOL] =               {NULL,          NULL,          PREC_NONE},
  [TOKEN_EOF] =               {NULL,          NULL,          PREC_NONE}
};



bool Compile(Chunk* currentChunk, const char* source) {
  currentCompilerChunk = currentChunk;
  InitScanner(source);
  parser.hadError = false;
  parser.panicMode = false;
  Advance();

  ParsePrecedence(PREC_ASSIGNMENT);
  Consume(TOKEN_EOF, "Expect EOF at the end");
  EmitReturn();
  return !parser.hadError;
}



static void ParseErrorAt(Token* token, const char* errorMessage) {
  if (parser.panicMode) return;
  parser.panicMode = true;

  fprintf(stderr, "[Line %d] Error", token->line);

  switch (token->type) {
    case TOKEN_EOF:
      fprintf(stderr, " at the end:");
    case TOKEN_ERROR:
      break;

    default:
      fprintf(stderr, " at '%.*s':", token->length, token->start);
  };

  fprintf(stderr, " %s.\n", errorMessage);
  parser.hadError = true;
}

static void ParseErrorAtCurr(const char* errorMessage) {
  ParseErrorAt(&parser.previous, errorMessage);
}



static inline ParseRules* GetRule(TokenType type) {
  return &rules[type];
}

static void Advance() {
  parser.previous = parser.current;

  while (parser.current.type != TOKEN_EOF) {
    parser.current = ScanToken();
    if (parser.current.type != TOKEN_ERROR && parser.current.type != TOKEN_EOL)
      return;

    if (parser.current.type == TOKEN_ERROR)
      parser.hadError = true;
  }
}

static void Consume(TokenType type, const char* errorMessage) {
  if (parser.current.type == type) {
    Advance();
    return;
  }

  ParseErrorAtCurr(errorMessage);
}

static void ParsePrecedence(Precedence precedence) {
  Advance();
  ParseFn prefixRule = GetRule(parser.previous.type)->prefix;
  if (null prefixRule) {
    ParseErrorAtCurr("Expect expression");
    return;
  }
  prefixRule();

  while (precedence <= GetRule(parser.current.type)->precedence) {
    Advance();
    ParseFn infixRule = GetRule(parser.previous.type)->infix;
    if (null infixRule) {
      ParseErrorAtCurr("Expect operator");
      return;
    }
    infixRule();
  }
}



static void EmitByte(uint8_t opcode) {
  WriteChunk(currentCompilerChunk, parser.previous.line, opcode);
}

static void EmitConstant(Value value) {
  EmitByte(OP_CONSTANT);
  EmitByte(AddConstant(currentCompilerChunk, value));
}

static void EmitReturn() {
  EmitByte(OP_RETURN);
}

static void Number() {
  EmitConstant(MAKE_NUMBER(strtod(parser.previous.start, NULL)));
}

/*
TODO: Finish this later
static void String() {
  EmitConstant(MAKE_STRING(parser.previous.start, parser.previous.length));
}
*/

static void Grouping() {
  ParsePrecedence(PREC_ASSIGNMENT);
  Consume(TOKEN_RIGHT_PARENTHESIS, "Expect ')' after expression");
}

static void Unary() {
  TokenType operatorType = parser.previous.type;
  ParsePrecedence(PREC_UNARY);

  switch (operatorType) {
    case TOKEN_MINUS:
      EmitByte(OP_NEGATIVE);
      return;

    case TOKEN_NOT:
      EmitByte(OP_NOT);
      return;

    default:
      return;
  };
}

static void Binary() {
  TokenType operatorType = parser.previous.type;
  ParsePrecedence((GetRule(operatorType)->precedence) + 1);

  switch (operatorType) {
    case TOKEN_NUM_DIV: EmitByte(OP_NUM_DIV); break;
    case TOKEN_MODULE:  EmitByte(OP_MODULE);  break;
    case TOKEN_ADD:     EmitByte(OP_ADD);     break;
    case TOKEN_MINUS:   EmitByte(OP_SUB);     break;
    case TOKEN_MUL:     EmitByte(OP_MUL);     break;
    case TOKEN_DIV:     EmitByte(OP_DIV);     break;
    case TOKEN_POWER:   EmitByte(OP_POWER);   break; 

    default:
      return;
  };
}
