%{
  #include "global.hpp"

  vector<int> ids;
%}

%define api.value.type union

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
%token T_AND
%token T_MOD

%type <int> simple_expression
%type <int> term

%%
  program:
    T_PROGRAM T_ID '(' identifier_list ')' ';' declarations subprogram_declarations {
      writeToStream("lab0:");
    }
    compound_statement
    '.' {
      writeToStream("\texit");
      writeToFile();
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

        if ($5 == T_INTEGER) {
          symbol.token = T_VAR;
          symbol.type = TYPE_INTEGER;
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
    T_INTEGER
    | T_REAL
    ;
  subprogram_declarations:
    subprogram_declarations subprogram_declaration ';'
    | %empty
    ;
  subprogram_declaration:
    subprogram_head declarations compound_statement
    ;
  subprogram_head:
    %empty
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
    | T_ADDOP term {
      if ($1 == T_ADD) {
        $$ = $2;
      } else if ($1 == T_SUB) {
        // -1
      }
    }
    | simple_expression T_ADDOP term {
        $$ = symbolTable.insertTemp(T_INTEGER);
        generateExpression(symbolTable.get($1), symbolTable.get($3), symbolTable.get($$), $2);
    }
    ;
  term:
    factor
    | term T_MULOP factor {
        $$ = symbolTable.insertTemp(T_INTEGER);
        generateExpression(symbolTable.grt($1), symbolTable.get($3), symbolTable.get($$), $2);
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