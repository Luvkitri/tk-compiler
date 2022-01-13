#include "global.hpp"

Symbol::Symbol() {}
Symbol::~Symbol() {}

int SymbolTable::insert(string name, int token, Type type) {
  Symbol symbol;
  symbol.name = name;
  symbol.type = type;
  symbol.token = token;
  symbol.address = 0;
  symbol.isGlobal = isInGlobalScope;
  symbol.isReference = false;

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