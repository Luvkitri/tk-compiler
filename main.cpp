#include "global.hpp"

ofstream outputStream;

int main(int argc, char *argv[]) {
  const char *programName = argv[0];
  const char *path = argv[1];

  if (argc < 2) {
    log("Missing key argument: <path>");
    return 1;
  }

  yyin = fopen(path, "r");

  if (yyin == nullptr) {
    string filePath = path;
    errorLog("The file at path: " + filePath + " does not exists");
    return 1;
  }

  outputStream.open("output.asm", ios::trunc);
  if (!outputStream.is_open()) {
    errorLog("Failed to open output file");
    return 1;
  }

  yyparse();

  // Clear symbol table

  // Close streams
  outputStream.close();
  fclose(yyin);

  yylex_destroy();

  return 0;
}