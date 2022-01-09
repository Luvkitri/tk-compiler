#include "global.hpp"

void yyerror(const char *message) { fprintf(stderr, "%s\n", message); }