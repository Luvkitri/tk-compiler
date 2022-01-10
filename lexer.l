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
SIGN                              "+"|""
MULOP                             "*"|"/"|"div"|"mod"|"&"
RELOP                             "="|"<>"|">"|">="|"<"|"<="


%%
[ \t\n]+                          {/* Skip whitespaces, tabs and new lines (those are maintained by yylineno) */}
"program"                         { return T_PROGRAM; }
"var"                             { return T_VAR; }
"begin"                           { return T_BEGIN; }
"end"                             { return T_END; }
%%