#include "scanner.h"

Scanner scanner;
static Token MakeToken(TokenType type);
static bool SkipUnnecessary();
static bool Match(const char targetChar);
static Token TokenString();
static Token TokenNumber();
static Token Trie();

#define Peek           (scanner.current[-1])
#define PeekNext       (*scanner.current)
#define IsAtTheEnd     (Peek == '\0')
#define IsNearTheEnd   (PeekNext == '\0')
#define IsNumber       (between('0',Peek,'9'))
#define IsNearNumber   (between('0',PeekNext,'9'))
#define IsAlphabet     (between('a',Peek,'z') || between('A',Peek,'Z'))
#define IsNearAlphabet (between('a',PeekNext,'z') || between('A',PeekNext,'Z'))

void InitScanner(const char* source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

void PrintToken(Token* token) {
  printf("[Token] %.*s %i\n", token->length, token->start, token->type);
}

Token ScanToken() {
  scanner.start = scanner.current;
  if (IsAtTheEnd)
    return MakeToken(TOKEN_EOF);

  if (SkipUnnecessary())
    return MakeToken(TOKEN_EOL);

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
    case ';': return MakeToken(TOKEN_SEMICOLON);
    case '.':
      if (IsNearNumber) {
        while (IsNumber) {scanner.current++;}
        return MakeToken(TOKEN_NUMBER);
      }
      return MakeToken(TOKEN_DOT);

    default:
      if (IsNumber)
        return TokenNumber();
      elif (IsAlphabet)
        return Trie();

      ErrorChar(*scanner.start, scanner.line);
      return MakeToken(TOKEN_ERROR);
  }
}



static Token MakeToken(TokenType type) {
  return (Token){
    .start = scanner.start,
    .length = scanner.current-scanner.start,
    .type = type,
    .line = scanner.line
  };
}

static bool SkipUnnecessary() {
  bool endline = false;

  while (*scanner.current == '\t' || *scanner.current == ' ' ||
         *scanner.current == '\n' || *scanner.current == '#' ||
        (*scanner.current == '/'  && scanner.current[1] == '*'))
  {
    if (*scanner.current == '\n') {
      scanner.line++;
      endline = true;
    }

    // Comment
    if (*scanner.current == '#') {
      until (IsAtTheEnd || *scanner.current++ == '\n') {}
      scanner.line++;
    } else if (*scanner.current == '/' && scanner.current[1] == '*')
      until (IsAtTheEnd || (*scanner.current++ == '*' && *scanner.current == '/')) {}

    ifnot (IsAtTheEnd)
      scanner.current++;
  }

  scanner.start = scanner.current;
  return endline;
}

static bool Match(const char targetChar) {
  if (IsAtTheEnd) return false;
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
  until (IsAtTheEnd || *scanner.current++ == *scanner.start) {}

  if (IsAtTheEnd) {
    Error("Unfinished string.", scanner.line);
    return MakeToken(TOKEN_ERROR);
  }

  return MakeToken(TOKEN_STRING);
}

static Token TokenNumber() {
  // TODO: Add e, 0x, 0b
  while (!IsAtTheEnd && IsNearNumber) {scanner.current++;}
  if (PeekNext == '.')
    do {scanner.current++;} while (!IsAtTheEnd && IsNearNumber);

  return MakeToken(TOKEN_NUMBER);
}

static Token Identifier() {
  while (!IsAtTheEnd && IsNearAlphabet) {scanner.current++;}
  return MakeToken(TOKEN_IDENTIFY);
}

static inline bool CompareTheRest(const uint8_t start, const uint8_t restLen,
                                  const char* rest)
{
  if (memcmp(scanner.current+start-1, rest, restLen) == 0) {
    scanner.current += start+restLen-1;
    return true;
  }

  return false;
}

static Token Trie() {
  switch (Peek) {
    case 'a':
      if (CompareTheRest(1, 2, "nd")) return MakeToken(TOKEN_AND_AND);
    case 'o':
      if (CompareTheRest(1, 1, "r")) return MakeToken(TOKEN_OR_OR);
    case 'm':
      if (CompareTheRest(1, 4, "od")) return MakeToken(TOKEN_MODULE);
    case 'i':
      if (CompareTheRest(1, 1, "f")) return MakeToken(TOKEN_IF);
    case 'e':
      if (CompareTheRest(1, 3, "lse")) return MakeToken(TOKEN_ELSE);
    case 'w':
      if (CompareTheRest(1, 4, "hile")) return MakeToken(TOKEN_WHILE);
    case 'c':
      if (CompareTheRest(1, 4, "lass")) return MakeToken(TOKEN_CLASS);
    case 't':
      if (CompareTheRest(1, 3, "rue")) return MakeToken(TOKEN_TRUE);

    case 'n':
      switch (PeekNext) {
        case 'o':
          if (CompareTheRest(2, 1, "t")) return MakeToken(TOKEN_NOT);
        case 'i':
          if (CompareTheRest(2, 3, "ill")) return MakeToken(TOKEN_NILL);
      }
    case 'f':
      switch (PeekNext) {
        case 'a':
          if (CompareTheRest(2, 3, "lse")) return MakeToken(TOKEN_FALSE);
        case 'o':
          if (CompareTheRest(2, 1, "r")) return MakeToken(TOKEN_FOR);
        case 'u':
          if (CompareTheRest(2, 1, "n")) return MakeToken(TOKEN_FUNC);
          if (CompareTheRest(2, 2, "nc")) return MakeToken(TOKEN_FUNC);
          if (CompareTheRest(2, 6, "nction")) return MakeToken(TOKEN_FUNC);
      }
    default:
      return Identifier();
  }
}

#undef Peek
#undef PeekNext
#undef IsAtTheEnd
#undef IsNearTheEnd
#undef IsNumber
#undef IsNearNumber
#undef IsAlphabet
#undef IsNearAlphabet
