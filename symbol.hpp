struct Symbol {
  string name;
  int type;
  int token;
  int address;

  bool isGlobal;

  Symbol();
  ~Symbol();
};

class SymbolTable {
  int numberOfTemps = 0;
  int globalAddress = 0;
  int labelCount = 0;

  vector<Symbol> symTable;

 public:
  SymbolTable();
  ~SymbolTable();
  Symbol& get(int index);
  int insert(string name, int token, int type);
  int insertOrGet(string name, int token, int type);
  int insertTemp(int type);
  int insertLabel();
  int lookup(string name);
  void allocate(int index);
  int getSizeOfSymbolAt(int index);
  int selectType(int, int);
  void display();
};
