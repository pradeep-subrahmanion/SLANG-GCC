#include "common.h"
#include "visitor.h"

using namespace std;

extern "C" {

// Print Statement

PrintStatement::PrintStatement(Expression *_exp) {
    exp = _exp;
    stmt_type = StatementTypePrint;
}

SymbolInfo *PrintStatement::execute(Runtime_Context *ctx) {
    SymbolInfo *info = exp->evaluate(ctx);

    if (info->type == TYPE_STRING) {
        std::cout << info->string_val;
    } else if (info->type == TYPE_NUMERIC) {
        std::cout << info->double_val;
    } else if (info->type == TYPE_BOOL) {
        std::cout << info->bool_val;
    }

    return NULL;
}

PrintStatement::~PrintStatement() {
  delete exp;
}

SymbolInfo * PrintStatement::generate_js(Runtime_Context *ctx) {
    ctx->update_stream("print(");
    exp->generate_js(ctx);
    ctx->update_stream(");\n");
}


void PrintStatement::accept(Visitor &visitor) {
    visitor.visit(this);
}

//PrintLine Statement

PrintLineStatement::PrintLineStatement(Expression *_exp) {
    exp = _exp;
    stmt_type = StatementTypePrintLine;
}

SymbolInfo * PrintLineStatement::execute(Runtime_Context *ctx) {
    SymbolInfo *info = exp->evaluate(ctx);
    if (info->type == TYPE_STRING) {
        std::cout << info->string_val << "\n";
    } else if (info->type == TYPE_NUMERIC) {
        std::cout << info->double_val << "\n";
    } else if (info->type == TYPE_BOOL) {
        std::cout << info->bool_val << "\n";
    }

    return NULL;
}

PrintLineStatement::~PrintLineStatement() {
  delete exp;
}


SymbolInfo * PrintLineStatement::generate_js(Runtime_Context *ctx) {
    ctx->update_stream("print(");
    exp->generate_js(ctx);
    ctx->update_stream(");\n");
}

void PrintLineStatement::accept(Visitor &visitor) {
    visitor.visit(this);
}

// Variable Declaration

VariableDeclStatement::VariableDeclStatement(SymbolInfo *_info) {
    info = _info;
    stmt_type = StatementTypeDeclaration;
}
SymbolInfo *VariableDeclStatement::execute(Runtime_Context *ctx) {
    ctx->add_symbol(info);
    var = new Variable(info);
    return NULL;
}


VariableDeclStatement::~VariableDeclStatement() {
  delete info;
  delete var;
}

SymbolInfo * VariableDeclStatement::generate_js(Runtime_Context *ctx) {
    ctx->update_stream("var ");
    ctx->update_stream(info->symbol_name);
    ctx->update_stream(";\n");
}

void VariableDeclStatement::accept(Visitor &visitor) {
    visitor.visit(this);
}


// Assignment Statement

AssignmentStatement::AssignmentStatement(Variable *_var, Expression *_exp) {
    var = _var;
    exp = _exp;
    stmt_type = StatementTypeAssignment;
}
AssignmentStatement::AssignmentStatement(SymbolInfo *info, Expression *_exp) {
    var = new Variable(info);
    exp = _exp;
    stmt_type = StatementTypeAssignment;
}

SymbolInfo *AssignmentStatement::execute(Runtime_Context *ctx) {
    SymbolInfo *info = exp->evaluate(ctx);
    ctx->assign_symbol(var, info);
    return NULL;
}


AssignmentStatement::~AssignmentStatement() {
  delete exp;
  delete var;
}

SymbolInfo * AssignmentStatement::generate_js(Runtime_Context *ctx) {

    var->generate_js(ctx);
    ctx->update_stream(" = ");
    exp->generate_js(ctx);
    ctx->update_stream(";\n");
}

void AssignmentStatement::accept(Visitor &visitor) {
    visitor.visit(this);
}

//If Statement

IfStatement::IfStatement(Expression *_exp, vector<Statement *> v1, vector<
                         Statement *> v2) {
    condition = _exp;
    if_statements = v1;
    else_statements = v2;
    stmt_type = StatementTypeIf;
}

SymbolInfo *IfStatement::execute(Runtime_Context *ctx) {
    SymbolInfo *info = condition->evaluate(ctx);

    SymbolInfo *ret = NULL;

    if (info->type == TYPE_BOOL) {

        vector<Statement *> *statements = &if_statements;
        if (info->bool_val == false) {
            statements = &else_statements;
        }

        for (int i = 0; i < (*statements).size(); ++i) {
            Statement *st = (*statements).at(i);
            ret = st->execute(ctx);
            if (ret != NULL) {
                return ret;
            }
        }

    }

    return NULL;
}


IfStatement::~IfStatement() {
  delete condition;
}

SymbolInfo * IfStatement::generate_js(Runtime_Context *ctx) {

    ctx->update_stream("if(");
    condition->generate_js(ctx);
    ctx->update_stream(")");
    ctx->update_stream(" {\n");

    for (int i = 0; i < if_statements.size(); ++i) {
        Statement *st = if_statements.at(i);
        st->generate_js(ctx);
    }

    ctx->update_stream(" }\n");

    if(else_statements.size() > 0) {
        ctx->update_stream("else {\n");
        for (int i = 0; i < else_statements.size(); ++i) {
            Statement *st = else_statements.at(i);
            st->generate_js(ctx);
        }
        ctx->update_stream(" }\n");
        
    }
    
}

void IfStatement::accept(Visitor &visitor) {
    visitor.visit(this);
}

// While Statement

WhileStatement::WhileStatement(Expression *_exp, vector<Statement *> v) {
    condition = _exp;
    statements = v;
    stmt_type = StatementTypeWhile;
}
SymbolInfo *WhileStatement::execute(Runtime_Context *ctx) {
    SymbolInfo *ret = NULL;
    SymbolInfo *info = condition->evaluate(ctx);

    while (info->type == TYPE_BOOL && info->bool_val == true) {

        for (int i = 0; i < statements.size(); ++i) {
            Statement *st = statements.at(i);
            st->execute(ctx);
            if (ret != NULL) {
                return ret;
            }
        }

        info = condition->evaluate(ctx);
    }

    return NULL;
}



WhileStatement::~WhileStatement() {
  
  delete condition;
}

SymbolInfo * WhileStatement::generate_js(Runtime_Context *ctx) {

    ctx->update_stream("while(");
    condition->generate_js(ctx);

    ctx->update_stream(")");
    ctx->update_stream(" {\n");

    for (int i = 0; i < statements.size(); ++i) {
        Statement *st = statements.at(i);
        st->generate_js(ctx);
    }

    ctx->update_stream(" }\n");

}

void WhileStatement::accept(Visitor &visitor) {
    visitor.visit(this);
}

ReturnStatement::ReturnStatement(Expression *_exp) {
    stmt_type = StatementTypeReturn;
    exp = _exp;
}

SymbolInfo *ReturnStatement::execute(Runtime_Context *ctx) {
    info = exp->evaluate(ctx);
    return info;
}


ReturnStatement::~ReturnStatement() {
  delete exp;
  delete info;
}

SymbolInfo * ReturnStatement::generate_js(Runtime_Context *ctx) {

    ctx->update_stream("return ");
    exp->generate_js(ctx);
    ctx->update_stream(";\n");
}

void ReturnStatement::accept(Visitor &visitor) {
    visitor.visit(this);
}


CallStatement::CallStatement(Expression *_exp) {
    exp = _exp;
    stmt_type = StatementTypeCall;
}

SymbolInfo *CallStatement::execute(Runtime_Context *ctx) {
    return exp->evaluate(ctx);
}


CallStatement::~CallStatement() {
  delete exp;
}

SymbolInfo * CallStatement::generate_js(Runtime_Context *ctx) {

    exp->generate_js(ctx);
    ctx->update_stream(";\n");
}

}

void CallStatement::accept(Visitor &visitor) {
    visitor.visit(this);
}


