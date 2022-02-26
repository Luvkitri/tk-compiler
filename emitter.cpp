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

  string instuction("\tjump.i #" + label.name);
  writeToStream(instuction, !commentsEnabled);

  if (commentsEnabled) {
    string spaces(30 - instuction.length(), ' ');
    writeToStream(spaces + ";jump.i " + label.name, commentsEnabled);
  }
}

void emitAssignment(int variableIndex, int assigneeIndex) {
  Symbol &variable = symbolTable.get(variableIndex);
  Symbol &assignee = symbolTable.get(assigneeIndex);

  string instruction;
  string assignment;

  if (variable.type == assignee.type) {
    instruction = "mov." + getSuffixByType(assignee.type);
    assignment = "\t" + instruction + getSymbolRepresentation(assigneeIndex) +
                 "," + getSymbolRepresentation(variableIndex);

    writeToStream(assignment, !commentsEnabled);
  } else if (variable.type == T_REAL && assignee.type == T_INTEGER) {
    instruction = "inttoreal." + getSuffixByType(assignee.type);
    assignment = "\t" + instruction + getSymbolRepresentation(assigneeIndex) +
                 "," + getSymbolRepresentation(variableIndex);

    writeToStream(assignment, !commentsEnabled);
  } else if (variable.type == T_INTEGER && assignee.type == T_REAL) {
    instruction = "realtoint." + getSuffixByType(assignee.type);
    assignment = "\t" + instruction + getSymbolRepresentation(assigneeIndex) +
                 "," + getSymbolRepresentation(variableIndex);

    writeToStream(assignment, !commentsEnabled);
  }

  if (commentsEnabled) {
    string spaces(30 - assignment.length(), ' ');
    writeToStream(
        spaces + ";" + instruction + assignee.name + "," + variable.name,
        commentsEnabled);
  }
}

void emitExpression(int firstIndex, int secondIndex, int outputIndex, int op) {
  Symbol &first = symbolTable.get(firstIndex);
  Symbol &second = symbolTable.get(secondIndex);
  Symbol &output = symbolTable.get(outputIndex);

  string instruction =
      getInstructionByOperator(op) + "." + getSuffixByType(output.type);

  string expression("\t" + instruction + getSymbolRepresentation(firstIndex) +
                    "," + getSymbolRepresentation(secondIndex) + "," +
                    getSymbolRepresentation(outputIndex));

  writeToStream(expression, !commentsEnabled);

  if (commentsEnabled) {
    string spaces(30 - expression.length(), ' ');
    writeToStream(spaces + ";" + instruction + first.name + "," + second.name +
                      "," + output.name,
                  commentsEnabled);
  }
}

