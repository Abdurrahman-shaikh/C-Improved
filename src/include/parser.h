#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "AST.h"

typedef struct PARSER_STRUCT
{
  Lexer* lexer;
  Token* token;
} Parser;

Parser* init_parser(Lexer* lexer);

Token* parser_eat(Parser* parser, int type);

AST* parser_parse(Parser* parser);

AST* parser_parse_factor(Parser* parser);

AST* parser_parse_term(Parser* parser);

AST* parser_parse_expr(Parser* parser);

AST* parser_parse_statement(Parser* parser);

AST* parser_parse_list(Parser* parser);

AST* parser_parse_string(Parser* parser);

AST* parser_parse_compound(Parser* parser);
#endif
