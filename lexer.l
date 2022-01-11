%{
  #include "global.hpp"
%}

/*
  Flex maintains the number of the current line read
*/
%option yylineno
/*
  If you do not supply your own version of yywrap(), then you must either use %option
  noyywrap (in which case the scanner behaves as though yywrap() returned 1), or you must
  link with ‘-lfl’ to obtain the default version of the routine, which always returns 1.
*/
%option noyywrap

DIGIT                             [0-9]
LETTER                            [a-zA-Z]
DIGITS                            {DIGIT}+
ID                                {LETTER}({LETTER}|{DIGIT})*
ADDOP                              "+"|"-"|"or"
MULOP                             "*"|"/"|"div"|"mod"|"and"
RELOP                             "="|"<>"|">"|">="|"<"|"<="
ASSIGNOP                          ":="


%%
[ \t\n]+                          {/* Skip whitespaces, tabs and new lines (those are maintained by yylineno) */}
"program"                         { return T_PROGRAM; }
"var"                             { return T_VAR; }
"begin"                           { return T_BEGIN; }
"end"                             { return T_END; }
{ADDOP}                           { yylval = getAddopToken(yytext); return T_ADDOP; }
{MULOP}                           { yylval = getMulopToken(yytext); return T_MULOP; }
{RELOP}                           { yylval = getRelopToken(yytext); return T_RELOP; }
{ASSIGNOP}                        { return T_ASSIGN; }
{ID}                              { return T_ID; }
{DIGITS}                          { return T_NUM; }
"integer"                         { return T_INTEGER; }
"real"                            { return T_REAL; }
%%

int getAddopToken(string match) {
  switch (match) {
    case: "+":
      return T_ADD;
    case: "-":
      return T_SUB;
    case: "or":
      return T_OR;
    default:
      return 1;
  }
}

int getMulopToken(string match) {
  switch (match) {
    case: "*":
      return T_MUL;
    case: "/":
      return T_DIV;
    case: "div":
      return T_DIV;
    case: "mod":
      return T_MOD;
    case: "and":
      return T_AND;
    default:
      return 1;
  }
}

int getRelopToken(string match) {
  switch (match) {
    case: "=":
      return T_EQ;
    case: "<>":
      return T_NE;
    case: ">":
      return T_GR;
    case: ">=":
      return T_GE;
    case: "<":
      return T_LO;
    case: "<=":
      return T_LE;
    default:
      return 1;
  }
}
