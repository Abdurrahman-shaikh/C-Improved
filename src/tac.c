#include "include/tac.h"
#include "include/lexer.h"
#include "include/io.h"
#include "include/parser.h"
#include <stdlib.h>


void tac_compile(char* src)
{
  Lexer* lexer = init_lexer(src);
  Parser* parser = init_parser(lexer);
  AST* root = parser_parse(parser);
  printf("%d\n",root->type);
  Token* tok = 0;

  /*char* s = as_f_root(optimized_root, init_list(sizeof(struct AST_STRUCT*)));

  tac_write_file("a.s", s);
  tac_write_file("a.s.txt", s);
  sh("as --32 a.s -o a.o");
  sh("ld a.o -o a.out -m elf_i386");
  */

  while ((tok = lexer_next_token(lexer))->type != TOKEN_EOF) {
    printf("%s\n",token_to_str(tok));
  }
}

void tac_compile_file(const char* filename)
{
  char* src = tac_read_file(filename);
  tac_compile(src);
  free(src);
}
