void writeToStream(string str, bool newLine);
void writeToFile();

void emitLabel(Symbol& label);
void emitJump(Symbol& label);
void emitAssignment(Symbol& first, Symbol& second);
void emitExpression(Symbol& first, Symbol& second, Symbol& output, int op);
void emitRead(Symbol& symbol);
void emitWrite(Symbol& symbol);

string getSuffixByType(int type);
string getSymbolRepresentation(Symbol& symbol);
string getInstructionByOperator(int op);
string getTokenAsString(int token);
