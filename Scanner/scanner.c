#include "scanner.h"

Scanner scanner;
bool IsAtTheEnd();
bool Match(const char targetChar);

void InitScanner(const char* source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
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

  // Skip
  if (*scanner.current == '\0')
    return MakeToken(TOKEN_EOF);

  while (*scanner.current == '\t' || *scanner.current == ' ' ||
         *scanner.current == '\n' || *scanner.current == '#' ||
        (*scanner.current == '/'  && scanner.current[1] == '*'))
  {
    if (*scanner.current == '\n')
      scanner.line++;

    // Comment
    if (*scanner.start == '#')
      while (!IsAtTheEnd() && *scanner.current++ != '\n') {}
    else if (*scanner.start == '/' || scanner.start[1] == '*')
      while (!IsAtTheEnd() && (*scanner.current++ != '*' || *scanner.current != '/')) {}
    else
      scanner.current++;

    scanner.start = scanner.current;
  }



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

    // Others
    case ',': return MakeToken(TOKEN_COMMA);
    case '.': return MakeToken(TOKEN_DOT);
    case ';': return MakeToken(TOKEN_SEMICOLON);

    default:
      ErrorChar(scanner.current[-1], scanner.line);
      return MakeToken(TOKEN_ERROR);
  }
}



bool IsAtTheEnd() {
  return *scanner.current == '\0';
}

bool Match(const char targetChar) {
  if (IsAtTheEnd()) return false;
  if (*scanner.current == targetChar) {
    scanner.current++;
    return true;
  }

  return false;
}
