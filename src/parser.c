#include "include/parser.h"
#include "include/types.h"
#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Parser* init_parser(Lexer* lexer)
{
  Parser* parser = calloc(1, sizeof(struct PARSER_STRUCT));
  parser->lexer = lexer;
  parser->token = lexer_next_token(lexer);

  return parser;
}

Token* parser_eat(Parser* parser, int type)
{
  if (parser->token->type != type)
  {
    printf("[Parser]: Unexpected token: `%s`, was expecting: `%s`\n", token_to_str(parser->token), token_type_to_str(type));
    exit(1);
  }

  parser->token = lexer_next_token(parser->lexer);
  return parser->token;
}

AST* parser_parse(Parser* parser)
{
  return parser_parse_compound(parser);
}

AST* parser_parse_id(Parser* parser)
{
  char* value = calloc(strlen(parser->token->value) + 1, sizeof(char));
  strcpy(value, parser->token->value);
  parser_eat(parser, TOKEN_ID); 

  AST* ast = init_ast(AST_VARIABLE);
  ast->name = value;


  if (parser->token->type == TOKEN_COLON)
  {
    parser_eat(parser, TOKEN_COLON);

    while (parser->token->type == TOKEN_ID)
    {
      ast->dtype = typename_to_int(parser->token->value);
      parser_eat(parser, TOKEN_ID);

      if (parser->token->type == TOKEN_LT)
      {
        parser_eat(parser, TOKEN_LT);
        ast->dtype += typename_to_int(parser->token->value);
        parser_eat(parser, TOKEN_ID);
        parser_eat(parser, TOKEN_GT);
      }
    }
  }
  else
  {
    if (parser->token->type == TOKEN_LPAREN)
    {
      ast->type = AST_CALL;
      ast->value = parser_parse_list(parser);
    }
    else
    if (parser->token->type == TOKEN_LBRACKET)
    {
      parser_eat(parser, TOKEN_LBRACKET);
      ast->type = AST_ACCESS;
      ast->int_value = atoi(parser->token->value);
      parser_eat(parser, TOKEN_INT);
      parser_eat(parser, TOKEN_RBRACKET);
    }
  }

  if (parser->token->type == TOKEN_EQUALS)
  {
    parser_eat(parser, TOKEN_EQUALS);
    ast->type = AST_ASSIGNMENT;
    ast->name = value;

    ast->value = parser_parse_expr(parser);

    if (ast->value->type != AST_CALL)
      ast->value->name = mkstr(ast->name);
  }

  return ast;
}

AST* parser_parse_block(Parser* parser)
{
  parser_eat(parser, TOKEN_LBRACE);

  AST* ast = init_ast(AST_COMPOUND);

  while (parser->token->type != TOKEN_RBRACE)
  {
    list_push(ast->children, parser_parse_expr(parser));
  }

  parser_eat(parser, TOKEN_RBRACE);

  return ast;
}

AST* parser_parse_list(Parser* parser)
{
  unsigned int is_bracket = parser->token->type == TOKEN_LBRACKET;

  parser_eat(parser, is_bracket ? TOKEN_LBRACKET : TOKEN_LPAREN);

  AST* ast = init_ast(AST_COMPOUND);
  
  if (parser->token->type != (is_bracket ? TOKEN_RBRACKET : TOKEN_RPAREN))
  {
    list_push(ast->children, parser_parse_expr(parser));

    while (parser->token->type == TOKEN_COMMA)
    {
      parser_eat(parser, TOKEN_COMMA);
      list_push(ast->children, parser_parse_expr(parser));
    }
  }

  parser_eat(parser, is_bracket ? TOKEN_RBRACKET : TOKEN_RPAREN);

  if (parser->token->type == TOKEN_COLON)
  {
    parser_eat(parser, TOKEN_COLON);

    while (parser->token->type == TOKEN_ID)
    {
      ast->dtype = typename_to_int(parser->token->value);
      parser_eat(parser, TOKEN_ID);

      if (parser->token->type == TOKEN_LT)
      {
        parser_eat(parser, TOKEN_LT);
        ast->dtype += typename_to_int(parser->token->value);
        parser_eat(parser, TOKEN_ID);
        parser_eat(parser, TOKEN_GT);
      }
    }
  }

  if (parser->token->type == TOKEN_ARROW_RIGHT)
  {
    parser_eat(parser, TOKEN_ARROW_RIGHT);
    ast->type = AST_FUNCTION;
    ast->value = parser_parse_compound(parser);

    // TODO: implement this
    for (unsigned int i = 0; i < ast->children->size; i++)
      ((AST*)ast->children->items[i])->type = AST_ASSIGNMENT;
  }

  return ast;
}

