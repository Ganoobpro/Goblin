#include "config.h"

int main() {
  char* source = "  + - #";
  InitScanner(source);

  for(;;) {
    Token token = ScanToken();
    if (token.type == TOKEN_EOF) break;

    if (token.length == 1)
      fprintf(stdout, "Token: %c %i\n", *token.start, (int)token.type);
    else
      fprintf(stdout, "Token: %c%c %i\n", *token.start, token.start[1], (int)token.type);
  }

  return 0;
}
