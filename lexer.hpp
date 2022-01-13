int yylex();
int yylex_destroy();
extern int yylineno;
extern FILE *yyin;
extern FILE *yyout;

int getAddopToken(string);
int getMulopToken(string);
int getRelopToken(string);
