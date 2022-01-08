#include "global.hpp"

void log(string logMessage) { cout << "Log: " << logMessage << "\n"; }

void errorLog(string errorMessage) {
  cerr << "Error: " << errorMessage << "\n";
}