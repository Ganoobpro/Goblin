#include "scanner.h"

Scanner scanner;
bool IsAtTheEnd();
bool IsMatch(const char targetChar);

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

  switch (*scanner.current++) {
    // Skip white space
    case '\n':
      scanner.line++;
    case ' ':
    case '\t':
      break;
  }
}



bool IsAtTheEnd() {
  return scanner.current[1] == '\0';
}

bool IsMatch(const char targetChar) {
  if (IsAtTheEnd()) return false;
  if (scanner.current[1] == targetChar) {
    scanner.current++;
    return true;
  }

  return false;
}
