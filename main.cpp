#include "global.hpp"

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
  }
}