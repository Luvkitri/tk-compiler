#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

#include "error.hpp"
#include "lexer.hpp"
#include "logger.hpp"
#include "main.hpp"
#include "parser.hpp"
#include "symbol.hpp"
#include "emitter.hpp"

extern bool commentsEnabled;
extern bool isInGlobalScope;
extern SymbolTable symbolTable;
extern ofstream outputStream;
extern FILE *yyin;

int yyparse();
