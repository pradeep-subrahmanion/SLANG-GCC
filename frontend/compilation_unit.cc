#include "compilation_unit.h"
#include "visitor.h"

Procedure::Procedure(string _name, vector<SymbolInfo *> _formals, vector<
                     Statement *> stats, SymbolInfoTable *_locals, TypeInfo _type) {

    name = _name;
    formals = _formals;
    statements = stats;
    locals = _locals;
    type = _type;

    /// initialize symbolinfo for return value .
    return_val  = new SymbolInfo();
    return_val->symbol_name = "";
    return_val->type = type;
}

SymbolInfo * Procedure::execute(Runtime_Context *ctx,
                                vector<SymbolInfo *> actuals) {

    // add all arguments to symbol table

    if (actuals.size() == formals.size()) {
        for (int j = 0; j < formals.size(); ++j) {
            SymbolInfo * info_actual = actuals.at(j);
            SymbolInfo * info_formal = formals.at(j);

            info_actual->symbol_name = info_formal->symbol_name;
            ctx->add_symbol(info_actual);
        }
    }

    // execute statements

    for (int j = 0; j < statements.size(); ++j) {
        Statement *st = statements.at(j);
        return_val = st->execute(ctx);

        if (return_val != NULL) {
            return return_val;
        }
    }

    return NULL;
}

TypeInfo Procedure::typecheck(Compilation_Context *ctx) {
    return TYPE_NUMERIC;
}


Procedure::~Procedure() {
  
}

SymbolInfo * Procedure::generate_js(Runtime_Context *ctx) {

    string str  = "function " + name + "(";
    ctx->update_stream(str);

    for (unsigned i = 0;i < formals.size(); ++i) {
        SymbolInfo *info = formals.at(i);
        ctx->update_stream(info->symbol_name);

        if(i!=formals.size()-1) {
            ctx->update_stream(",");
        }
    }

    ctx->update_stream(") { \n");

    for(int i=0;i<statements.size();++i) {
        Statement *st = statements.at(i);
        st->generate_js(ctx);
    }
    ctx->update_stream("} \n");

}

void Procedure::accept(Visitor &visitor) {
    visitor.visit(this);
}

Tmodule::Tmodule(vector<Procedure *> _procs) {
    procs = _procs;
}

Procedure * Tmodule::find_procedure(string name) {
    for (int i = 0; i < procs.size(); ++i) {
        Procedure *proc = procs.at(i);
        string name1 = proc->name;

        std::transform(name1.begin(), name1.end(), name1.begin(), ::tolower);
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);

        if (name.compare(name1) == 0) {
            return proc;
        }
    }

    return NULL;
}

SymbolInfo * Tmodule::execute(Runtime_Context *ctx,
                              vector<SymbolInfo *> _actuals) {
    Procedure *_main = find_procedure("main");
    if (_main != NULL) {
        return _main->execute(ctx, _actuals);
    }

    return NULL;
}


Tmodule::~Tmodule() {
  
}

SymbolInfo * Tmodule::generate_js(Runtime_Context *ctx) {

    for (int i = 0; i < procs.size(); ++i) {
        Procedure *proc = procs.at(i);
        proc->generate_js(ctx);
    }

    ctx->update_stream("MAIN()");
}

void Tmodule::accept(Visitor &visitor) {
    visitor.visit(this);
}

vector<Procedure *> Tmodule::get_procs() {
    return procs;
}

