#include "slang-gcc.h"
#include "frontend/parser.h"
#include "frontend/context.h"
#include "frontend/compilation_unit.h"
#include "genericizer.h"
#include <cstdio>


#ifdef __cplusplus
extern "C" {

#endif

void generate_js(char *program_string) {

 Parser *p = new Parser(program_string);
 Tmodule *mod = p->do_parse();

 Runtime_Context *rc = new Runtime_Context();

 //set up file descriptor
 rc->setup_js_codegen(stdout); // stdin

 //start js codegen
 mod->generate_js(rc);

 delete p;

}

void parse_program(const char *file_name) {

   FILE *fd = fopen(file_name, "r");

   if (fd != NULL) {
        //find file size
      
        fseek(fd, 0, SEEK_END);
        int len = ftell(fd);
        fseek(fd, 0, SEEK_SET);

        //allocate memory for program text
        char *buffer = (char *) malloc(len+1);

        if (buffer == NULL) {
             fputs("Memory error", stderr);
             return;
        }

        //read program text
        int size = fread(buffer, 1, len, fd);
        string program_str(buffer);

        //parse , get all statements
        Parser *p = new Parser(program_str);
        Tmodule *mod = p->do_parse();
        
        GenericVisitor visitor;
        mod->accept(visitor);

        delete p;
    }

}

#ifdef __cplusplus
}
#endif
