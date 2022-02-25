#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <csignal>

#define BREAKPOINT // raise(SIGINT);

using namespace std;

#include "error.hpp"
#include "emitter.hpp"
#include "lexer.hpp"
#include "logger.hpp"
#include "main.hpp"
#include "parser.hpp"
#include "symbol.hpp"

extern bool commentsEnabled;
extern bool isInGlobalScope;
extern SymbolTable symbolTable;
extern ofstream outputStream;
extern FILE *yyin;

int yyparse();
int passArguments(int);
