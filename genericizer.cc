
#include "genericizer.h"
#include "config.h"
#include "system.h"
#include "ansidecl.h"
#include "coretypes.h"
#include "opts.h"
#include "tree.h"
#include "gimple.h"
#include "ggc.h"
#include "toplev.h"
#include "debug.h"
#include "options.h"
#include "flags.h"
#include "convert.h"
#include "diagnostic.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "except.h"
#include "target.h"
#include "common/common-target.h"
#include "tree-iterator.h"
#include "cgraph.h"
#include "tree-dump.h"
#include <mpfr.h>

using namespace std;


#ifdef __cplusplus
extern "C" {

#endif

static tree current_procedure;
static tree current_statements;
static tree current_exp;
static map<string,tree> symbol_table;

/// Helper functions for GENERIC lowering

static tree
build_function_decl (const char *name, bool external, tree function_type) {

    tree fndecl =  build_fn_decl (name, function_type);
    SET_DECL_ASSEMBLER_NAME(fndecl, get_identifier(name));

    DECL_EXTERNAL (fndecl) = external;
    DECL_ARTIFICIAL (fndecl) = false;
    TREE_STATIC (fndecl) = !external;
    DECL_CONTEXT (fndecl) = NULL_TREE;

    return fndecl;
}


/// build function body.

static void
build_function_body (tree fndecl, tree stmts, tree block, tree ret) {

    DECL_INITIAL(fndecl) = block;
    DECL_SAVED_TREE (fndecl) = stmts ;

    tree resdecl = build_decl(input_location,RESULT_DECL, NULL_TREE, ret);
    DECL_CONTEXT (resdecl) = fndecl;
    DECL_RESULT(fndecl) = resdecl;
    current_function_decl = fndecl;

    if (DECL_STRUCT_FUNCTION(fndecl) == NULL)
        push_struct_function(fndecl);
    else
		push_cfun(DECL_STRUCT_FUNCTION(fndecl));

    cfun->function_end_locus = BUILTINS_LOCATION;

}

/// Returns tree from SLANG built-in type.

tree tree_from_slang_type(TypeInfo info) {

    tree ret = NULL_TREE;

    switch(info) {
        case TYPE_NUMERIC:
            ret = make_unsigned_type (INT_TYPE_SIZE);
            break;   
        case TYPE_BOOL:
            ret = make_unsigned_type (BOOL_TYPE_SIZE);
            break;  
        case TYPE_STRING:
            ret = build_pointer_type (char_type_node);
            break;  
        default:
            break;

    }

    return ret;
}

/// Visitor Methods

void GenericVisitor::visit (BooleanConstant * exp) {
    cout << "BooleanConstant " << "\n";
    
    SymbolInfo *info = exp->get_symbol();
    current_exp = build_int_cst (boolean_type_node, info->bool_val);
}

void GenericVisitor::visit (NumericConstant * exp) {
    cout << "NumericConstant " << "\n";

    SymbolInfo *info = exp->get_symbol();
    current_exp = build_int_cst (integer_type_node, info->double_val);
}

void GenericVisitor::visit (StringLiteral * exp) {
    cout << "StringLiteral " << "\n";

    SymbolInfo *info = exp->get_symbol();
    current_exp = build_string_literal (info->string_val.length() + 1, info->string_val.c_str());
}

void GenericVisitor::visit (Variable * exp) {
    cout << "Variable " << "\n";
    current_exp = tree_from_slang_type(TYPE_STRING);
}

void GenericVisitor::visit (BinaryPlus * exp) {
    cout << "BinaryPlus " << "\n";
}

void GenericVisitor::visit (BinaryMinus * exp) {
    cout << "BinaryMinus " << "\n";
}
void GenericVisitor::visit (Mult * exp) {
    cout << "Mult " << "\n";
}
void GenericVisitor::visit (Div * exp) {
    cout << "Div " << "\n";
}

void GenericVisitor::visit (UnaryPlus * exp) {
    cout << "UnaryPlus " << "\n";
}
void GenericVisitor::visit (UnaryMinus * exp) {
    cout << "UnaryMinus " << "\n";
}
void GenericVisitor::visit (RelationalExpression * exp) {
    cout << "RelationalExpression " << "\n";
}
void GenericVisitor::visit (LogicalExpression * exp) {
    cout << "LogicalExpression " << "\n";
}
void GenericVisitor::visit (LogicalNot * exp) {
    cout << "LogicalNot " << "\n";
}

void GenericVisitor::visit (CallExpression * exp) {

    cout << "CallExpression " << "\n";
}

// Statements

void GenericVisitor::visit (PrintStatement * exp) {
    cout << "PrintStatement " << "\n";
}
void GenericVisitor::visit (PrintLineStatement * exp) {
    cout << "PrintLineStatement " << "\n";
}

void GenericVisitor::visit (VariableDeclStatement * exp) {
    cout << "VariableDeclStatement " << "\n";
}
void GenericVisitor::visit (AssignmentStatement * exp) {
    cout << "AssignmentStatement " << "\n";
}

void GenericVisitor::visit (IfStatement * exp) {
    cout << "IfStatement " << "\n";
}
void GenericVisitor::visit (WhileStatement * exp) {
    cout << "WhileStatement " << "\n";
}

void GenericVisitor::visit (ReturnStatement * exp) {
    cout << "ReturnStatement " << "\n";
}
void GenericVisitor::visit (CallStatement * exp) {
    cout << "CallStatement " << "\n";
}

//Procedure , Module

void GenericVisitor::visit (Procedure * exp) {

    cout << "Procedure is  -- "  << exp->name << "\n";

    string str = exp->name;
    std::transform(str.begin(), str.end(), str.begin(), ::tolower); 

    vector<SymbolInfo *> formals = exp->formals;
    SymbolInfo *return_val = exp->return_val;

    // Build formal arguments
    tree *args = (tree *) alloca (sizeof(tree) * formals.size());
    int i;

    for(i =0; i<formals.size(); ++i) {

        SymbolInfo *info = formals.at(i);
        args[i] = tree_from_slang_type(info->type);
    }

    // tree from SLANG built-in type
    tree ret = tree_from_slang_type(return_val->type);

    // Build function declaration
    tree  main_type   = build_function_type_array(ret, formals.size(), args);
    tree  main_fndecl = build_function_decl (str.c_str(), false, main_type);

    // Make block , build statements 

    tree block = make_node(BLOCK);
    tree stmts = NULL_TREE ;

    vector<Statement *> proc_stmts = exp->get_statements();

    for(i =0; i<proc_stmts.size(); ++i) {

        Statement *stmt = proc_stmts.at(i); 
        stmt->accept(*this);
    } 

   // append_to_statement_list (call, &stmts);

    build_function_body (main_fndecl, stmts, block, ret);


    // Gimplify and finalize function.
    gimplify_function_tree (main_fndecl);

    //FILE *fd = fopen("/home/pradeep/Desktop/dump1.txt","w");
    //dump_function_to_file (main_fndecl, fd, 0);
    //fclose(fd);

    

    cgraph_finalize_function (main_fndecl, false);

    //Back to global scope

    current_function_decl = NULL_TREE;
    pop_cfun();
    
}
void GenericVisitor::visit (Tmodule * exp) {

    // Genericize the all procedures . 

    vector<Procedure *> procs = exp->get_procs();

    int i;
    for(i =0; i<procs.size(); ++i) {

        Procedure *proc = procs.at(i); 
        proc->accept(*this);
    }    
}



#ifdef __cplusplus
}
#endif
