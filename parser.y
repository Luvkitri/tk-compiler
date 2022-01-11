%{
  #include "global.hpp"
%}

/* Set of essentials tokens */
%token T_PROGRAM
%token T_VAR
%token T_INTEGER
%token T_REAL
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
%token T_ADDOP
%token T_OR
%token T_MULOP // "*" "/" DIV MOD AND
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

%%
  program:
    T_PROGRAM T_ID '(' identifier_list ')' ';' declarations subprogram_declarations {
      // write to output lab0
    }
    compound_statement
    '.' {
      // write to output exit
    }
    ;
  identifier_list:
    T_ID {
      // check declared variable
      // push to some kind of id vector
    }
    | identifier_list ',' T_ID {
      // check declared variable
      // push to some kind of id vector
    }
    ;
  declarations:
    declarations T_VAR identifier_list ':' type ';' {
      // Handle variables declarations
    }
    |
    ;
  type:
    T_INTEGER
    | T_REAL
    ;
  subprogram_declarations:
    subprogram_declarations subprogram_declaration ';'
    |
    ;
  subprogram_declaration:
    subprogram_head declarations compound_statement {
      // do something
    }
    ;
  subprogram_head:
    %empty
    ;
  arguments:
    '(' parameter_list ')' {

    }
    |
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
    |
    ;
  statement_list:
    statement
    | statement_list ';' statement
    ;
  statement:
    variable T_ASSIGN expression {
      // generate assignment
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
      // push to identifier 
    }
    | expression_list ',' expression {
      // push to identifier
    }
    ;
  expression:
    simple_expression
    | simple_expression T_RELOP simple_expression {
      // generate functions
    }
    ;
  simple_expression:
    term
    | T_ADDOP term {

    }
    | simple_expression T_ADDOP term {

    }
    | simple_expression T_OR term {

    }
    ;
  term:
    factor
    | term T_MULOP factor  {

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