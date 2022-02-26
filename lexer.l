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

%option nounput
%option noinput

DIGIT                             [0-9]
LETTER                            [a-zA-Z]
DIGITS                            {DIGIT}+
OPTIONAL_FRACTION                 "."{DIGITS}
OPTIONAL_EXPONENT                 [eE][+-]?{DIGITS}
NUM                               {DIGITS}{OPTIONAL_FRACTION}?{OPTIONAL_EXPONENT}?
ID                                {LETTER}({LETTER}|{DIGIT})*
SIGN                              "+"|"-"
OR                                "or"
MULOP                             "*"|"/"|"div"|"mod"|"and"
RELOP_EQ                          "="
RELOP_NE                          "<>"
RELOP_GR                          ">"
RELOP_GE                          ">="
RELOP_LO                          "<"
RELOP_LE                          "<="
ASSIGNOP                          ":="


%%
[ \t\n]+                          {/* Skip whitespaces, tabs and new lines (those are maintained by yylineno) */}
"program"                         { yylval = symbolTable.insertLabel(); return T_PROGRAM; }
"integer"                         { yylval = T_INTEGER; return T_INTEGER; }
"real"                            { yylval = T_REAL; return T_REAL; }
"var"                             { return T_VAR; }
"begin"                           { return T_BEGIN; }
"end"                             { return T_END; }
"if"                              { return T_IF; }
"then"                            { return T_THEN; }
"else"                            { return T_ELSE; }
"do"                              { return T_DO; }
"while"                           { return T_WHILE; }
"function"                        { return T_FUN; }
"procedure"                       { return T_PROC; }
"not"                             { yylval = T_NOT; return T_NOT; }
{OR}                              { yylval = T_OR; return T_OR; }
{RELOP_EQ}                        { yylval = T_EQ; return T_RELOP; }
{RELOP_NE}                        { yylval = T_NE; return T_RELOP; }
{RELOP_GR}                        { yylval = T_GR; return T_RELOP; }
{RELOP_GE}                        { yylval = T_GE; return T_RELOP; }
{RELOP_LO}                        { yylval = T_LO; return T_RELOP; }
{RELOP_LE}                        { yylval = T_LE; return T_RELOP; }
{MULOP}                           { yylval = getMulopToken(yytext); return T_MULOP; }
{SIGN}                            { yylval = getSignToken(yytext); return T_SIGN;}
{ASSIGNOP}                        { return T_ASSIGN; }
{ID}                              { yylval = symbolTable.insertOrGet(yytext, T_ID, T_NONE); return T_ID; }
{DIGITS}                          { yylval = symbolTable.insertOrGet(yytext, T_NUM, T_INTEGER); return T_NUM; }
{NUM}                             { yylval = symbolTable.insertOrGet(yytext, T_NUM, T_REAL); return T_NUM; }
.                                 { return *yytext; }
%%

int getSignToken(string match) {
  if (match.compare("+") == 0) {
    return T_ADD;
  }

  if (match.compare("-") == 0) {
    return T_SUB;
  }

  return 1;
}

int getMulopToken(string match) {
  if (match.compare("*") == 0) {
    return T_MUL;
  }

  if (match.compare("/") == 0) {
    return T_DIV;
  }

  if (match.compare("div") == 0) {
    return T_DIV;
  }

  if (match.compare("mod") == 0) {
    return T_MOD;
  }

  if (match.compare("and") == 0) {
    return T_AND;
  }

  return 1;
}
