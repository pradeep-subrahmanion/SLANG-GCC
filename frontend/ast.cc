
#include "ast.h"
#include "common.h"
#include "context.h"
#include "compilation_unit.h"
#include <sstream>
#include "visitor.h"

extern "C" {

// Boolean Constant

BooleanConstant::BooleanConstant(bool val) {
    info = new SymbolInfo();
    info->symbol_name = "";
    info->type = TYPE_BOOL;
    info->bool_val = val;
}
SymbolInfo *BooleanConstant::evaluate(Runtime_Context *ctx) {
    return info;
}
TypeInfo BooleanConstant::typecheck(Compilation_Context *ctx) {
    return info->type;
}
TypeInfo BooleanConstant::get_type() {
    return info->type;
}

SymbolInfo * BooleanConstant::generate_js(Runtime_Context *ctx)
{
    string str = (info->bool_val == false) ? "false" : "true";
    ctx->update_stream(str);
}

BooleanConstant::~BooleanConstant() {
    delete info;
}

void BooleanConstant::accept(Visitor &visitor) {

    visitor.visit(this);
}

// Numeric Constant

NumericConstant::NumericConstant(double _value) {
    info = new SymbolInfo();
    info->symbol_name = "";
    info->type = TYPE_NUMERIC;
    info->double_val = _value;
}

SymbolInfo *NumericConstant::evaluate(Runtime_Context *ctx) {

    return info;
}

TypeInfo NumericConstant::typecheck(Compilation_Context *ctx) {
    return info->type;
}
TypeInfo NumericConstant::get_type() {
    return info->type;
}


NumericConstant::~NumericConstant()
{
  delete info;
}

SymbolInfo * NumericConstant::generate_js(Runtime_Context *ctx)
{
    // convert double to string

    std::ostringstream stream;
    stream << info->double_val;
    string str = stream.str();

    ctx->update_stream(str);
}

void NumericConstant::accept(Visitor &visitor) {

    visitor.visit(this);
}


// String Literal

StringLiteral::StringLiteral(std::string _value) {

    info = new SymbolInfo();
    info->symbol_name = "";
    info->type = TYPE_STRING;
    info->string_val = _value;
}

SymbolInfo *StringLiteral::evaluate(Runtime_Context *ctx) {

    return info;
}

TypeInfo StringLiteral::typecheck(Compilation_Context *ctx) {
    return info->type;
}
TypeInfo StringLiteral::get_type() {
    return info->type;
}

string StringLiteral::evaluate_string(Execution_Context *ctx) {
    return info->string_val;
}

StringLiteral::~StringLiteral() {
  delete info;
}

SymbolInfo * StringLiteral::generate_js(Runtime_Context *ctx)
{
    string str = "'" + info->string_val + "'";
    ctx->update_stream(str);  
}

void StringLiteral::accept(Visitor &visitor) {

    visitor.visit(this);
}


//Variable

Variable::Variable(SymbolInfo *_info) {
    info = _info;

    name = _info->symbol_name;
    type = _info->type;

}
Variable::Variable(Compilation_Context *ctx, std::string _name, double _value) {

    SymbolInfo *info = new SymbolInfo();
    info->symbol_name = _name;
    info->double_val = _value;
    info->type = TYPE_NUMERIC;
    ///symbol table add
    ctx->add_symbol(info);
    name = info->symbol_name;
}
Variable::Variable(Compilation_Context *ctx, std::string _name,
                   std::string _value) {

    SymbolInfo *info = new SymbolInfo();
    info->symbol_name = _name;
    info->string_val = _value;
    info->type = TYPE_STRING;
    ///symbol table add
    ctx->add_symbol(info);
    name = info->symbol_name;

}
Variable::Variable(Compilation_Context *ctx, std::string _name, bool _value) {

    SymbolInfo *info = new SymbolInfo();
    info->symbol_name = _name;
    info->bool_val = _value;
    info->type = TYPE_BOOL;
    ///symbol table add
    ctx->add_symbol(info);
    name = info->symbol_name;
}
std::string Variable::get_name() {
    return name;
}
SymbolInfo *Variable::evaluate(Runtime_Context *ctx) {
    SymbolInfoTable *st = ctx->get_symboltable();

    if (st == NULL) {
        return NULL;
    }

    SymbolInfo *inf = st->get(name);
    return inf;
}
TypeInfo Variable::typecheck(Compilation_Context *ctx) {

    SymbolInfoTable *st = ctx->get_symboltable();
    if (st == NULL) {
        return TYPE_ILLEGAL;
    } else {

        SymbolInfo *inf = st->get(name);
        if (inf != NULL) {
            type = inf->type;
            return type;
        }

    }

    return TYPE_ILLEGAL;
}
TypeInfo Variable::get_type() {
    return type;
}

string Variable::evaluate_string(Execution_Context *ctx) {
    return info->string_val;
}



Variable::~Variable() {
  delete info;
}

SymbolInfo * Variable::generate_js(Runtime_Context *ctx)
{
    string str = info->symbol_name;
    ctx->update_stream(str);  
}

void Variable::accept(Visitor &visitor) {

    visitor.visit(this);
}


//Binary Plus

BinaryPlus::BinaryPlus(Expression *e1, Expression *e2) {

    exp1 = e1;
    exp2 = e2;
}
SymbolInfo *BinaryPlus::evaluate(Runtime_Context *ctx) {
    SymbolInfo *eval_left = exp1->evaluate(ctx);
    SymbolInfo *eval_right = exp2->evaluate(ctx);

    if (eval_left->type == TYPE_STRING && eval_right->type == TYPE_STRING) {
        SymbolInfo *info = new SymbolInfo();
        info->type = TYPE_STRING;
        info->string_val = eval_left->string_val + eval_right->string_val;
        return info;
    }

    else if (eval_left->type == TYPE_NUMERIC && eval_right->type
             == TYPE_NUMERIC) {
        SymbolInfo *info = new SymbolInfo();
        info->type = TYPE_NUMERIC;
        info->double_val = eval_left->double_val + eval_right->double_val;
        return info;
    }

    else {
        exit_with_message("Type Mismatch");
    }

    return NULL;
}
TypeInfo BinaryPlus::typecheck(Compilation_Context *ctx) {
    TypeInfo info1 = exp1->typecheck(ctx);
    TypeInfo info2 = exp2->typecheck(ctx);

    if (info1 == info2 && info1 != TYPE_BOOL) {
        type = info1;
        return type;
    }

    return TYPE_ILLEGAL;
}
TypeInfo BinaryPlus::get_type() {
    return type;
}

string BinaryPlus::evaluate_string(Execution_Context *ctx) {
    string str1 = exp1->evaluate_string(ctx);
    string str2 = exp2->evaluate_string(ctx);

    return str1 + str2;
}


BinaryPlus::~BinaryPlus() {
  delete exp1;
  delete exp2;
}

SymbolInfo * BinaryPlus::generate_js(Runtime_Context *ctx)
{
    exp1->generate_js(ctx);
    ctx->update_stream("+");
    exp2->generate_js(ctx);
}

void BinaryPlus::accept(Visitor &visitor) {

    visitor.visit(this);
}


//Binary Minus


BinaryMinus::BinaryMinus(Expression *e1, Expression *e2) {
    exp1 = e1;
    exp2 = e2;
}
SymbolInfo *BinaryMinus::evaluate(Runtime_Context *ctx) {
    SymbolInfo *eval_left = exp1->evaluate(ctx);
    SymbolInfo *eval_right = exp2->evaluate(ctx);

    if (eval_left->type == TYPE_NUMERIC && eval_right->type == TYPE_NUMERIC) {
        SymbolInfo *info = new SymbolInfo();
        info->type = TYPE_NUMERIC;
        info->double_val = eval_left->double_val - eval_right->double_val;
        return info;
    }

    else {
        exit_with_message("Type Mismatch");
    }

    return NULL;
}
TypeInfo BinaryMinus::typecheck(Compilation_Context *ctx) {
    TypeInfo info1 = exp1->typecheck(ctx);
    TypeInfo info2 = exp2->typecheck(ctx);

    if (info1 == info2 && info1 == TYPE_NUMERIC) {
        type = info1;
        return type;
    }
}
TypeInfo BinaryMinus::get_type() {
    return type;
}



BinaryMinus::~BinaryMinus() {
  delete exp1;
  delete exp2;
}

SymbolInfo * BinaryMinus::generate_js(Runtime_Context *ctx)
{
    exp1->generate_js(ctx);
    ctx->update_stream("-");
    exp2->generate_js(ctx);
}

void BinaryMinus::accept(Visitor &visitor) {

    visitor.visit(this);
}


//Multiplication


Mult::Mult(Expression *e1, Expression *e2) {
    exp1 = e1;
    exp2 = e2;
}
SymbolInfo *Mult::evaluate(Runtime_Context *ctx) {
    SymbolInfo *eval_left = exp1->evaluate(ctx);
    SymbolInfo *eval_right = exp2->evaluate(ctx);

    if (eval_left->type == TYPE_NUMERIC && eval_right->type == TYPE_NUMERIC) {
        SymbolInfo *info = new SymbolInfo();
        info->type = TYPE_NUMERIC;
        info->double_val = eval_left->double_val * eval_right->double_val;
        return info;
    }

    else {
        exit_with_message("Type Mismatch");
    }

    return NULL;
}
TypeInfo Mult::typecheck(Compilation_Context *ctx) {
    TypeInfo info1 = exp1->typecheck(ctx);
    TypeInfo info2 = exp2->typecheck(ctx);

    if (info1 == info2 && info1 == TYPE_NUMERIC) {
        type = info1;
        return type;
    }
}
TypeInfo Mult::get_type() {
    return type;
}



Mult::~Mult() {
  delete exp1;
  delete exp2;
}

SymbolInfo * Mult::generate_js(Runtime_Context *ctx)
{
    exp1->generate_js(ctx);
    ctx->update_stream("*");
    exp2->generate_js(ctx);
}

void Mult::accept(Visitor &visitor) {

    visitor.visit(this);
}


//Division


Div::Div(Expression *e1, Expression *e2) {
    exp1 = e1;
    exp2 = e2;
}
SymbolInfo *Div::evaluate(Runtime_Context *ctx) {
    SymbolInfo *eval_left = exp1->evaluate(ctx);
    SymbolInfo *eval_right = exp2->evaluate(ctx);

    if (eval_left->type == TYPE_NUMERIC && eval_right->type == TYPE_NUMERIC) {
        SymbolInfo *info = new SymbolInfo();
        info->type = TYPE_NUMERIC;
        info->double_val = eval_left->double_val / eval_right->double_val;
        return info;
    }

    else {
        exit_with_message("Type Mismatch");
    }

    return NULL;
}
TypeInfo Div::typecheck(Compilation_Context *ctx) {
    TypeInfo info1 = exp1->typecheck(ctx);
    TypeInfo info2 = exp2->typecheck(ctx);

    if (info1 == info2 && info1 == TYPE_NUMERIC) {
        type = info1;
        return type;
    }
}
TypeInfo Div::get_type() {
    return type;
}


Div::~Div() {
  delete exp1;
  delete exp2;
}

SymbolInfo * Div::generate_js(Runtime_Context *ctx)
{
    exp1->generate_js(ctx);
    ctx->update_stream("/");
    exp2->generate_js(ctx);
}

void Div::accept(Visitor &visitor) {

    visitor.visit(this);
}


//UnaryPlus

UnaryPlus::UnaryPlus(Expression *e1) {
    exp1 = e1;
}
SymbolInfo *UnaryPlus::evaluate(Runtime_Context *ctx) {
    SymbolInfo *eval_left = exp1->evaluate(ctx);

    if (eval_left->type == TYPE_NUMERIC) {
        SymbolInfo *info = new SymbolInfo();
        info->type = TYPE_NUMERIC;
        info->double_val = eval_left->double_val;
        return info;
    }

    else {
        exit_with_message("Type Mismatch");
    }

    return NULL;
}
TypeInfo UnaryPlus::typecheck(Compilation_Context *ctx) {

    TypeInfo info1 = exp1->typecheck(ctx);

    if (info1 == TYPE_NUMERIC) {
        type = info1;
        return type;
    }

    else {
        exit_with_message("Type Mismatch");
    }

    return TYPE_ILLEGAL;

}
TypeInfo UnaryPlus::get_type() {
    return type;
}



UnaryPlus::~UnaryPlus() {
  delete exp1;
}

SymbolInfo * UnaryPlus::generate_js(Runtime_Context *ctx)
{
    ctx->update_stream("+");
    exp1->generate_js(ctx);
}

void UnaryPlus::accept(Visitor &visitor) {

    visitor.visit(this);
}


//UnaryMinus

UnaryMinus::UnaryMinus(Expression *e1) {
    exp1 = e1;
}
SymbolInfo *UnaryMinus::evaluate(Runtime_Context *ctx) {
    SymbolInfo *eval_left = exp1->evaluate(ctx);

    if (eval_left->type == TYPE_NUMERIC) {
        SymbolInfo *info = new SymbolInfo();
        info->type = TYPE_NUMERIC;
        info->double_val = -eval_left->double_val;
        return info;
    }

    else {
        exit_with_message("Type Mismatch");
    }

    return NULL;
}
TypeInfo UnaryMinus::typecheck(Compilation_Context *ctx) {

    TypeInfo info1 = exp1->typecheck(ctx);

    if (info1 == TYPE_NUMERIC) {
        type = info1;
        return type;
    }

    else {
        exit_with_message("Type Mismatch");
    }

    return TYPE_ILLEGAL;

}
TypeInfo UnaryMinus::get_type() {
    return type;
}


UnaryMinus::~UnaryMinus() {
  delete exp1;
}


SymbolInfo * UnaryMinus::generate_js(Runtime_Context *ctx)
{
    ctx->update_stream("-");
    exp1->generate_js(ctx);
}

void UnaryMinus::accept(Visitor &visitor) {

    visitor.visit(this);
}

// Relational Operator

RelationalExpression::RelationalExpression(Expression *e1, Expression *e2,
                                           RelationalOperator _op) {
    exp1 = e1;
    exp2 = e2;
    optr = _op;
}

SymbolInfo * RelationalExpression::evaluate(Runtime_Context *ctx) {
    SymbolInfo *eval_left = exp1->evaluate(ctx);
    SymbolInfo *eval_right = exp2->evaluate(ctx);

    SymbolInfo *ret = new SymbolInfo();

    if (eval_left->type == TYPE_NUMERIC && eval_right->type == TYPE_NUMERIC) {
        ret->type = TYPE_BOOL;
        ret->symbol_name = "";

        if (optr == OPTR_EQUAL) {
            ret->bool_val = (eval_left->double_val == eval_right->double_val);
        } else if (optr == OPTR_NEQUAL) {
            ret->bool_val = (eval_left->double_val != eval_right->double_val);
        } else if (optr == OPTR_GREATER_THAN) {
            ret->bool_val = (eval_left->double_val > eval_right->double_val);
        } else if (optr == OPTR_GREATER_EQUAL) {
            ret->bool_val = (eval_left->double_val >= eval_right->double_val);
        } else if (optr == OPTR_LESS_THAN) {
            ret->bool_val = (eval_left->double_val < eval_right->double_val);
        } else if (optr == OPTR_LESS_EQUAL) {
            ret->bool_val = (eval_left->double_val <= eval_right->double_val);
        }

        return ret;
    }

    else if (eval_left->type == TYPE_STRING && eval_right->type == TYPE_STRING) {
        ret->type = TYPE_BOOL;
        ret->symbol_name = "";

        if (optr == OPTR_EQUAL) {
            ret->bool_val = (eval_left->string_val.compare(
                                 eval_right->string_val) == 0);
        } else if (optr == OPTR_NEQUAL) {
            ret->bool_val = (eval_left->string_val.compare(
                                 eval_right->string_val) != 0);
        } else {
            ret->bool_val = false;
        }
        return ret;
    } else if (eval_left->type == TYPE_BOOL && eval_right->type == TYPE_BOOL) {
        ret->type = TYPE_BOOL;
        ret->symbol_name = "";

        if (optr == OPTR_EQUAL) {
            ret->bool_val = (eval_left->bool_val == eval_right->bool_val);
        } else if (optr == OPTR_NEQUAL) {
            ret->bool_val = (eval_left->bool_val != eval_right->bool_val);
        } else {
            ret->bool_val = false;
        }
        return ret;
    }

}

TypeInfo RelationalExpression::typecheck(Compilation_Context *ctx) {
    TypeInfo left_type = exp1->typecheck(ctx);
    TypeInfo right_type = exp2->typecheck(ctx);

    if (left_type != right_type) {
        exit_with_message("Wrong type in expression");
    } else if ((left_type == TYPE_STRING) && (optr != OPTR_EQUAL && optr
                                              != OPTR_NEQUAL)) {
        exit_with_message("Only == and != supported for string type");
    } else if ((left_type == TYPE_BOOL) && (optr != OPTR_EQUAL && optr
                                            != OPTR_NEQUAL)) {
        exit_with_message("Only == and != supported for bool type");
    }

    operand_type = left_type;
    type = TYPE_BOOL;
    return type;
}

TypeInfo RelationalExpression::get_type() {
    return type;
}

RelationalExpression::~RelationalExpression() {
  delete exp1;
  delete exp2;
}

SymbolInfo * RelationalExpression::generate_js(Runtime_Context *ctx)
{

    exp1->generate_js(ctx);
    if (optr == OPTR_EQUAL) {
            ctx->update_stream("==");           
    } 
    else if (optr == OPTR_NEQUAL) {
            ctx->update_stream("!=");           
    }
    else if (optr == OPTR_GREATER_THAN) {
            ctx->update_stream(">");           
    } 
    else if (optr == OPTR_GREATER_EQUAL) {
            ctx->update_stream(">=");           
    } 
    else if (optr == OPTR_LESS_THAN) {
            ctx->update_stream("<");           
    } 
    else if (optr == OPTR_LESS_EQUAL) {
            ctx->update_stream("<=");           
    }

   exp2->generate_js(ctx);
}

void RelationalExpression::accept(Visitor &visitor) {

    visitor.visit(this);
}


// Logical Operator

LogicalExpression::LogicalExpression(Expression *e1, Expression *e2, Token _op) {
    exp1 = e1;
    exp2 = e2;
    optr = _op;
}

SymbolInfo * LogicalExpression::evaluate(Runtime_Context *ctx) {
    SymbolInfo *eval_left = exp1->evaluate(ctx);
    SymbolInfo *eval_right = exp2->evaluate(ctx);

    SymbolInfo *ret = new SymbolInfo();
    if (eval_left->type == TYPE_BOOL && eval_right->type == TYPE_BOOL) {
        ret->type = TYPE_BOOL;
        ret->symbol_name = "";

        if (optr == TOKEN_AND) {
            ret->bool_val = (eval_left->bool_val && eval_right->bool_val);
        } else if (optr == TOKEN_OR) {
            ret->bool_val = (eval_left->bool_val || eval_right->bool_val);
        }

        else {
            ret->bool_val == TYPE_BOOL;
        }
        return ret;
    }

}

TypeInfo LogicalExpression::typecheck(Compilation_Context *ctx) {
    TypeInfo left_type = exp1->typecheck(ctx);
    TypeInfo right_type = exp2->typecheck(ctx);

    if (left_type != TYPE_BOOL || right_type != TYPE_BOOL) {
        exit_with_message("Wrong Type in expression");
    }

    type = TYPE_BOOL;
    return type;
}

TypeInfo LogicalExpression::get_type() {
    return type;
}


LogicalExpression::~LogicalExpression() {
  delete exp1;
  delete exp2;
}

SymbolInfo * LogicalExpression::generate_js(Runtime_Context *ctx)
{

    exp1->generate_js(ctx);
    if (optr == TOKEN_AND) {
        ctx->update_stream("&&");   
    } 
    else if (optr == TOKEN_OR) {
        ctx->update_stream("||");   
    }

   exp2->generate_js(ctx);
}

void LogicalExpression::accept(Visitor &visitor) {

    visitor.visit(this);
}



// Logical NOT

LogicalNot::LogicalNot(Expression *e1) {
    exp = e1;
}

SymbolInfo *LogicalNot::evaluate(Runtime_Context *ctx) {
    SymbolInfo *val = exp->evaluate(ctx);
    if (val->type == TYPE_BOOL) {
        SymbolInfo *info = new SymbolInfo();
        info->type = TYPE_BOOL;
        info->symbol_name = "";
        info->bool_val = !val->bool_val;

        return info;
    }

    return NULL;
}

TypeInfo LogicalNot::typecheck(Compilation_Context *ctx) {
    TypeInfo info = exp->typecheck(ctx);
    if (info != TYPE_BOOL) {
        exit_with_message("Wrong type in expression");
    }
    type = TYPE_BOOL;
    return info;
}

TypeInfo LogicalNot::get_type() {
    return type;
}


LogicalNot::~LogicalNot() {
  delete exp;
}

void LogicalNot::accept(Visitor &visitor) {

    visitor.visit(this);
}


SymbolInfo * LogicalNot::generate_js(Runtime_Context *ctx)
{
    ctx->update_stream("!");   
    exp->generate_js(ctx);
}

CallExpression::CallExpression(Procedure *_proc, vector<Expression *> _actuals) {
    proc = _proc;
    actuals = _actuals;
    procname = proc->name;
    type = proc->type;
    isrecurse = false;
}
CallExpression::CallExpression(string _name, vector<Expression *> _actuals,
                               bool _isrecurse, TypeInfo _type) {
    procname = _name;
    proc = NULL;
    actuals = _actuals;
    isrecurse = _isrecurse;
    type = _type;
}
SymbolInfo * CallExpression::evaluate(Runtime_Context *ctx) {
    if (proc == NULL) {

        Tmodule *mod = ctx->get_program();
        if (mod == NULL) {
            exit_with_message("Error in function call");
        }

        proc = mod->find_procedure(procname);
        if (proc == NULL) {
            exit_with_message("Error in function call");
        }
    }

    Runtime_Context *ctx1 = new Runtime_Context(ctx->get_program());
    vector<SymbolInfo *> list;
    for (int i = 0; i < actuals.size(); ++i) {
        SymbolInfo *info = actuals.at(i)->evaluate(ctx);
        list.push_back(info);
    }

    return proc->execute(ctx1, list);
}

TypeInfo CallExpression::typecheck(Compilation_Context *ctx) {
    if (proc != NULL) {
        return proc->typecheck(ctx);
    }
    return type;
}
TypeInfo CallExpression::get_type() {
    return type;
}



CallExpression::~CallExpression() {
  
}

SymbolInfo * CallExpression::generate_js(Runtime_Context *ctx)
{
    ctx->update_stream(procname);
    ctx->update_stream("(");
    
    for (unsigned i = 0;i < actuals.size(); ++i) {
        Expression *exp = actuals.at(i);
        exp->generate_js(ctx);

        if(i!=actuals.size()-1) {
            ctx->update_stream(",");
        }
    }

    ctx->update_stream(")");
}

}

void CallExpression::accept(Visitor &visitor) {

    visitor.visit(this);
}


