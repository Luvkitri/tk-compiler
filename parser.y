%{
  #include "global.hpp"
  #define YYDEBUG 1

  // Global ids
  vector<int> ids;

  // Procedure and Function
  int stackReservedMemory;
  vector<int> parameters;
  vector<int> parametersTypes;
%}

%require "3.5.1"

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
  T_PROGRAM {
    emitJump($1);
  } T_ID '(' identifier_list ')' ';' declarations subprogram_declarations {
    cout << "helllo??" << endl;
    emitLabel($1);
  }
  compound_statement
  '.' {
    emitExit();
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
        symbol.token = T_VAR;
        symbol.type = T_INTEGER;
        symbolTable.allocate(id);
      } else if ($5 == T_REAL) {
        symbol.token = T_VAR;
        symbol.type = T_REAL;
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
  subprogram_head declarations compound_statement {
    updateEnter(symbolTable.localAddress * -1);

    emitLeave();
    emitReturn();
    
    symbolTable.eraseLocalSymbols();
    isInGlobalScope = true;
  }
  ;
subprogram_head:
  T_FUN T_ID {
    symbolTable.get($2).token = T_FUN;
    isInGlobalScope = false;

    // old BP | readdr | function return 
    stackReservedMemory = 12;

    // Emit function label
    emitLabel($2);
    emitEnter();

    // Dispaly and clear symbol table
    symbolTable.display();
    symbolTable.eraseLocalSymbols();
  } arguments {
    // Storing information about function parameters types for future reference
    symbolTable.get($2).parametersTypes = parametersTypes;
    parametersTypes.clear();
  } ':' standard_type ';' {
    // Set function return type
    Symbol &functionID = symbolTable.get($2);
    functionID.type = $7;

    // Add function return variable to symbol table
    symbolTable.insert(functionID.name, T_VAR, $7, 8, true);
  }
  | T_PROC T_ID {
    symbolTable.get($2).token = T_PROC;
    isInGlobalScope = false;

    // old BP | readdr
    stackReservedMemory = 8;

    // Emit procedure label
    emitLabel($2);
    emitEnter();
  } arguments ';' {
    // Storing information about procedure parameters types for future reference
    symbolTable.get($2).parametersTypes = parametersTypes;
    parametersTypes.clear();
  } 
  ;
arguments:
  '(' parameter_list ')' {
    for (auto &parameterID : parameters) {
      symbolTable.get(parameterID).address = stackReservedMemory;

      // Function arguments are passed by reference which is 4 bytes
      stackReservedMemory += 4;
    }

    parameters.clear();
  }
  | %empty
  ;
parameter_list:
  identifier_list ':' type {
    for (auto &id : ids) {
      Symbol &symbol = symbolTable.get(id);
      symbol.isReference = true;
      symbol.token = T_VAR;
      symbol.type = $3;

      // Add paramter to current function parameters vector
      parameters.push_back(id);

      // Add parameter type
      parametersTypes.push_back($3);
    }

    ids.clear();
    symbolTable.display();
  }
  | parameter_list ';' identifier_list ':' type {
    for (auto &id : ids) {
      Symbol &symbol = symbolTable.get(id);
      symbol.isReference = true;
      symbol.token = T_VAR;
      symbol.type = $5;

      // Add paramter index in symbol table
      parameters.push_back(id);

      // Add parameter type
      parametersTypes.push_back($5);
    }
    ids.clear();
  }
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
    emitAssignment($1, $3);
  }
  | procedure_statement
  | compound_statement
  | T_IF expression {
    int elseLabelIndex = symbolTable.insertLabel();
    int falseIndex = symbolTable.insertOrGet("0", T_NUM, T_INTEGER);
    emitRelopExpression($2, falseIndex, elseLabelIndex, T_EQ);
    $$ = elseLabelIndex; // $3
  } T_THEN statement {
    int endConditionalLabelIndex = symbolTable.insertLabel();
    emitJump(endConditionalLabelIndex);
    emitLabel($3);
    $$ = endConditionalLabelIndex; // $6
  } T_ELSE statement {
    emitLabel($6);
  }
  | T_WHILE {
    // Out of loop label 
    int loopFinishLabelIndex = symbolTable.insertLabel();
    $$ = loopFinishLabelIndex; // $2

    // Begin loop label
    int loopStartLabelIndex = symbolTable.insertLabel();
    $1 = loopStartLabelIndex;
    emitLabel(loopStartLabelIndex);
  } expression T_DO {
    int falseIndex = symbolTable.insertOrGet("0", T_NUM, T_INTEGER);
    emitRelopExpression($3, falseIndex, $2, T_EQ);
  } statement {
    emitJump($1);
    emitLabel($2);
  }
  ;
variable:
  T_ID
  | T_ID '[' expression ']'
  ;
