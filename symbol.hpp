struct Symbol;
enum Type : int;

enum Type : int { TYPE_INTEGER, TYPE_REAL, TYPE_NONE };

struct Symbol {
  string name;
  Type type;
  int token;
  int address;

  bool isReference;
  bool isGlobal;

  Symbol();
  ~Symbol();
};

class SymbolTable {
  int numberOfTemps = 0;

  vector<Symbol> symTable;

 public:
  SymbolTable();
  ~SymbolTable();
  int insert(string name, int token, Type type);
  int insertOrGet(string name, int token, Type type);
  int insertTemp(Type type);
  int lookup(string name);
};
