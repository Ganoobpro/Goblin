#include "compiler.h"

Parser parser;
Chunk* currentCompilerChunk;



static void ParseErrorAt(Token* token, const char* errorMessage);
static void ParseErrorAtCurr(const char* errorMessage);
static inline void EqualError();

static inline ParseRules* GetRule(TokenType type);
static void Advance();
static void ParsePrecedence(Precedence precedence);
static inline void Statement();
static void ParseUntil(TokenType type, const char* errorMessage);
static inline void ParseAll();
static void Consume(TokenType type, const char* errorMessage);
static bool TypeMatch(TokenType type);

static void EmitByte(uint8_t instruction);
static void EmitBytes(uint8_t instruction1, uint8_t instruction2);
static uint32_t EmitJump(uint8_t instruction);
static void EmitConstant(Value value);
static ConstIndex GetVarIndex();
static void EmitReturn();

static void Number();
static void String();
static void Grouping();
static void CodeChunk();
static void Unary();
static void Variable();
static void PatchJump(uint32_t offset);
static void IfElse();

static void Operator();



ParseRules rules[] = {
  [TOKEN_IF] =                {IfElse,        NULL,          PREC_ASSIGNMENT},
  [TOKEN_ELSE] =              {NULL,          NULL,          PREC_NONE},
  [TOKEN_FOR] =               {NULL,          NULL,          PREC_ASSIGNMENT},
  [TOKEN_WHILE] =             {NULL,          NULL,          PREC_ASSIGNMENT},
  [TOKEN_FUNC] =              {NULL,          NULL,          PREC_ASSIGNMENT},
  [TOKEN_CLASS] =             {NULL,          NULL,          PREC_ASSIGNMENT},

  [TOKEN_ADD] =               {NULL,          Operator,      PREC_TERM},
  [TOKEN_MINUS] =             {Unary,         Operator,      PREC_TERM},
  [TOKEN_MUL] =               {NULL,          Operator,      PREC_FACTOR},
  [TOKEN_DIV] =               {NULL,          Operator,      PREC_FACTOR},
  [TOKEN_NUM_DIV] =           {NULL,          Operator,      PREC_SPECIAL_DIV},
  [TOKEN_MODULE] =            {NULL,          Operator,      PREC_SPECIAL_DIV},
  [TOKEN_POWER] =             {NULL,          Operator,      PREC_POWER},

  [TOKEN_OR] =                {NULL,          Operator,      PREC_BIT_OP},
  [TOKEN_AND] =               {NULL,          Operator,      PREC_BIT_OP},
  [TOKEN_BITWISE] =           {NULL,          Operator,      PREC_BIT_OP},
  [TOKEN_XOR] =               {NULL,          Operator,      PREC_BIT_OP},
  [TOKEN_LEFT_SHIFT] =        {NULL,          Operator,      PREC_BIT_OP},
  [TOKEN_RIGHT_SHIFT] =       {NULL,          Operator,      PREC_BIT_OP},

  [TOKEN_OR_OR] =             {NULL,          Operator,      PREC_OR},
  [TOKEN_AND_AND] =           {NULL,          Operator,      PREC_AND},
  [TOKEN_EQUAL_EQUAL] =       {NULL,          Operator,      PREC_EQUALITY},
  [TOKEN_NOT_EQUAL] =         {NULL,          Operator,      PREC_EQUALITY},
  [TOKEN_LESS] =              {NULL,          Operator,      PREC_COMPARISON},
  [TOKEN_LESS_EQUAL] =        {NULL,          Operator,      PREC_COMPARISON},
  [TOKEN_BIGGER] =            {NULL,          Operator,      PREC_COMPARISON},
  [TOKEN_BIGGER_EQUAL] =      {NULL,          Operator,      PREC_COMPARISON},
  [TOKEN_NOT] =               {Unary,         NULL,          PREC_UNARY},

  [TOKEN_LEFT_PARENTHESIS] =  {Grouping,      NULL,          PREC_NONE},
  [TOKEN_RIGHT_PARENTHESIS] = {NULL,          NULL,          PREC_NONE},
  [TOKEN_LEFT_BRACKET] =      {NULL,          NULL,          PREC_NONE},
  [TOKEN_RIGHT_BRACKET] =     {NULL,          NULL,          PREC_NONE},
  [TOKEN_LEFT_BRACE] =        {CodeChunk,     NULL,          PREC_NONE},
  [TOKEN_RIGHT_BRACE] =       {NULL,          NULL,          PREC_NONE},

  [TOKEN_IDENTIFY] =          {Variable,      NULL,          PREC_PRIMARY},
  [TOKEN_NUMBER] =            {Number,        NULL,          PREC_PRIMARY},
  [TOKEN_STRING] =            {NULL,          NULL,          PREC_PRIMARY},
  [TOKEN_NILL] =              {NULL,          NULL,          PREC_PRIMARY},
  [TOKEN_TRUE] =              {NULL,          NULL,          PREC_PRIMARY},
  [TOKEN_FALSE] =             {NULL,          NULL,          PREC_PRIMARY},

  [TOKEN_EQUAL] =             {NULL,          EqualError,    PREC_ASSIGNMENT},
  [TOKEN_COMMA] =             {NULL,          NULL,          PREC_NONE},
  [TOKEN_DOT] =               {NULL,          NULL,          PREC_NONE},
  [TOKEN_SEMICOLON] =         {NULL,          NULL,          PREC_NONE},
  [TOKEN_EOL] =               {NULL,          NULL,          PREC_NONE},
  [TOKEN_EOF] =               {NULL,          NULL,          PREC_NONE}
};



