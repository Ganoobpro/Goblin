#include "compiler.h"
#include "../Scanner/scanner.h"

//////////  Parser  //////////
Parser parser;

void InitParser() {
  parser.hadError = false;
  parser.panicMode = false;
}

void Compiler() {
  parser.previous = parser.current;
}

static void Consume() {

}



//////////  Parser Rules  //////////
ParserRules rules[] = {
  // COMMANDS
  [TOKEN_IF] =                {NULL,          NULL,          NULL},
  [TOKEN_ELSE] =              {NULL,          NULL,          NULL},
  [TOKEN_FOR] =               {NULL,          NULL,          NULL},
  [TOKEN_WHILE] =             {NULL,          NULL,          NULL},
  [TOKEN_FUNC] =              {NULL,          NULL,          NULL},
  [TOKEN_CLASS] =             {NULL,          NULL,          NULL},
                              {NULL,          NULL,          NULL}, 
  // OPERATORS
  // Math operators
  [TOKEN_ADD] =               {NULL,          NULL,          NULL},
  [TOKEN_MINUS] =             {NULL,          NULL,          NULL},
  [TOKEN_MUL] =               {NULL,          NULL,          NULL},
  [TOKEN_DIV] =               {NULL,          NULL,          NULL},
  [TOKEN_NUM_DIV] =           {NULL,          NULL,          NULL},
  [TOKEN_MODULE] =            {NULL,          NULL,          NULL},
  [TOKEN_POWER] =             {NULL,          NULL,          NULL},
  // Bit operators
  [TOKEN_OR] =                {NULL,          NULL,          NULL},
  [TOKEN_AND] =               {NULL,          NULL,          NULL},
  [TOKEN_BITWISE ] =          {NULL,          NULL,          NULL},
  [TOKEN_XOR] =               {NULL,          NULL,          NULL},
  [TOKEN_LEFT_SHIFT] =        {NULL,          NULL,          NULL},
  [TOKEN_RIGHT_SHIFT] =       {NULL,          NULL,          NULL},
  // Condition
  [TOKEN_OR_OR] =             {NULL,          NULL,          NULL},
  [TOKEN_AND_AND] =           {NULL,          NULL,          NULL},
  [TOKEN_EQUAL_EQUAL] =       {NULL,          NULL,          NULL},
  [TOKEN_EQUAL] =             {NULL,          NULL,          NULL},
  [TOKEN_LESS] =              {NULL,          NULL,          NULL},
  [TOKEN_LESS_EQUAL] =        {NULL,          NULL,          NULL},
  [TOKEN_BIGGER] =            {NULL,          NULL,          NULL},
  [TOKEN_BIGGER_EQUAL] =      {NULL,          NULL,          NULL},
  [TOKEN_NOT] =               {NULL,          NULL,          NULL},
  [TOKEN_NOT_EQUAL] =         {NULL,          NULL,          NULL},

  // GROUPING
  [TOKEN_LEFT_PARENTHESIS] =  {NULL,          NULL,          NULL},
  [TOKEN_RIGHT_PARENTHESIS] = {NULL,          NULL,          NULL},
  [TOKEN_LEFT_BRACKET] =      {NULL,          NULL,          NULL},
  [TOKEN_RIGHT_BRACKET] =     {NULL,          NULL,          NULL},
  [TOKEN_LEFT_BRACE] =        {NULL,          NULL,          NULL},
  [TOKEN_RIGHT_BRACE] =       {NULL,          NULL,          NULL},

  // LITERALS
  [TOKEN_IDENTIFY] =          {NULL,          NULL,          NULL},
  [TOKEN_NUMBER] =            {NULL,          NULL,          NULL},
  [TOKEN_STRING] =            {NULL,          NULL,          NULL},
  [TOKEN_NILL] =              {NULL,          NULL,          NULL},
  [TOKEN_TRUE] =              {NULL,          NULL,          NULL},
  [TOKEN_FALSE] =             {NULL,          NULL,          NULL},

  // OTHERS
  [TOKEN_COMMA] =             {NULL,          NULL,          NULL},
  [TOKEN_DOT] =               {NULL,          NULL,          NULL},
  [TOKEN_SEMICOLON] =         {NULL,          NULL,          NULL},
  [TOKEN_EOL] =               {NULL,          NULL,          NULL}
};
