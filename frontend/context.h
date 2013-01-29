
#ifndef CONTEXT_INCLUDED
#define CONTEXT_INCLUDED

#include "ast.h"
#include "common.h"
#include "symbol_table.h"

class Tmodule;

extern "C" {

class Runtime_Context {
    SymbolInfoTable *st;
    Tmodule *prog;
    FILE *js_fd;

public:
    Runtime_Context();
    Runtime_Context(Tmodule *mod);
    Tmodule *get_program();
    SymbolInfoTable *get_symboltable();
    void set_symboltable(SymbolInfoTable *_st);
    void add_symbol(SymbolInfo *info);
    void assign_symbol(Variable *var, SymbolInfo *value);
    SymbolInfo *get_symbol(string name);
    void update_stream(string str);
    void setup_js_codegen(FILE *fd);
    ~Runtime_Context();
};

class Compilation_Context {
    SymbolInfoTable *st;

public:
    Compilation_Context();
    SymbolInfoTable *get_symboltable();
    void set_symboltable(SymbolInfoTable *_st);
    void add_symbol(SymbolInfo *info);
    void assign_symbol(Variable *var, SymbolInfo *value);
    SymbolInfo *get_symbol(string name);
    ~Compilation_Context();
};


}
#endif
