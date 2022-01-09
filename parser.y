%{
  #include "global.hpp"
%}

/* Set of essentials tokens */
%token T_PROGRAM
%token T_VARIABLE
%token T_INTEGER
%token T_REAL
%token T_BEGIN
%token T_END
%token T_ASSIGN
%token T_ID
%token T_DIV
%token T_MOD
%token T_NUM

%%
  program: %empty
%%