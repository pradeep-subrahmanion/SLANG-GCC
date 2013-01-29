#ifndef VISTOR_INCLUDED
#define VISITOR_INCLUDED

#include "ast.h"
#include "ast_statement.h"
#include "compilation_unit.h"

class Visitor {

public:
    
    // Expressions 

    virtual void visit(BooleanConstant * exp) = 0;
    virtual void visit(NumericConstant * exp) = 0;
    virtual void visit(StringLiteral * exp) = 0;
    virtual void visit(Variable * exp) = 0;

    virtual void visit(BinaryPlus * exp) = 0;
    virtual void visit(BinaryMinus * exp) = 0;
    virtual void visit(Mult * exp) = 0;
    virtual void visit(Div * exp) = 0;

    virtual void visit(UnaryPlus * exp) = 0;
    virtual void visit(UnaryMinus * exp) = 0;

    virtual void visit(RelationalExpression * exp) = 0;
    virtual void visit(LogicalExpression * exp) = 0;
    virtual void visit(LogicalNot * exp) = 0;

    virtual void visit(CallExpression * exp) = 0;

    // Statements

    virtual void visit(PrintStatement * exp) = 0;
    virtual void visit(PrintLineStatement * exp) = 0;

    virtual void visit(VariableDeclStatement * exp) = 0;
    virtual void visit(AssignmentStatement * exp) = 0;

    virtual void visit(IfStatement * exp) = 0;
    virtual void visit(WhileStatement * exp) = 0;

    virtual void visit(ReturnStatement * exp) = 0;
    virtual void visit(CallStatement * exp) = 0;

    //Procedure , Module

    virtual void visit(Procedure * exp) = 0;
    virtual void visit(Tmodule * exp) = 0;


};

#endif
