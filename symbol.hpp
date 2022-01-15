enum Type : int { TYPE_INTEGER = T_INTEGER, TYPE_REAL = T_REAL };
struct Symbol {
  string name;
  Type type;
  int token;
  int address;

  bool isGlobal;

  Symbol();
  ~Symbol();
};

class SymbolTable {
  int numberOfTemps = 0;
  int globalAddress = 0;

  vector<Symbol> symTable;

 public:
  SymbolTable();
  ~SymbolTable();
  Symbol get(int index);
  int insert(string name, int token, Type type);
  int insertOrGet(string name, int token, Type type);
  int insertTemp(Type type);
  int lookup(string name);
  void allocate(int index);
  int getSizeOfSymbolAt(int index);
};
