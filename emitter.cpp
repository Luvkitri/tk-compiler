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

void emitLabel(Symbol& label) { writeToStream(label.name + ":", true); }

void emitJump(Symbol& label) {
  writeToStream("\tjump.i\t#" + label.name, !commentsEnabled);
  if (commentsEnabled) {
    writeToStream("\t\t;jump.i\t" + label.name, commentsEnabled);
  }
}

void emitAssignment(Symbol& variable, Symbol& expression_result) {
  // TODO check for types

  string instruction = "mov." + getSuffixByType(expression_result.type);

  writeToStream("\t" + instruction +
                    getSymbolRepresentation(expression_result) + "," +
                    getSymbolRepresentation(variable),
                !commentsEnabled);

  if (commentsEnabled) {
    writeToStream(
        "\t\t;" + instruction + expression_result.name + "," + variable.name,
        commentsEnabled);
  }
}

void emitExpression(Symbol& first, Symbol& second, Symbol& output, int op) {
  // TODO cast to correct type once implementing reals

  string instruction =
      getInstructionByOperator(op) + "." + getSuffixByType(output.type);

  writeToStream("\t" + instruction + getSymbolRepresentation(first) + "," +
                    getSymbolRepresentation(second) + "," +
                    getSymbolRepresentation(output),
                !commentsEnabled);

  if (commentsEnabled) {
    writeToStream("\t;" + instruction + first.name + "," + second.name + "," +
                      output.name,
                  commentsEnabled);
  }
}

void emitRead(Symbol& symbol) {
  writeToStream("\tread." + getSuffixByType(symbol.type) +
                    getSymbolRepresentation(symbol),
                !commentsEnabled);

  if (commentsEnabled) {
    writeToStream("\t\t;read." + getSuffixByType(symbol.type) + symbol.name,
                  commentsEnabled);
  }
}

void emitWrite(Symbol& symbol) {
  writeToStream("\twrite." + getSuffixByType(symbol.type) +
                    getSymbolRepresentation(symbol),
                !commentsEnabled);

  if (commentsEnabled) {
    writeToStream("\t\t;write." + getSuffixByType(symbol.type) + symbol.name,
                  commentsEnabled);
  }
}

string getSuffixByType(int type) {
  if (type == T_INTEGER) {
    return "i\t";
  }

  // TODO add real type

  yyerror("Type does not exists");
  return "";
}

string getSymbolRepresentation(Symbol& symbol) {
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
