#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "lexer.hpp"
#include "logger.hpp"
#include "main.hpp"
#include "symbol.hpp"
#include "parser.hpp"
#include "error.hpp"

extern bool isInGlobalScope;
int yyparse();
