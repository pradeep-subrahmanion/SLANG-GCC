/* 
   This is the contribution to the `default_compilers' array in gcc.c for
   treelang.  
   
   This file must compile with 'traditional', so no ANSI string concatenations
*/

{".sl", "@slang", NULL, 0, 0},
{"@slang", "slang1 %i %(invoke_as)", NULL, 0, 0},
