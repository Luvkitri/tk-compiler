#include "global.hpp"

extern ofstream outputStream;
stringstream output;

void writeToStream(string str, bool newLine) {
  if (newLine) {
    output << str << endl;
  } else {
    output << str;
  }
}

void writeToFile() {
  outputStream << output.str();
  output.str("");
}

void emitLabel(int labelIndex) {
  Symbol &label = symbolTable.get(labelIndex);
  writeToStream(label.name + ":", true);
  }

void emitJump(int labelIndex) {
  Symbol &label = symbolTable.get(labelIndex);

  writeToStream("\tjump.i\t#" + label.name, !commentsEnabled);

  if (commentsEnabled) {
    writeToStream("\t\t;jump.i\t" + label.name, commentsEnabled);
  }
}

void emitAssignment(int variableIndex, int assigneeIndex) {
  Symbol &variable = symbolTable.get(variableIndex);
  Symbol &assignee = symbolTable.get(assigneeIndex);

  string instruction;

  if (variable.type == assignee.type) {
    instruction = "mov." + getSuffixByType(assignee.type);
    writeToStream("\t" + instruction + getSymbolRepresentation(assigneeIndex) + "," +
                      getSymbolRepresentation(variableIndex),
                  !commentsEnabled);
  } else if (variable.type == T_REAL && assignee.type == T_INTEGER) {
    instruction = "inttoreal." + getSuffixByType(assignee.type);
    writeToStream("\t" + instruction + getSymbolRepresentation(assigneeIndex) + "," +
                      getSymbolRepresentation(variableIndex),
                  !commentsEnabled);
  } else if (variable.type == T_INTEGER && assignee.type == T_REAL) {
    instruction = "inttoreal." + getSuffixByType(assignee.type);
    writeToStream("\t" + instruction + getSymbolRepresentation(assigneeIndex) + "," +
                      getSymbolRepresentation(variableIndex),
                  !commentsEnabled);
  }

  if (commentsEnabled) {
    writeToStream("\t\t;" + instruction + assignee.name + "," + variable.name,
                  commentsEnabled);
  }
}

void emitExpression(int firstIndex, int secondIndex, int outputIndex, int op) {
  // TODO cast to correct type once implementing reals

  Symbol &first = symbolTable.get(firstIndex);
  Symbol &second = symbolTable.get(secondIndex);
  Symbol &output = symbolTable.get(outputIndex);

  string instruction =
      getInstructionByOperator(op) + "." + getSuffixByType(output.type);

  writeToStream("\t" + instruction + getSymbolRepresentation(firstIndex) + "," +
                    getSymbolRepresentation(secondIndex) + "," +
                    getSymbolRepresentation(outputIndex),
                !commentsEnabled);

  if (commentsEnabled) {
    writeToStream("\t;" + instruction + first.name + "," + second.name + "," +
                      output.name,
                  commentsEnabled);
  }
}

void emitRead(int symbolIndex) {
  Symbol &symbol = symbolTable.get(symbolIndex);

  writeToStream("\tread." + getSuffixByType(symbol.type) +
                    getSymbolRepresentation(symbolIndex),
                !commentsEnabled);

  if (commentsEnabled) {
    writeToStream("\t\t;read." + getSuffixByType(symbol.type) + symbol.name,
                  commentsEnabled);
  }
}

void emitWrite(int symbolIndex) {
  Symbol &symbol = symbolTable.get(symbolIndex);

  writeToStream("\twrite." + getSuffixByType(symbol.type) +
                    getSymbolRepresentation(symbolIndex),
                !commentsEnabled);

  if (commentsEnabled) {
    writeToStream("\t\t;write." + getSuffixByType(symbol.type) + symbol.name,
                  commentsEnabled);
  }
}

void matchTypes(int firstIndex, int secondIndex) {
  // if (first.type == INTEGER && second.type == REAL) {
  //   int newVar = symbolTable.insertTempSymbol(REAL);
  //   writeToOutput("\tinttoreal.i\t" + getVariableAddress(symbol1) + ',' +
  //                 getVariableAddress(symbolTable[newVar]));
  //   symbol1 = symbolTable[newVar];
  // } else if (symbol1.type == REAL && symbol2.type == INTEGER) {
  //   int newVar = symbolTable.insertTempSymbol(REAL);
  //   writeToOutput("\tinttoreal.i\t" + getVariableAddress(symbol2) + ',' +
  //                 getVariableAddress(symbolTable[newVar]));
  //   symbol2 = symbolTable[newVar];
  // } else if (symbol1.type != symbol2.type) {
  //   yyerror("incompatible types in casting up");
  // }
}

string getSuffixByType(int type) {
  if (type == T_INTEGER) {
    return "i\t";
  } else if (type == T_REAL) {
    return "r\t";
  }

  yyerror("Type does not exists");
  return "";
}

string getSymbolRepresentation(int symbolIndex) {
  Symbol &symbol = symbolTable.get(symbolIndex);

  if (symbol.token == T_NUM) {
    return "#" + symbol.name;
  } else if (symbol.token == T_VAR) {
    // TODO more to implement when functions come to play
    return to_string(symbol.address);
  }

  return "";
}

string getInstructionByOperator(int op) {
  // TODO Add remaining instructions
  switch (op) {
    case T_ADD:
      return "add";
    case T_SUB:
      return "sub";
    case T_MUL:
      return "mul";
    case T_DIV:
      return "div";
    case T_MOD:
      return "mod";
    case T_OR:
      return "or";
    case T_AND:
      return "and";
    default:
      yyerror("Operator not allowed");
      return "";
  }
}

string getTokenAsString(int token) {
  // TODO Add remaining tokens
  switch (token) {
    case T_ID:
      return "id";
    case T_VAR:
      return "variable";
    case T_NUM:
      return "number";
    case T_LABEL:
      return "label";
    case T_INTEGER:
      return "integer";
    case T_PROC:
      return "procedure";
    default:
      return "";
  }
}
