
#include "ast.h"
#include "context.h"

extern "C" {

class Statement {
public:
    StatementType stmt_type;
    virtual SymbolInfo *execute(Runtime_Context *ctx) =0;
    virtual SymbolInfo *generate_js(Runtime_Context *ctx) = 0;
    virtual void accept(Visitor &visitor) = 0;
};

class PrintStatement: public Statement {

    Expression *exp;

public:
    PrintStatement(Expression *_exp);
    ~PrintStatement();
    SymbolInfo *execute(Runtime_Context *ctx);
    SymbolInfo *generate_js(Runtime_Context *ctx);
    void accept(Visitor &visitor);

};

class PrintLineStatement: public Statement {

    Expression *exp;

public:
    PrintLineStatement(Expression *_exp);
    ~PrintLineStatement();
    SymbolInfo * execute(Runtime_Context *ctx);
    SymbolInfo *generate_js(Runtime_Context *ctx);
    void accept(Visitor &visitor);
};

class VariableDeclStatement: public Statement {
    SymbolInfo *info;
    Variable *var;

public:
    VariableDeclStatement(SymbolInfo *_info);
    SymbolInfo *execute(Runtime_Context *ctx);
    ~VariableDeclStatement();
    SymbolInfo *generate_js(Runtime_Context *ctx);
    void accept(Visitor &visitor);
};

class AssignmentStatement: public Statement {
    Variable *var;
    Expression *exp;

public:
    AssignmentStatement(SymbolInfo *info, Expression *_exp);
    AssignmentStatement(Variable *_var, Expression *_exp);
    SymbolInfo *execute(Runtime_Context *ctx);
    ~AssignmentStatement();
    SymbolInfo *generate_js(Runtime_Context *ctx);
    void accept(Visitor &visitor);
};

class IfStatement: public Statement {
    Expression *condition;
    vector<Statement *> if_statements;
    vector<Statement *> else_statements;

public:
    IfStatement(Expression *_exp, vector<Statement *> v1,
                vector<Statement *> v2);
    SymbolInfo *execute(Runtime_Context *ctx);
    ~IfStatement();
    SymbolInfo *generate_js(Runtime_Context *ctx);
    void accept(Visitor &visitor);
};

class WhileStatement: public Statement {

    Expression *condition;
    vector<Statement *> statements;

public:
    WhileStatement(Expression *_exp, vector<Statement *> v);
    SymbolInfo *execute(Runtime_Context *ctx);
    ~WhileStatement();
    SymbolInfo *generate_js(Runtime_Context *ctx);
    void accept(Visitor &visitor);

};

class ReturnStatement: public Statement {
    Expression *exp;
    SymbolInfo *info;

public:
    ReturnStatement(Expression *_exp);
    SymbolInfo *execute(Runtime_Context *ctx);
    ~ReturnStatement();
    SymbolInfo *generate_js(Runtime_Context *ctx);
    void accept(Visitor &visitor);
};

class CallStatement: public Statement {
    Expression *exp;

public:
    CallStatement(Expression *_exp);
    SymbolInfo *execute(Runtime_Context *ctx);
    ~CallStatement();
    SymbolInfo *generate_js(Runtime_Context *ctx);
    void accept(Visitor &visitor);
};

}
