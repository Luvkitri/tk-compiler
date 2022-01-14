#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#include "error.hpp"
#include "lexer.hpp"
#include "logger.hpp"
#include "main.hpp"
#include "parser.hpp"
#include "symbol.hpp"
#include "emitter.hpp"

extern bool isInGlobalScope;
extern SymbolTable symTable;
extern ofstream outputStream;

// parser
int yyparse();
