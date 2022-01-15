#include "global.hpp"

stringstream output;

void writeToStream(string str) { output << str << "\n"; }

void writeToFile() {
  outputStream << output.str();
  output.str("");
}

void emitLabel(Symbol label) { writeToStream(label.name + ":"); }

void emitJump(Symbol label) { writeToStream("\tjump.i\t#" + label.name); }

void emitAssignment(Symbol &first, Symbol &second) {
  // TODO check for types
  writeToStream("\tmov." + getTypeSuffix(first.type) +
                getSymbolRepresentation(first) + "," +
                getSymbolRepresentation(second));
}

void emitExpression(Symbol &first, Symbol &second, Symbol &result, int op) {
  // TODO cast to correct type once implementing reals
  writeToStream("\t" + getInstructionByOperator(op) +
                getTypeSuffix(result.type) + getSymbolRepresentation(first) +
                getSymbolRepresentation(second) +
                getSymbolRepresentation(result));
}

string getTypeSuffix(Type type) {
  if (type == TYPE_INTEGER) {
    return "i\t";
  }

  // TODO add real type

  yyerror("Type does not exists");
  return "";
}

string getSymbolRepresentation(Symbol &symbol) {
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
      return "\tadd";
    case T_SUB:
      return "\tsub";
    case T_MUL:
      return "\tmul";
    case T_DIV:
      return "\tdiv";
    case T_MOD:
      return "\tmod";
    case T_OR:
      return "\tor";
    case T_AND:
      return "\tand";
    default:
      yyerror("Operator not allowed");
      return "";
  }
}
