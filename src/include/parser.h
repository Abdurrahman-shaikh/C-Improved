#ifndef TAC_PARSER_H
#define TAC_PARSER_H
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

//AST* parser_parse_factor(parser_T* parser);

//AST* parser_parse_term(parser_T* parser);

//AST* parser_parse_expr(parser_T* parser);

//AST* parser_parse_statement(parser_T* parser);

//AST* parser_parse_list(parser_T* parser);

//AST* parser_parse_string(parser_T* parser);

//AST* parser_parse_compound(parser_T* parser);
#endif