AST* parser_parse_int(Parser* parser)
{
  int int_value = atoi(parser->token->value);
  parser_eat(parser, TOKEN_INT);

  AST* ast = init_ast(AST_INT);
  ast->int_value = int_value;

  return ast;
}

AST* parser_parse_string(Parser* parser)
{
  char* value = mkstr(parser->token->value);
  parser_eat(parser, TOKEN_STRING);

  AST* ast = init_ast(AST_STRING);
  ast->string_value = value;

  return ast;
}

AST* parser_parse_factor(Parser* parser)
{
  switch (parser->token->type)
  {
    case TOKEN_ID: return parser_parse_id(parser);
    case TOKEN_LPAREN: return parser_parse_list(parser);
    case TOKEN_INT: return parser_parse_int(parser);
    case TOKEN_STRING: return parser_parse_string(parser);
    default: { printf("[Parser]: Unexpected token `%s`\n", token_to_str(parser->token)); exit(1); };
  } 
}

AST* parser_parse_term(Parser* parser)
{
  AST* ast_left = parser_parse_factor(parser);

  while (
      parser->token->type == TOKEN_MUL ||
      parser->token->type == TOKEN_DIV
  )
  {
    AST* ast_binop = init_ast(AST_BINOP);
    ast_binop->left = ast_left;
    ast_binop->op = parser->token->type;
    parser_eat(parser, parser->token->type);
    ast_binop->right = parser_parse_factor(parser);
    return ast_binop;
  }  

  return ast_left;
}

AST* parser_parse_expr(Parser* parser)
{
  AST* ast_left = parser_parse_term(parser);

  while (
      parser->token->type == TOKEN_PLUS ||
      parser->token->type == TOKEN_MINUS
  )
  {
    AST* ast_binop = init_ast(AST_BINOP);
    ast_binop->left = ast_left;
    ast_binop->op = parser->token->type;
    parser_eat(parser, parser->token->type);
    ast_binop->right = parser_parse_expr(parser);
    return ast_binop;
  }
  
  return ast_left;
}

AST* parser_parse_statement(Parser* parser)
{
  // we only have return statements right now
  AST* ast = init_ast(AST_STATEMENT_RETURN);
  parser_eat(parser, TOKEN_STATEMENT);
  ast->value = parser_parse_expr(parser);

  return ast;
}

AST* parser_parse_compound(Parser* parser)
{
  unsigned int should_close = 0;

  if (parser->token->type == TOKEN_LBRACE)
  {
    parser_eat(parser, TOKEN_LBRACE);
    should_close = 1;
  }

  AST* compound = init_ast(AST_COMPOUND);

  while (parser->token->type != TOKEN_EOF && parser->token->type != TOKEN_RBRACE)
  {
    if (parser->token->type == TOKEN_STATEMENT)
      list_push(compound->children, parser_parse_statement(parser));
    else
      list_push(compound->children, parser_parse_expr(parser));

    if (parser->token->type == TOKEN_SEMI)
      parser_eat(parser, TOKEN_SEMI);
  }

  if (should_close)
    parser_eat(parser, TOKEN_RBRACE);

  return compound;
}
