#include "global.hpp"

ofstream outputStream;
SymbolTable symbolTable;
bool isInGlobalScope = true;

int main(int argc, char *argv[]) {
  yydebug = 1;
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

  // Display symbol table
  symbolTable.display();
  writeToFile();

  // Close streams
  outputStream.close();
  fclose(yyin);

  yylex_destroy();

  return 0;
}