#include "global.hpp"

Symbol::Symbol() {}

Symbol::~Symbol() {}

SymbolTable::SymbolTable() {
  insert("read", T_PROC, T_NONE);
  insert("write", T_PROC, T_NONE);
}
SymbolTable::~SymbolTable() {}

Symbol &SymbolTable::get(int index) { return symTable.at(index); }

int SymbolTable::insert(string name, int token, int type, int address,
                        bool isReference) {
  Symbol symbol;
  symbol.name = name;
  symbol.type = type;
  symbol.token = token;
  symbol.address = address;
  symbol.isReference = isReference;
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

int SymbolTable::insertLabel() {
  string labelName = "lab" + to_string(labelCount);
  labelCount++;

  int index = insert(labelName, T_LABEL, T_NONE);
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
      if (symTable.at(index).name == name && !symbolTable.get(index).isGlobal) {
        return index;
      }
    }
  }

  return -1;
}

int SymbolTable::lookupFunction(string name) {
  int index = (int)(symTable.size() - 1);

  for (; index >= 0; index--) {
    if (symTable.at(index).name == name && symbolTable.get(index).isGlobal) {
      return index;
    }
  }

  return -1;
}

void SymbolTable::allocate(int index) {
  Symbol &symbol = symTable.at(index);
  if (isInGlobalScope) {
    symbol.address = globalAddress;
    globalAddress += getSizeOfSymbolAt(index);
  } else {
    localAddress -= getSizeOfSymbolAt(index);
    symbol.address = localAddress;
  }
}

int SymbolTable::getSizeOfSymbolAt(int index) {
  Symbol &symbol = symTable.at(index);

  if (symbol.type == T_INTEGER || symbol.isReference) {
    return 4;
  } else if (symbol.type == T_REAL) {
    return 8;
  }

  return 0;
}

int SymbolTable::selectType(int firstIndex, int secondIndex) {
  Symbol &firstSymbol = symTable.at(firstIndex);
  Symbol &secondSymbol = symTable.at(secondIndex);

  int type = T_INTEGER;

  if (firstSymbol.type == T_REAL || secondSymbol.type == T_REAL) {
    type = T_REAL;
  }

  return type;
}

void SymbolTable::eraseLocalSymbols() {
  // Find start index for local symbols
  int startIndex = 0;

  for (auto &symbol : symTable) {
    if (symbol.isGlobal == true) {
      startIndex++;
    }
  }

  // remove local symbols and set address to 0
  symTable.erase(symTable.begin() + startIndex, symTable.end());
  localAddress = 0;
}

void SymbolTable::display() {
  int i = 0;

  cout << ";Symbol table dumb" << endl;

  for (auto &symbol : symTable) {
    if (symbol.token == T_ID) continue;

    cout << ";" << i++ << "\t";

    if (symbol.isGlobal) {
      cout << "Global\t";
    } else {
      cout << "Local\t";
    }

    if (symbol.isReference) {
      cout << "reference " << getTokenAsString(symbol.token) << "\t"
           << symbol.name << "\t" << getTokenAsString(symbol.type)
           << "\toffset=" << symbol.address << endl;
    } else if (symbol.token == T_NUM) {
      cout << getTokenAsString(symbol.token) << "\t\t" << symbol.name << "\t"
           << getTokenAsString(symbol.type) << endl;
    } else if (symbol.token == T_VAR) {
      cout << getTokenAsString(symbol.token) << "\t" << symbol.name << "\t"
           << getTokenAsString(symbol.type) << "\toffset=" << symbol.address
           << endl;
    } else if (symbol.token == T_LABEL) {
      cout << getTokenAsString(symbol.token) << "\t\t" << symbol.name << endl;
    } else if (symbol.token == T_PROC) {
      cout << getTokenAsString(symbol.token) << "\t\t" << symbol.name << endl;
    } else if (symbol.token == T_FUN) {
      cout << getTokenAsString(symbol.token) << "\t" << symbol.name << "\t"
           << getTokenAsString(symbol.type) << endl;
    }
  }
}