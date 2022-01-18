#include "global.hpp"

Symbol::Symbol() {}
Symbol::~Symbol() {}

SymbolTable::SymbolTable() {}
SymbolTable::~SymbolTable() {}

Symbol &SymbolTable::get(int index) { return symTable.at(index); }

int SymbolTable::insert(string name, int token, int type) {
  Symbol symbol;
  symbol.name = name;
  symbol.type = type;
  symbol.token = token;
  symbol.address = 0;
  symbol.isGlobal = isInGlobalScope;

  symTable.push_back(symbol);

  return (int)(symTable.size() - 1);
}

int SymbolTable::insertOrGet(string name, int token, int type) {
  int existingSymbolIndex = lookup(name);

  if (existingSymbolIndex != -1) {
    return existingSymbolIndex;
  }

  return insert(name, token, type);
}

int SymbolTable::insertTemp(int type) {
  string name = "$t" + to_string(numberOfTemps++);
  int index = insert(name, T_VAR, type);
  allocate(index);
  return index;
}

int SymbolTable::lookup(string name) {
  int index = (int)(symTable.size() - 1);
  if (isInGlobalScope) {
    for (; index >= 0; index--) {
      if (symTable.at(index).name == name) {
        return index;
      }
    }
  } else {
    for (; index >= 0; index--) {
      if (symTable.at(index).name == name && !symTable.at(index).isGlobal) {
        return index;
      }
    }
  }

  return -1;
}

void SymbolTable::allocate(int index) {
  Symbol &symbol = symTable.at(index);
  if (isInGlobalScope) {
    symbol.address = globalAddress;
    globalAddress += getSizeOfSymbolAt(index);
  }
}

int SymbolTable::getSizeOfSymbolAt(int index) {
  Symbol &symbol = symTable.at(index);

  if (symbol.type == T_INTEGER) {
    return 4;
  } else if (symbol.type == T_REAL) {
    return 8;
  }

  return 0;
}

void SymbolTable::display() {
  int i = 0;

  for (auto &symbol : symTable) {
    if (symbol.token != T_ID) {
      cout << ";" << i++ << "\t";

      if (symbol.isGlobal) {
        cout << "Global\t";
      } else {
        cout << "Local\t";
      }

      if (symbol.token == T_NUM) {
        cout << getTokenAsString(symbol.token) << "\t" << symbol.name << "\t"
             << getTokenAsString(symbol.type) << endl;
      } else if (symbol.token == T_VAR) {
        cout << getTokenAsString(symbol.token) << "\t" << symbol.name << "\t"
             << getTokenAsString(symbol.type) << "\toffset=" << symbol.address << endl;
      }
    }
  }
}