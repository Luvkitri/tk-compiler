#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

#include "logger.hpp"
#include "main.hpp"
#include "symbol.hpp"
#include "parser.hpp"
#include "error.hpp"
#include "lexer.hpp"

extern bool isInGlobalScope;
extern SymbolTable symTable;
extern ofstream outputStream;

// parser
int yyparse();
