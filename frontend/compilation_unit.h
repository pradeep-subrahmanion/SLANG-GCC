

#ifndef CUNIT_INCLUDED
#define CUNIT_INCLUDED

class Statement;
class Visitor;

#include "common.h"
#include "context.h"
#include "symbol_table.h"


extern "C" {

class CompilationUnit {
public:
    virtual SymbolInfo * execute(Runtime_Context *ctx,
                                 vector<SymbolInfo *> _actuals) = 0;
    virtual SymbolInfo *generate_js(Runtime_Context *ctx) = 0;
    virtual void accept(Visitor &visitor) = 0;
};

class Proc {
public:
    virtual SymbolInfo * execute(Runtime_Context *ctx,
                                 vector<SymbolInfo *> _actuals) = 0;
    virtual SymbolInfo *generate_js(Runtime_Context *ctx) = 0;
    virtual void accept(Visitor &visitor) = 0;
};

class Procedure: Proc {

  
public:

    string name;
    vector<Statement *> statements;
    vector<SymbolInfo *> formals;
    SymbolInfoTable * locals;
    SymbolInfo *return_val;
    TypeInfo type;

    Procedure(string _name, vector<SymbolInfo *> _formals, vector<
              Statement *> stats, SymbolInfoTable *_locals,
              TypeInfo _type);
    SymbolInfo * execute(Runtime_Context *ctx, vector<SymbolInfo *> formals);
    TypeInfo typecheck(Compilation_Context *ctx);
    ~Procedure();
    SymbolInfo *generate_js(Runtime_Context *ctx);
    void accept(Visitor &visitor);
    vector<Statement *> get_statements();
};

class Tmodule: CompilationUnit {
    vector<Procedure *> procs;

public:
    Tmodule(vector<Procedure *> _procs);
    Procedure * find_procedure(string name);
    SymbolInfo * execute(Runtime_Context *ctx, vector<SymbolInfo *> _actuals);
    TypeInfo typecheck(Compilation_Context *ctx);
    ~Tmodule();
    SymbolInfo *generate_js(Runtime_Context *ctx);
    void accept(Visitor &visitor);
    vector<Procedure *> get_procs();
};

}

#endif

