void writeToStream(string, bool);
void writeToFile();

void emitLabel(int);
void emitJump(int);
void emitAssignment(int, int);
void emitExpression(int, int, int, int);
void emitRelopExpression(int, int, int, int);
void emitCastTo(int &, int &);
void emitProcedure(int);
void emitRead(int);
void emitWrite(int);
void emitPush(int);
void emitCall(int);
void emitIncsp(int);
void emitLeave();
void emitReturn();
void emitEnter();
void emitExit();
void updateEnter(int);

string getSuffixByType(int);
string getSymbolRepresentation(int, bool = false);
string getInstructionByOperator(int);
string getTokenAsString(int);