void emitCastTo(int &firstIndex, int &secondIndex) {
  Symbol &first = symbolTable.get(firstIndex);
  Symbol &second = symbolTable.get(secondIndex);

  int tempIndex = 0;

  bool firstHasBeenCasted = false;
  bool secondHasBeenCasted = false;
  string instruction;
  string cast;

  if (first.type == T_INTEGER && second.type == T_REAL) {
    tempIndex = symbolTable.insertTemp(T_REAL);

    instruction = "inttoreal." + getSuffixByType(T_INTEGER);
    cast = "\t" + instruction + getSymbolRepresentation(firstIndex) + ',' +
           getSymbolRepresentation(tempIndex);

    writeToStream(cast, !commentsEnabled);

    firstIndex = tempIndex;
    firstHasBeenCasted = true;
  } else if (first.type == T_REAL && second.type == T_INTEGER) {
    tempIndex = symbolTable.insertTemp(T_REAL);

    instruction = "inttoreal." + getSuffixByType(T_INTEGER);
    cast = "\t" + instruction + getSymbolRepresentation(secondIndex) + ',' +
           getSymbolRepresentation(tempIndex);

    writeToStream(cast, !commentsEnabled);

    secondIndex = tempIndex;
    secondHasBeenCasted = true;
  }

  if ((firstHasBeenCasted || secondHasBeenCasted) && commentsEnabled) {
    Symbol &temp = symbolTable.get(tempIndex);
    string spaces(30 - cast.length(), ' ');
    string symbolName;

    if (firstHasBeenCasted) {
      symbolName = first.name;
    } else if (secondHasBeenCasted) {
      symbolName = second.name;
    }

    writeToStream(spaces + ";" + instruction + symbolName + "," + temp.name,
                  commentsEnabled);
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

  string instruction("\t" + getInstructionByOperator(op) + "." +
                     getSuffixByType(suffixType) +
                     getSymbolRepresentation(leftSymbolIndex) + "," +
                     getSymbolRepresentation(rightSymbolIndex) + "," +
                     getSymbolRepresentation(labelIndex));

  writeToStream(instruction, !commentsEnabled);

  if (commentsEnabled) {
    string spaces(30 - instruction.length(), ' ');
    writeToStream(spaces + ";" + getInstructionByOperator(op) + "." +
                      getSuffixByType(suffixType) + left.name + "," +
                      right.name + "," + jumpLabel.name,
                  commentsEnabled);
  }
}

void emitRead(int symbolIndex) {
  Symbol &symbol = symbolTable.get(symbolIndex);

  string instruction("\tread." + getSuffixByType(symbol.type) +
                     getSymbolRepresentation(symbolIndex));

  writeToStream(instruction, !commentsEnabled);

  if (commentsEnabled) {
    string spaces(30 - instruction.length(), ' ');
    writeToStream(
        spaces + ";read." + getSuffixByType(symbol.type) + symbol.name,
        commentsEnabled);
  }
}

void emitWrite(int symbolIndex) {
  Symbol &symbol = symbolTable.get(symbolIndex);

  string instruction("\twrite." + getSuffixByType(symbol.type) +
                     getSymbolRepresentation(symbolIndex));

  writeToStream(instruction, !commentsEnabled);

  if (commentsEnabled) {
    string spaces(30 - instruction.length(), ' ');
    writeToStream(
        spaces + ";write." + getSuffixByType(symbol.type) + symbol.name,
        commentsEnabled);
  }
}

void emitPush(int symbolIndex) {
  string instruction("\tpush.i " + getSymbolRepresentation(symbolIndex, true));

  writeToStream(instruction, !commentsEnabled);

  if (commentsEnabled) {
    string spaces(30 - instruction.length(), ' ');
    writeToStream(spaces + ";push.i " + "&" + symbolTable.get(symbolIndex).name,
                  commentsEnabled);
  }
}

void emitCall(int symbolIndex) {
  string instruction("\tcall.i " + getSymbolRepresentation(symbolIndex));

  writeToStream(instruction, !commentsEnabled);

  if (commentsEnabled) {
    string spaces(30 - instruction.length(), ' ');
    writeToStream(spaces + ";call.i " + "&" + symbolTable.get(symbolIndex).name,
                  commentsEnabled);
  }
}

void emitIncsp(int incsp) {
  string instruction("\tincsp.i #" + to_string(incsp));

  writeToStream(instruction, !commentsEnabled);

  if (commentsEnabled) {
    string spaces(30 - instruction.length(), ' ');
    writeToStream(spaces + ";incsp.i " + to_string(incsp), commentsEnabled);
  }
}

void emitEnter() {
  string instruction = "\tenter.i #{_}";

  writeToStream(instruction, !commentsEnabled);

  if (commentsEnabled) {
    string spaces(30 - instruction.length(), ' ');
    writeToStream(spaces + ";enter.i {_}", commentsEnabled);
  }
}

void emitLeave() {
  string instruction = "\tleave";

  writeToStream(instruction, !commentsEnabled);

  if (commentsEnabled) {
    string spaces(30 - instruction.length(), ' ');
    writeToStream(spaces + ";leave", commentsEnabled);
  }
}

void emitReturn() {
  string instruction = "\treturn";

  writeToStream(instruction, !commentsEnabled);

  if (commentsEnabled) {
    string spaces(30 - instruction.length(), ' ');
    writeToStream(spaces + ";return", commentsEnabled);
  }
}

void emitExit() {
  string instruction = "\texit";
  writeToStream(instruction, !commentsEnabled);

  if (commentsEnabled) {
    string spaces(30 - instruction.length(), ' ');
    writeToStream(spaces + ";exit", commentsEnabled);
  }
}

void updateEnter(int reservedMemory) {
  // Copy existing output
  string stringOutput = output.str();

  // Clear the current stream
  output.str(string());

  // Match replace missing data and write to output stream
  const regex re("\\{_\\}");
  writeToStream(regex_replace(stringOutput, re, to_string(reservedMemory)),
                false);
}

string getSuffixByType(int type) {
  string suffix;

  if (type == T_INTEGER) {
    suffix += "i";
  } else if (type == T_REAL) {
    suffix += "r";
  } else {
    yyerror("Type does not exists");
    return "";
  }

  return suffix + " ";
}

string getSymbolRepresentation(int symbolIndex, bool asAddress) {
  Symbol &symbol = symbolTable.get(symbolIndex);

  if (symbol.token == T_NUM || symbol.token == T_LABEL ||
      symbol.token == T_FUN || symbol.token == T_PROC) {
    return "#" + symbol.name;
  } else if (symbol.token == T_VAR) {
    string representation = "";

    if (!symbol.isReference && asAddress) {
      representation += "#";
    }

    if (symbol.isReference && !asAddress) {
      representation += "*";
    }

    if (!symbol.isGlobal) {
      representation += "BP";
    }

    if (!symbol.isGlobal && symbol.address > 0) {
      representation += "+";
    }

    representation += to_string(symbol.address);

    return representation;
  }

  return "";
}

string getInstructionByOperator(int op) {
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
    case T_FUN:
      return "function";
    default:
      return "";
  }
}
