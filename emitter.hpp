void writeToStream(string);
void writeToFile();

void emitLabel(string);
void emitJump(string);
void emitAssignment(Symbol, Symbol);
void emitExpression(Symbol, Symbol, Symbol, int);

string getTypeSuffix(Type);
string getSymbolRepresentation(Symbol&);
string getInstructionByOperator(int op);
