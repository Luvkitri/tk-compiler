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
ID                                {LETTER}({LETTER}|{DIGIT})*
SIGN                              "+"|"-"
OR                                "or"
MULOP                             "*"|"/"|"div"|"mod"|"and"
RELOP                             "="|"<>"|">"|">="|"<"|"<="
ASSIGNOP                          ":="


%%
[ \t\n]+                          {/* Skip whitespaces, tabs and new lines (those are maintained by yylineno) */}
"program"                         { return T_PROGRAM; }
"var"                             { return T_VAR; }
"begin"                           { return T_BEGIN; }
"end"                             { return T_END; }
"integer"                         { yylval = T_INTEGER; return T_INTEGER; }
"real"                            { yylval = T_REAL; return T_REAL; }
{SIGN}                            { yylval = getSignToken(yytext); return T_SIGN;}
{OR}                              { yylval = T_OR; return T_OR; }
{MULOP}                           { yylval = getMulopToken(yytext); return T_MULOP; }
{RELOP}                           { yylval = getRelopToken(yytext); return T_RELOP; }
{ASSIGNOP}                        { return T_ASSIGN; }
{ID}                              { yylval = symbolTable.insertOrGet(yytext, T_ID, T_NONE); return T_ID; }
{DIGITS}                          { yylval = symbolTable.insertOrGet(yytext, T_NUM, T_INTEGER); return T_NUM; }
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

int getRelopToken(string match) {
  if (match.compare("=") == 0) {
    return T_EQ;
  }

  if (match.compare("<>") == 0) {
    return T_NE;
  }

  if (match.compare(">") == 0) {
    return T_GR;
  }

  if (match.compare(">=") == 0) {
    return T_GE;
  }

  if (match.compare("<") == 0) {
    return T_LO;
  }

  if (match.compare("<=") == 0) {
    return T_LE;
  }

  return 1;
}
