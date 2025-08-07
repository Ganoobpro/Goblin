///////////// PREVIOUS = CURRENT, CURRENT = NEXT
#include "scanner.h"

Scanner scanner;
static inline bool IsAtTheEnd();
static inline bool IsNumber();
static inline bool IsAlphabet();
static void SkipUnnecessary();
static bool Match(const char targetChar);
static Token TokenString();
static Token TokenNumber();

void InitScanner(const char* source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

void PrintToken(Token* token) {
  printf("[Token] %.*s %i\n", token->length, token->start, token->type);
}

Token MakeToken(TokenType type) {
  return (Token){
    .start = scanner.start,
    .length = scanner.current-scanner.start,
    .type = type
  };
}

Token ScanToken() {
  scanner.start = scanner.current;
  if (IsAtTheEnd())
    return MakeToken(TOKEN_EOF);

  SkipUnnecessary();

  // Switch tree
  switch (*scanner.current++) {
    // End of file
    case '\0': return MakeToken(TOKEN_EOF);

    // Operators
    case '+': return MakeToken(TOKEN_ADD);
    case '-': return MakeToken(TOKEN_MINUS);
    case '%': return MakeToken(TOKEN_MODULE);
    case '*':
      return when Match('*') then MakeToken(TOKEN_POWER)
                             other MakeToken(TOKEN_MUL);
    case '/':
      return when Match('/') then MakeToken(TOKEN_NUM_DIV)
                             other MakeToken(TOKEN_DIV);

    case '~': return MakeToken(TOKEN_BITWISE);
    case '^': return MakeToken(TOKEN_XOR);
    case '|':
      return when Match('|') then MakeToken(TOKEN_OR_OR)
                             other MakeToken(TOKEN_OR);
    case '&':
      return when Match('&') then MakeToken(TOKEN_AND_AND)
                             other MakeToken(TOKEN_AND);
    case '=':
      return when Match('=') then MakeToken(TOKEN_EQUAL_EQUAL)
                             other MakeToken(TOKEN_EQUAL);
    case '!':
      return when Match('=') then MakeToken(TOKEN_NOT_EQUAL)
                             other MakeToken(TOKEN_NOT);
    case '<':
      return when Match('<') then MakeToken(TOKEN_LEFT_SHIFT)
             other Match('=') then MakeToken(TOKEN_LESS_EQUAL)
                             other MakeToken(TOKEN_LESS);
    case '>':
      return when Match('>') then MakeToken(TOKEN_RIGHT_SHIFT)
             other Match('=') then MakeToken(TOKEN_BIGGER_EQUAL)
                             other MakeToken(TOKEN_BIGGER);

    // Grouping
    case '(': return MakeToken(TOKEN_LEFT_PARENTHESIS);
    case ')': return MakeToken(TOKEN_RIGHT_PARENTHESIS);
    case '[': return MakeToken(TOKEN_LEFT_BRACKET);
    case ']': return MakeToken(TOKEN_RIGHT_BRACKET);
    case '{': return MakeToken(TOKEN_LEFT_BRACE);
    case '}': return MakeToken(TOKEN_RIGHT_BRACE);

    // Literals
    case '\"':
    case '\'':
      return TokenString();

    // Others
    case ',': return MakeToken(TOKEN_COMMA);
    case '.': return MakeToken(TOKEN_DOT);
    case ';': return MakeToken(TOKEN_SEMICOLON);

    default:
      ErrorChar(*scanner.start, scanner.line);
      return MakeToken(TOKEN_ERROR);
  }
}



static inline bool IsAtTheEnd() {
  return scanner.current[-1] == '\0';
}

static inline bool IsNumber() {
  return between('0', scanner.current[-1], '9');
}

static inline bool IsAlphabet() {
  return (between('a', scanner.current[-1], 'z') ||
          between('A', scanner.current[-1], 'Z'));
}



static void SkipUnnecessary() {
  while (*scanner.current == '\t' || *scanner.current == ' ' ||
         *scanner.current == '\n' || *scanner.current == '#' ||
        (*scanner.current == '/'  && scanner.current[1] == '*'))
  {
    if (*scanner.current == '\n')
      scanner.line++;

    // Comment
    if (*scanner.current == '#') {
      until (IsAtTheEnd() || *scanner.current++ == '\n') {}
      scanner.line++;
    } else if (*scanner.current == '/' && scanner.current[1] == '*')
      until (IsAtTheEnd() || (*scanner.current++ == '*' && *scanner.current == '/')) {}

    ifnot (IsAtTheEnd())
      scanner.current++;
  }

  scanner.start = scanner.current;
}

static bool Match(const char targetChar) {
  if (IsAtTheEnd()) return false;
  if (*scanner.current == targetChar) {
    scanner.current++;
    return true;
  }

  return false;
}

// TODO: Add built-in format-string
static Token TokenString() {
  // If start = ", must end with "
  // If start = ', must end with '
  while (!IsAtTheEnd() && *scanner.current++ != *scanner.start) {}

  if (IsAtTheEnd()) {
    Error("Unfinished string.", scanner.line);
    return MakeToken(TOKEN_ERROR);
  }

  return MakeToken(TOKEN_STRING);
}

static Token TokenNumber() {
  // TODO: Add e, 0x, 0b
  return MakeToken(TOKEN_NUMBER);
}
