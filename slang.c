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
#include "slang-gcc.h"

#include <stdio.h>

struct  GTY(()) lang_identifier
{
    struct tree_identifier common;
};

union GTY((desc("TREE_CODE(&%h.node)"))) lang_tree_node 
{
    union GTY((tag ("1"))) tree_node  node;
};

struct GTY(()) lang_type 
{
    char dummy; 
};

/* Language-specific declaration information.  */

struct GTY(()) lang_decl 
{
    char dummy; 
};

struct GTY(()) language_function 
{
    char dummy;
};


/* language dependent parser setup  */
bool
slang_init (void)
{
 
    build_common_tree_nodes (false, false);

    return true;
}

static bool
slang_mark_addressable (tree exp)
{
  switch (TREE_CODE (exp)){
  case STRING_CST:
    break;
  case FUNCTION_DECL:
    TREE_ADDRESSABLE (exp) = 1;
    break;
  case ARRAY_REF:
    return slang_mark_addressable (TREE_OPERAND (exp, 0));
    break;
  default:
    gcc_unreachable();
    break;
  }
  return true;
}

static tree
build_function_decl (const char *name, bool external, tree function_type)
{
    tree fndecl =  build_fn_decl (name, function_type);
    SET_DECL_ASSEMBLER_NAME(fndecl, get_identifier(name));

    DECL_EXTERNAL (fndecl) = external;
    DECL_ARTIFICIAL (fndecl) = false;
    TREE_STATIC (fndecl) = !external;
    DECL_CONTEXT (fndecl) = NULL_TREE;

    return fndecl;
}

static void
build_function (tree fndecl, tree stmts, tree block)
{
    DECL_INITIAL(fndecl) = block;
    DECL_SAVED_TREE (fndecl) = stmts ;

    tree resdecl = build_decl(input_location,RESULT_DECL, NULL_TREE, integer_type_node);
    DECL_CONTEXT (resdecl) = fndecl;
    DECL_RESULT(fndecl) = resdecl;
    current_function_decl = fndecl;

    if (DECL_STRUCT_FUNCTION(fndecl) == NULL)
        push_struct_function(fndecl);
    else
		push_cfun(DECL_STRUCT_FUNCTION(fndecl));

    cfun->function_end_locus = BUILTINS_LOCATION;

}

/* parsing language hook */
static void slang_parse_file ()
{

    int i;
    for(i = 0; i<num_in_fnames; ++i) {
        parse_program(in_fnames[i]);
    } 
#if 0
  tree  char_p = build_pointer_type (char_type_node);
  tree  puts_type   = build_function_type_list (integer_type_node,
						char_p, NULL_TREE);
  tree  puts_fndecl = build_function_decl ("puts", true, puts_type);

  tree  main_type   = build_function_type_list (integer_type_node, NULL_TREE);
  tree  main_fndecl = build_function_decl ("main", false, main_type);

  const char *msg = "HelloWorld , ... This is pradeeps compiler";
  tree hello_str = build_string_literal (strlen(msg) + 1, msg);

  tree  call = build_call_expr (puts_fndecl,1, hello_str);
  tree block = make_node(BLOCK);
  tree       c1 = build_pointer_type (char_type_node);
  tree stmts = NULL_TREE ;//alloc_stmt_list ();
  append_to_statement_list (call, &stmts);

  build_function (main_fndecl, stmts, block);

  FILE *fd = fopen("/home/pradeep/Desktop/dump.txt","w");


  gimplify_function_tree (main_fndecl);

  dump_function_to_file (main_fndecl, fd, 0);

  fclose(fd);

  cgraph_finalize_function (main_fndecl, false);

  current_function_decl = NULL_TREE;
  pop_cfun();
#endif

}

static void
slang_expand_function (tree fndecl)
{
    tree_rest_of_compilation (fndecl);
}

static tree
slang_type_for_size (unsigned precision, int unsignedp)
{
    return NULL_TREE;
}

static tree
slang_type_for_mode (enum machine_mode mode, int unsignedp)
{
    return NULL_TREE;
}

static tree
slang_pushdecl (tree decl)
{
    return NULL_TREE;

}

static void
insert_block (tree block)
{
	gcc_unreachable ();
}

static tree
getdecls (void)
{
    return NULL_TREE;
}

static bool
global_bindings_p (void)
{
	gcc_unreachable ();
}

tree
convert (tree type, tree expr)
{
	return NULL_TREE;
}

const struct attribute_spec *lookup_attribute_spec (const_tree tree) {

   return NULL;
}

void apply_tm_attr (tree t1, tree t2) {

}

tree decl_attributes (tree *a, tree b, int c) {

   return NULL_TREE;
}


#undef LANG_HOOKS_TYPE_FOR_MODE
#undef LANG_HOOKS_TYPE_FOR_SIZE
#undef LANG_HOOKS_PUSHDECL
#undef LANG_HOOKS_NAME
#undef LANG_HOOKS_INIT
#undef LANG_HOOKS_PARSE_FILE
#undef  LANG_HOOKS_MARK_ADDRESSABLE
#undef LANG_HOOKS_CALLGRAPH_EXPAND_FUNCTION


#define LANG_HOOKS_TYPE_FOR_MODE slang_type_for_mode
#define LANG_HOOKS_TYPE_FOR_SIZE slang_type_for_size
#define LANG_HOOKS_PUSHDECL		slang_pushdecl
#define LANG_HOOKS_NAME	"SLANG Frontend"
#define LANG_HOOKS_PARSE_FILE slang_parse_file
#define LANG_HOOKS_INIT slang_init
#define LANG_HOOKS_MARK_ADDRESSABLE        slang_mark_addressable
#define LANG_HOOKS_CALLGRAPH_EXPAND_FUNCTION slang_expand_function

struct lang_hooks lang_hooks = LANG_HOOKS_INITIALIZER;


#include "debug.h" 
#include "gt-slang-slang.h"
#include "gtype-slang.h"
