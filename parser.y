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
%token T_IF
%token T_THEN
%token T_ELSE
%token T_WHILE
%token T_DO

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
    declarations T_VARIABLE identifier_list ':' type ';' {
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
  optional_statement:
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
  variable:
    

%%