bool Compile(Chunk* currentChunk, const char* source) {
  currentCompilerChunk = currentChunk;
  parser.hadError = false;
  parser.panicMode = false;

  InitScanner(source);
  Advance();
  ParseAll();
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

static inline void EqualError() {
  ParseErrorAtCurr("Unexpect '='. Did you mean '=='?");
}



static inline ParseRules* GetRule(TokenType type) {
  return &rules[type];
}

static void Advance() {
  while (parser.current.type != TOKEN_EOF) {
    parser.previous = parser.current;
    parser.current = ScanToken();

    if (parser.current.type != TOKEN_ERROR)
      return;
    parser.hadError = true;
  }
}

static void ParsePrecedence(Precedence precedence) {
  Advance();
  ParseFn prefixRule = GetRule(parser.previous.type)->prefix;
  if (null prefixRule) {
    ParseErrorAtCurr("Expect expression");
    return;
  }
  prefixRule();

  // Exception
  if (
    parser.previous.type == TOKEN_RIGHT_PARENTHESIS ||
    parser.previous.type == TOKEN_RIGHT_BRACE ||
    parser.previous.type == TOKEN_SEMICOLON ||
    parser.previous.type == TOKEN_EOL
  ) {
    return;
  }

  while (precedence <= GetRule(parser.current.type)->precedence) {
    PrintToken(&parser.previous);
    Advance();
    printf("-----");
    PrintToken(&parser.previous);
    printf("----------");
    PrintToken(&parser.current);
    ParseFn infixRule = GetRule(parser.previous.type)->infix;
    if (null infixRule) {
      ParseErrorAtCurr("Expect operator");
      return;
    }
    infixRule();
  }
}

static void Statement() {
#define REACH_END_OF_COMMAND (parser.current.type == TOKEN_SEMICOLON || parser.current.type == TOKEN_EOL)

  while (REACH_END_OF_COMMAND) {
    Advance();
  }

  if (TypeMatch(TOKEN_EOF)) {
    return;
  }

  ParsePrecedence(PREC_ASSIGNMENT);
}

static void ParseUntil(TokenType type, const char* errorMessage) {
  if (TypeMatch(type))
    return;

#define FIND_TYPE (parser.current.type == TOKEN_EOF || parser.current.type == type)

  until (FIND_TYPE) {
    while (REACH_END_OF_COMMAND) {
      Advance();
    }

    if (FIND_TYPE) {
      break;
    }

    ParsePrecedence(PREC_ASSIGNMENT);
  }

  Consume(type, errorMessage);

#undef REACH_END_OF_COMMAND
#undef FIND_TYPE
}

static inline void ParseAll() {
  until (parser.current.type == TOKEN_EOF) {
    Statement();
  }
}

static void Consume(TokenType type, const char* errorMessage) {
  if (parser.current.type == type) {
    Advance();
    return;
  }

  ParseErrorAtCurr(errorMessage);
}

static bool TypeMatch(TokenType type) {
  if (parser.current.type != type)
    return false;

  Advance();
  return true;
}



static void EmitByte(uint8_t instruction) {
  WriteChunk(currentCompilerChunk, parser.previous.line, instruction);
}

static void EmitBytes(uint8_t instruction1, uint8_t instruction2) {
  EmitByte(instruction1);
  EmitByte(instruction2);
}

static uint32_t EmitJump(uint8_t instruction) {
  EmitByte(instruction);
  EmitByte(0xff);
  EmitByte(0xff);
  return currentCompilerChunk->counter - 2;
}

static void EmitConstant(Value value) {
  EmitBytes(
    OP_CONSTANT,
    AddConstant(currentCompilerChunk, value)
  );
}

static ConstIndex GetVarIndex() {
  return AddConstant(
    currentCompilerChunk,
    MAKE_STRING(parser.previous.start, parser.previous.length)
  );
}

static void EmitReturn() {
  EmitByte(OP_RETURN);
}



static void Number() {
  EmitConstant(MAKE_NUMBER(strtod(parser.previous.start, NULL)));
}

static void String() {
  EmitConstant(MAKE_STRING(parser.previous.start, parser.previous.length));
}

static void Grouping() {
  ParseUntil(TOKEN_RIGHT_PARENTHESIS, "Expect ')' after expression");
}

static void CodeChunk() {
  ParseUntil(TOKEN_RIGHT_BRACE, "Expect '}' after expression");
}

static void Unary() {
  TokenType operatorType = parser.previous.type;
  ParsePrecedence(PREC_UNARY);

  switch (operatorType) {
    case TOKEN_MINUS: EmitByte(OP_NEGATIVE); return;
    case TOKEN_NOT:   EmitByte(OP_NOT); return;

    default:
      return;
  };
}

static void Variable() {
  ConstIndex idIndex = GetVarIndex();

  if (TypeMatch(TOKEN_EQUAL)) {
    Statement();
    EmitBytes(OP_DECLARE_VAR, idIndex);
  } else {
    EmitBytes(OP_GET_VAR, idIndex);
  }
}

static void PatchJump(uint32_t offset) {
  uint32_t jumpStep = currentCompilerChunk->counter - 2 - offset;

  if (jumpStep > UINT16_MAX)
    ParseErrorAtCurr("Too much code to jump over");

  // jumpStep is 16-bit number
  // however each code only have 8-bit
  // -> must split jumpStep
  currentCompilerChunk->code[offset] = (jumpStep >> 8) & 0xff;
  currentCompilerChunk->code[offset+1] = jumpStep & 0xff;
}

static void IfElse() {
  // If
  Statement(); // Get condition
  uint32_t jump = EmitJump(OP_JUMP_COND);
  uint32_t elseJump = EmitJump(OP_JUMP);
  PatchJump(jump);
  Statement(); // Get line/chunk of code

  // Else
  TypeMatch(TOKEN_SEMICOLON);
  while (parser.current.type == TOKEN_EOL) {
    Advance();
  }

  if (TypeMatch(TOKEN_ELSE)) {
    jump = EmitJump(OP_JUMP);
    PatchJump(elseJump);
    Statement(); // Get else line/chunk of code
    PatchJump(jump);
  }
}



OpCode operatorRules[] = {
  [TOKEN_ADD] =               OP_ADD,
  [TOKEN_MINUS] =             OP_SUB,
  [TOKEN_MUL] =               OP_MUL,
  [TOKEN_DIV] =               OP_DIV,
  [TOKEN_NUM_DIV] =           OP_NUM_DIV,
  [TOKEN_MODULE] =            OP_MODULE,
  [TOKEN_POWER] =             OP_POWER,

  [TOKEN_OR] =                OP_OR,
  [TOKEN_AND] =               OP_AND,
  [TOKEN_BITWISE] =           OP_BITWISE,
  [TOKEN_XOR] =               OP_XOR,
  [TOKEN_LEFT_SHIFT] =        OP_LEFT_SHIFT,
  [TOKEN_RIGHT_SHIFT] =       OP_RIGHT_SHIFT,

  [TOKEN_NOT] =               OP_NOT,
  [TOKEN_OR_OR] =             OP_OR_OR,
  [TOKEN_AND_AND] =           OP_AND_AND,
  [TOKEN_EQUAL_EQUAL] =       OP_EQUAL_EQUAL,
  [TOKEN_NOT_EQUAL] =         OP_NOT_EQUAL,
  [TOKEN_LESS] =              OP_LESS,
  [TOKEN_LESS_EQUAL] =        OP_LESS_EQUAL,
  [TOKEN_BIGGER] =            OP_BIGGER,
  [TOKEN_BIGGER_EQUAL] =      OP_BIGGER_EQUAL,
};

static void Operator() {
  TokenType operatorType = parser.previous.type;
  ParsePrecedence((GetRule(operatorType)->precedence) + 1);
  EmitByte(operatorRules[operatorType]);
}
