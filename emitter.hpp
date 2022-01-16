void writeToStream(string str);
void writeToFile();

void emitLabel(Symbol& label);
void emitJump(Symbol& label);
void emitAssignment(Symbol& first, Symbol& second);
void emitExpression(Symbol& first, Symbol& second, Symbol& output, int op);

string getTypeSuffix(int type);
string getSymbolRepresentation(Symbol& symbol);
string getInstructionByOperator(int op);
string getTokenAsString(int token);
