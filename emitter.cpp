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
    writeToStream("\t\t\t;jump.i\t" + label.name, commentsEnabled);
  }
}

void emitAssignment(int variableIndex, int assigneeIndex) {
  Symbol &variable = symbolTable.get(variableIndex);
  Symbol &assignee = symbolTable.get(assigneeIndex);

  string instruction;

  if (variable.type == assignee.type) {
    instruction = "mov." + getSuffixByType(assignee.type, true);
    writeToStream("\t" + instruction + getSymbolRepresentation(assigneeIndex) +
                      "," + getSymbolRepresentation(variableIndex),
                  !commentsEnabled);
  } else if (variable.type == T_REAL && assignee.type == T_INTEGER) {
    instruction = "inttoreal." + getSuffixByType(assignee.type, false);
    writeToStream("\t" + instruction + getSymbolRepresentation(assigneeIndex) +
                      "," + getSymbolRepresentation(variableIndex),
                  !commentsEnabled);

  } else if (variable.type == T_INTEGER && assignee.type == T_REAL) {
    instruction = "realtoint." + getSuffixByType(assignee.type, false);
    writeToStream("\t" + instruction + getSymbolRepresentation(assigneeIndex) +
                      "," + getSymbolRepresentation(variableIndex),
                  !commentsEnabled);
  }

  if (commentsEnabled) {
    writeToStream("\t\t;" + instruction + assignee.name + "," + variable.name,
                  commentsEnabled);
  }
}

void emitExpression(int firstIndex, int secondIndex, int outputIndex, int op) {
  Symbol &first = symbolTable.get(firstIndex);
  Symbol &second = symbolTable.get(secondIndex);
  Symbol &output = symbolTable.get(outputIndex);

  string instruction =
      getInstructionByOperator(op) + "." + getSuffixByType(output.type, true);

  writeToStream("\t" + instruction + getSymbolRepresentation(firstIndex) + "," +
                    getSymbolRepresentation(secondIndex) + "," +
                    getSymbolRepresentation(outputIndex),
                !commentsEnabled);

  if (commentsEnabled) {
    writeToStream("\t\t;" + instruction + first.name + "," + second.name + "," +
                      output.name,
                  commentsEnabled);
  }
}

void emitCastTo(int &firstIndex, int &secondIndex) {
  Symbol &first = symbolTable.get(firstIndex);
  Symbol &second = symbolTable.get(secondIndex);

  string instruction;

  if (first.type == T_INTEGER && second.type == T_REAL) {
    int tempIndex = symbolTable.insertTemp(T_REAL);
    Symbol &temp = symbolTable.get(tempIndex);

    instruction = "inttoreal." + getSuffixByType(T_INTEGER, false);
    writeToStream("\t" + instruction + getSymbolRepresentation(firstIndex) +
                      ',' + getSymbolRepresentation(tempIndex),
                  !commentsEnabled);

    if (commentsEnabled) {
      writeToStream("\t\t;" + instruction + first.name + "," + temp.name,
                    commentsEnabled);
    }

    firstIndex = tempIndex;
  } else if (first.type == T_REAL && second.type == T_INTEGER) {
    int tempIndex = symbolTable.insertTemp(T_REAL);
    Symbol &temp = symbolTable.get(tempIndex);

    instruction = "inttoreal." + getSuffixByType(T_INTEGER, false);
    writeToStream("\t" + instruction + getSymbolRepresentation(secondIndex) +
                      ',' + getSymbolRepresentation(tempIndex),
                  !commentsEnabled);

    if (commentsEnabled) {
      writeToStream("\t\t;" + instruction + first.name + "," + temp.name,
                    commentsEnabled);
    }

    secondIndex = tempIndex;
  }
}

void emitRelopExpression(int leftSymbolIndex, int rightSymbolIndex,
                         int labelIndex, int op) {
  Symbol &left = symbolTable.get(leftSymbolIndex);
  Symbol &right = symbolTable.get(rightSymbolIndex);
  Symbol &jumpLabel = symbolTable.get(labelIndex);

  int suffixType = T_INTEGER;

  if (left.type == T_REAL || right.type == T_REAL) {
    suffixType = T_REAL;
  }

  string instruction =
      getInstructionByOperator(op) + "." + getSuffixByType(suffixType, true);

  writeToStream("\t" + instruction + getSymbolRepresentation(leftSymbolIndex) +
                    "," + getSymbolRepresentation(rightSymbolIndex) + "," +
                    getSymbolRepresentation(labelIndex),
                !commentsEnabled);

  if (commentsEnabled) {
    writeToStream("\t\t;" + instruction + left.name + "," + right.name + "," +
                      jumpLabel.name,
                  commentsEnabled);
  }
}

void emitRead(int symbolIndex) {
  Symbol &symbol = symbolTable.get(symbolIndex);

  writeToStream("\tread." + getSuffixByType(symbol.type, false) +
                    getSymbolRepresentation(symbolIndex),
                !commentsEnabled);

  if (commentsEnabled) {
    writeToStream(
        "\t\t\t;read." + getSuffixByType(symbol.type, false) + symbol.name,
        commentsEnabled);
  }
}

void emitWrite(int symbolIndex) {
  Symbol &symbol = symbolTable.get(symbolIndex);

  writeToStream("\twrite." + getSuffixByType(symbol.type, false) +
                    getSymbolRepresentation(symbolIndex),
                !commentsEnabled);

  if (commentsEnabled) {
    writeToStream(
        "\t\t\t;write." + getSuffixByType(symbol.type, false) + symbol.name,
        commentsEnabled);
  }
}

string getSuffixByType(int type, bool tab) {
  string suffix;

  if (type == T_INTEGER) {
    suffix += "i";
  } else if (type == T_REAL) {
    suffix += "r";
  } else {
    yyerror("Type does not exists");
    return "";
  }

  if (tab) {
    return suffix + "\t";
  }

  return suffix + " ";
}

string getSymbolRepresentation(int symbolIndex) {
  Symbol &symbol = symbolTable.get(symbolIndex);

  if (symbol.token == T_NUM || symbol.token == T_LABEL) {
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
    case T_EQ:
      return "je";
    case T_GE:
      return "jge";
    case T_LE:
      return "jle";
    case T_GR:
      return "jg";
    case T_LO:
      return "jl";
    case T_NE:
      return "jne";
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
    case T_REAL:
      return "real";
    case T_PROC:
      return "procedure";
    default:
      return "";
  }
}
