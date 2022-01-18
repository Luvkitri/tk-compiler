%{
  #include "global.hpp"
  #define YYDEBUG 1

  vector<int> ids;
%}

/* Set of essentials tokens */
%token T_PROGRAM
%token T_LABEL
%token T_VAR
%token T_INTEGER
%token T_REAL
%token T_NONE
%token T_BEGIN
%token T_END
%token T_ASSIGN
%token T_ID
%token T_IF
%token T_THEN
%token T_ELSE
%token T_WHILE
%token T_DO
%token T_RELOP
%token T_OR
%token T_MULOP
%token T_NUM
%token T_NOT
%token T_EQ
%token T_GE
%token T_LE
%token T_GR
%token T_LO
%token T_NE
%token T_MUL
%token T_DIV
%token T_ADD
%token T_SUB
%token T_AND
%token T_MOD
%token T_SIGN
%token T_FUN
%token T_PROC


%%
program:
  T_PROGRAM T_ID '(' identifier_list ')' ';' declarations subprogram_declarations {
    writeToStream("lab0:");
  }
  compound_statement
  '.' {
    writeToStream("\texit");
  }
  ;
identifier_list:
  T_ID {
    ids.push_back($1);
  }
  | identifier_list ',' T_ID {
    ids.push_back($3);
  }
  ;
declarations:
  declarations T_VAR identifier_list ':' type ';' {
    for (auto &id : ids) {
      Symbol &symbol = symbolTable.get(id);
      
      if (symbol.name == "input" || symbol.name == "output") {
        continue;
      }

      if ($5 == T_INTEGER) {
        log(to_string(id));
        symbol.token = T_VAR;
        symbol.type = T_INTEGER;
        symbolTable.allocate(id);
      } else {
        yyerror("Invalid type decleration");
      }
    }

    ids.clear();
  }
  | %empty
  ;
type:
  standard_type
standard_type:
  T_INTEGER {
    $$ = $1;
  }
  | T_REAL {
    $$ = $1;
  }
  ;
subprogram_declarations:
  subprogram_declarations subprogram_declaration ';'
  | %empty
  ;
subprogram_declaration:
  subprogram_head declarations compound_statement
  ;
subprogram_head:
  T_FUN T_ID arguments ':' standard_type ';'
  | T_PROC T_ID arguments ';'
  ;
arguments:
  '(' parameter_list ')'
  | %empty
  ;
parameter_list:
  identifier_list ':' type
  | parameter_list ';' identifier_list ':' type
  ;
compound_statement:
  T_BEGIN optional_statements T_END
  ;
optional_statements:
  statement_list
  | %empty
  ;
statement_list:
  statement
  | statement_list ';' statement
  ;
statement:
  variable T_ASSIGN expression {
    emitAssignment(symbolTable.get($1), symbolTable.get($3));
  }
  | procedure_statement
  | compound_statement
  | T_IF expression {

  } T_THEN statement {

  } T_ELSE statement {

  }
  | T_WHILE {

  } expression T_DO {

  } statement {

  }
  ;
variable:
  T_ID
  | T_ID '[' expression ']' {

  }
  ;
procedure_statement:
  T_ID
  | T_ID '(' expression_list ')'
  ;
expression_list:
  expression {
    ids.push_back($1);
  }
  | expression_list ',' expression {
    ids.push_back($3);
  }
  ;
expression:
  simple_expression
  | simple_expression T_RELOP simple_expression {
    
  }
  ;
simple_expression:
  term
  | T_SIGN term {
    if ($1 == T_SUB) {
      $$ = symbolTable.insertTemp(T_INTEGER);
      int indexOfSymbolZero = symbolTable.insertOrGet("0", T_NUM, T_INTEGER);
      emitExpression(symbolTable.get(indexOfSymbolZero), symbolTable.get($2), symbolTable.get($$), $1);
    } else {
      $$ = $2;
    } 
  }
  | simple_expression T_SIGN term {
    $$ = symbolTable.insertTemp(T_INTEGER);
    emitExpression(symbolTable.get($1), symbolTable.get($3), symbolTable.get($$), $2);
  }
  | simple_expression T_OR term {
    $$ = symbolTable.insertTemp(T_INTEGER);
    emitExpression(symbolTable.get($1), symbolTable.get($3), symbolTable.get($$), $2);
  }
  ;
term:
  factor
  | term T_MULOP factor {
    $$ = symbolTable.insertTemp(T_INTEGER);
    emitExpression(symbolTable.get($1), symbolTable.get($3), symbolTable.get($$), $2);
  }
  ;
factor:
  variable
  | T_ID '(' expression_list ')'
  | T_NUM
  | '(' expression ')'
  | T_NOT factor
  ;

%%