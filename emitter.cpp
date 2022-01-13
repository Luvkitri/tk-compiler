#include "global.hpp"
using namespace std;

stringstream output;

void writeToStream(string str) {
    output << str << "\n";
}

void writeToFile() {
    outputStream << output.str();
}

void emitLabel(Symbol label) {
    writeToStream(label.name + ":");
}

void emitJump(Symbol label) {
    writeToStream("\tjump.i\t#" + label.name);
}

void emitAssignment(Symbol &first, Symbol &second) {
    if (first.type == second.type) {
        writeToStream("\tmov." + getOperationTypeSuffix(first.type) + getVariableAddress(first) + "," + getVariableAddress(second));
    }
}

string getOperationTypeSuffix(Type type) {
    if (type == TYPE_INTEGER) {
        return "i\t";
    }

    if (type == TYPE_REAL) {
        return "r\t";
    }

    yyerror("Invalid type");
    return "";
}

string getVariableAddress(Symbol &symbol) {
    if (symbol.token == T_NUM) {
        return "#" + symbol.name;
    }

    return "";
}