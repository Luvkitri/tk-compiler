#include "global.hpp"

extern ofstream outputStream;
stringstream output;

void writeToStream(string str) { output << str; }

void writeToFile()
{
    outputStream << output.str();
    output.str("");
}

void emitLabel(Symbol &label) { writeToStream(label.name + ":"); }

void emitJump(Symbol &label) { writeToStream("\tjump.i\t#" + label.name); }

void emitAssignment(Symbol &variable, Symbol &expression_result)
{
    // TODO check for types
    writeToStream("\tmov." + getTypeSuffix(expression_result.type) +
                  getSymbolRepresentation(expression_result) + "," +
                  getSymbolRepresentation(variable));
    writeToStream("\t\t;mov." + getTypeSuffix(expression_result.type) + expression_result.name + "," + variable.name)
}

void emitExpression(Symbol &first, Symbol &second, Symbol &output, int op)
{
    // TODO cast to correct type once implementing reals
    writeToStream(getInstructionByOperator(op) + "." +
                  getTypeSuffix(output.type) + getSymbolRepresentation(first) + "," +
                  getSymbolRepresentation(second) + "," +
                  getSymbolRepresentation(output));
}

string getTypeSuffix(int type)
{
    if (type == T_INTEGER)
    {
        return "i\t";
    }

    // TODO add real type

    yyerror("Type does not exists");
    return "";
}

string getSymbolRepresentation(Symbol &symbol)
{
    if (symbol.token == T_NUM)
    {
        return "#" + symbol.name;
    }
    else if (symbol.token == T_VAR)
    {
        // TODO more to implement when functions come to play
        return to_string(symbol.address);
    }

    return "";
}

string getInstructionByOperator(int op)
{
    // TODO Add remaining instructions
    switch (op)
    {
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

string getTokenAsString(int token)
{
    // TODO Add remaining tokens
    switch (token)
    {
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
    default:
        return "";
    }
}
