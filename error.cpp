#include "global.hpp"

void yyerror(const char *message) {
  cerr << "line: " << yylineno << " error: " << message << endl;
}