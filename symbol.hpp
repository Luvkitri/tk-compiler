struct Symbol {
  string name;
  int type;
  int token;
  int address;
  bool isReference;
  bool isGlobal;

  vector<int> parametersTypes;

  Symbol();
  ~Symbol();
};

class SymbolTable {
  int numberOfTemps = 0;
  int labelCount = 0;

  vector<Symbol> symTable;

 public:
  int globalAddress = 0;
  int localAddress = 0;
  SymbolTable();
  ~SymbolTable();
  Symbol& get(int index);
  int insert(string, int, int, int = 0, bool = false);
  int insertOrGet(string name, int token, int type);
  int insertTemp(int type);
  int insertLabel();
  int lookup(string name);
  void allocate(int index);
  int getSizeOfSymbolAt(int index);
  int selectType(int, int);
  void eraseLocalSymbols();
  void display();
};
