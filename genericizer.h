#include "frontend/visitor.h"


#ifdef __cplusplus
extern "C" {

#endif


class GenericVisitor:public Visitor {

public:
    
    // Expressions 

    void visit(BooleanConstant * exp);
    void visit(NumericConstant * exp);
    void visit(StringLiteral * exp);
    void visit(Variable * exp);

    void visit(BinaryPlus * exp);
    void visit(BinaryMinus * exp);
    void visit(Mult * exp);
    void visit(Div * exp);

    void visit(UnaryPlus * exp);
    void visit(UnaryMinus * exp);

    void visit(RelationalExpression * exp);
    void visit(LogicalExpression * exp);
    void visit(LogicalNot * exp);

    void visit(CallExpression * exp);

    // Statements

    void visit(PrintStatement * exp);
    void visit(PrintLineStatement * exp);

    void visit(VariableDeclStatement * exp);
    void visit(AssignmentStatement * exp);

    void visit(IfStatement * exp);
    void visit(WhileStatement * exp);

    void visit(ReturnStatement * exp);
    void visit(CallStatement * exp);

    //Procedure , Module

    void visit(Procedure * exp);
    void visit(Tmodule * exp);

};


#ifdef __cplusplus
}
#endif

