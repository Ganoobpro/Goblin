#include "config.h"

static void Repl() {
  char line[1024];
  forever {
    printf("> ");

    if (!fgets(line, sizeof(char), stdin)) {
      printf("\n");
      break;
    }

    interpreter(line);
  }
}

static char* ReadFile(const char* filename);
static void RunFile(const char* filename) {
  char* source = ReadFile(filename);
  InterpreterResult result = interpreter(source);
  free(source);

  if (result == INTERPRETER_COMPILE_ERROR) exit(65);
  if (result == INTERPRETER_RUNTIME_ERROR) exit(70);
}

int main(int args, char* argv[]) {
  printf("%d\n", between('0','a','9'));

  if (args == 1)
    Repl();
  elif (args == 2) {
    printf("----------  %s\n", argv[1]);
    RunFile(argv[1]);
  } else {
    fprintf(stderr, "Usage: goblin [filename]\n");
    exit(64);
  }

  return 0;
}

static char* ReadFile(const char* filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "[System] Error: %s doesn\'t exist.\n", filename);
    exit(66);
  }

  fseek(file, 0L, SEEK_END);          // Move to end of file
  long length = ftell(file);          // Get current position (size)
  rewind(file);                       // Move back to beginning

  char* buffer = (char*)malloc(length+1);
  if (buffer == NULL) {
    fprintf(stderr, "[System] Error: Out of memory.\n");
    exit(71);
  }

  size_t byteRead = fread(buffer, sizeof(char), length, file);
  buffer[byteRead] = '\0';

  printf("%s\n", buffer);

  fclose(file);
  return buffer;
}