procedure_statement:
  T_ID {
    Symbol &symbol = symbolTable.get($1);
    if (symbol.token == T_PROC) {
      emitCall($1);
    } else if (symbol.token == T_FUN) {
      int returnIndex = symbolTable.insertTemp(symbol.type);
      emitPush(returnIndex);
      emitCall($1);
      emitIncsp(4);
      $$ = returnIndex;
    }
  }
  | T_ID '(' expression_list ')' {
    if ($1 == symbolTable.lookup("read")) {
      for (auto &id : ids) {
        emitRead(id);
      }
    } else if ($1 == symbolTable.lookup("write")) {
      for (auto &id : ids) {
        emitWrite(id);
      }
    } else {
      Symbol &symbol = symbolTable.get($1);

      if (symbol.token == T_PROC) {
        passArguments($1);
      } else if (symbol.token == T_FUN) {
        $$ = passArguments($1);
      }
    }

    ids.clear();
  }
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
    // Label for true condition
    int trueLabelIndex = symbolTable.insertLabel();
    emitRelopExpression($1, $3, trueLabelIndex, $2);

    // Temp for storing condition result
    int conditionResultIndex = symbolTable.insertTemp(T_INTEGER);
    
    // Set if statement result with false
    int falseIndex = symbolTable.insertOrGet("0", T_NUM, T_INTEGER);
    emitAssignment(conditionResultIndex, falseIndex);

    // Jump to label that check for condition - 'THEN'
    int thenLabelIndex = symbolTable.insertLabel();
    emitJump(thenLabelIndex);

    // Add true label 
    emitLabel(trueLabelIndex);

    // Set condition statement result to true
    int trueIndex = symbolTable.insertOrGet("1", T_NUM, T_INTEGER);
    emitAssignment(conditionResultIndex, trueIndex);

    // Add 'THEN' label
    emitLabel(thenLabelIndex);

    // Return condition result
    $$ = conditionResultIndex;
  }
  ;
simple_expression:
  term
  | T_SIGN term {
    if ($1 == T_SUB) {
      $$ = symbolTable.insertTemp(symbolTable.get($2).type);
      int indexOfSymbolZero = symbolTable.insertOrGet("0", T_NUM, T_INTEGER);
      emitExpression(indexOfSymbolZero, $2, $$, $1);
    } else {
      $$ = $2;
    } 
  }
  | simple_expression T_SIGN term {
    emitCastTo($1, $3);
    $$ = symbolTable.insertTemp(symbolTable.selectType($1, $3));
    emitExpression($1, $3, $$, $2);
  }
  | simple_expression T_OR term {
    $$ = symbolTable.insertTemp(T_INTEGER);
    emitExpression($1, $3, $$, $2);
  }
  ;
term:
  factor
  | term T_MULOP factor {
    emitCastTo($1, $3);
    $$ = symbolTable.insertTemp(symbolTable.selectType($1, $3));
    emitExpression($1, $3, $$, $2);
  }
  ;
factor:
  variable {
    Symbol &symbol = symbolTable.get($1);
    cout << symbol.name << endl;
    if (symbol.token == T_FUN) {
      $$ = passArguments($1);
    } else {
      yyerror("Nothing to return");
    }
  }
  | T_ID '(' expression_list ')' {
    if (symbolTable.get($1).token == T_FUN) {
      $$ = passArguments($1);
    } else {
      yyerror("No such function");
    }
  }
  | T_NUM
  | '(' expression ')' {
    $$ = $2;
  }
  | T_NOT factor {
    // Check if relop expression is false if so than set 
    // the result of relop to true otherwise false
    int trueLabelIndex = symbolTable.insertLabel();
    int falseIndex = symbolTable.insertOrGet("0", T_NUM, T_INTEGER);
    emitRelopExpression($2, falseIndex, trueLabelIndex, T_EQ);

    // Temp for storing condition result
    int conditionResultIndex = symbolTable.insertTemp(T_INTEGER);
    
    // Set if statement result with false
    emitAssignment(conditionResultIndex, falseIndex);

    // Jump to false label
    int falseLabelIndex = symbolTable.insertLabel();
    emitJump(falseLabelIndex);

    // True Label section
    emitLabel(trueLabelIndex);
    int trueIndex = symbolTable.insertOrGet("1", T_NUM, T_INTEGER);
    emitAssignment(conditionResultIndex, trueIndex);
    
    // False Label section
    emitLabel(falseLabelIndex);

    // Return condition result
    $$ = conditionResultIndex;
  }
  ;

%%

int passArguments(int symbolIndex) {
    Symbol &symbol = symbolTable.get(symbolIndex);
    int incsp = 0;
    int index = 0;

    for (auto &id : ids) {
      int argumentIndex = id;
      int functionParameterType = symbol.parametersTypes[index];

      // Assign to temp if number is passed
      if (symbolTable.get(argumentIndex).token == T_NUM || (symbolTable.get(argumentIndex).token == T_VAR && symbolTable.get(argumentIndex).type != functionParameterType)) {
        int tempIndex = symbolTable.insertTemp(functionParameterType);
        emitAssignment(tempIndex, id);
        argumentIndex = tempIndex;
      }

      // Push arguments to function|procedure
      emitPush(argumentIndex);
      incsp += 4;
      index += 1;
    }

    int returnIndex = 0;

    // if arguments are passed to function create temp for storing the return value if also needs to be pushed
    if (symbol.token == T_FUN) {
      cout << symbol.type << endl;
      returnIndex = symbolTable.insertTemp(symbol.type);
      emitPush(returnIndex);
      incsp += 4;
    }

    emitCall(symbolIndex);
    emitIncsp(incsp);
    ids.clear();

    return returnIndex;
  }
