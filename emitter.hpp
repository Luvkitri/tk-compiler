void writeToStream(string, bool);
void writeToFile();

void emitLabel(int);
void emitJump(int);
void emitAssignment(int, int);
void emitExpression(int, int, int, int);
void emitRead(int);
void emitWrite(int);

string getSuffixByType(int);
string getSymbolRepresentation(int);
string getInstructionByOperator(int);
string getTokenAsString(int);
