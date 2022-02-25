#include "global.hpp"

ofstream outputStream;
SymbolTable symbolTable;
bool isInGlobalScope = true;
bool commentsEnabled = true;
// extern int yydebug;

int main(int argc, char *argv[]) {
  // yydebug = 1;

  if (argc < 2) {
    errorLog("Missing key argument: <inputPath>");
    return 1;
  } else if (argc < 3) {
    errorLog("Missing key argument: <outpathPath>");
    return 1;
  }

  const char *inputPath = argv[1];
  const char *outputPath = argv[2];

  yyin = fopen(inputPath, "r");

  if (yyin == nullptr) {
    string filePath = inputPath;
    errorLog("The file at path: " + filePath + " does not exists");
    return 1;
  }

  outputStream.open(outputPath, ios::trunc);

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