#include "global.hpp"

Symbol::Symbol() {}
Symbol::~Symbol() {}

Symbol SymbolTable::get(int index) {
  return symTable.at(index);
}

int SymbolTable::insert(string name, int token, Type type) {
  Symbol symbol;
  symbol.name = name;
  symbol.type = type;
  symbol.token = token;
  symbol.address = 0;
  symbol.isGlobal = isInGlobalScope;

  symTable.push_back(symbol);

  return (int)(symTable.size() - 1);
}

int SymbolTable::insertOrGet(string name, int token, Type type) {
  int existingSymbolIndex = lookup(name);

  if (existingSymbolIndex != -1) {
    return existingSymbolIndex;
  }

  return insert(name, token, type);
}

int SymbolTable::insertTemp(Type type) {
  string name = "$t" + to_string(numberOfTemps++);
  return insert(name, T_VAR, type);
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

  if (symbol.type == TYPE_INTEGER) {
    return 4;
  } else if (symbol.type == TYPE_REAL) {
    return 8;
  }

  return 0;
}