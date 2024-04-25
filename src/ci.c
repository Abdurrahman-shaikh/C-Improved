#include "include/ci.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/io.h"
#include "include/visitor.h"
#include "include/as_frontend.h"


void compile(char* src)
{
  Lexer* lexer = init_lexer(src);
  Parser* parser = init_parser(lexer);
  AST* root = parser_parse(parser);

  Visitor* visitor = init_visitor();
  AST* optimized_root = visitor_visit(visitor, root, init_list(sizeof(struct AST_STRUCT*)), init_stack_frame());
  
  char* s = as_f_root(optimized_root, init_list(sizeof(struct AST_STRUCT*)));

  write_file("a.s", s);
  write_file("a.s.txt", s);
  sh("as --32 a.s -o a.o");
  sh("ld a.o -o a.out -m elf_i386");
}

void compile_file(const char* filename)
{
  char* src = read_file(filename);
  compile(src);
  free(src);
}
