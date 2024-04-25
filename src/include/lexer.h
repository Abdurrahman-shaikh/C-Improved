#ifndef LEXER_H
#define LEXER_H
#include "token.h"
#include <stdio.h>

typedef struct LEXER_STRUCT
{
  char* src;
  size_t src_size;
  char c;
  unsigned int i;
} Lexer;

Lexer* init_lexer(char* src);

void lexer_advance(Lexer* lexer);

char lexer_peek(Lexer* lexer, int offset);

Token* lexer_advance_with(Lexer* lexer, Token* token);

Token* lexer_advance_current(Lexer* lexer, int type);

void lexer_skip_whitespace(Lexer* lexer);

void lexer_skip_comment(Lexer* lexer);

Token* lexer_parse_id(Lexer* lexer);

Token* lexer_parse_number(Lexer* lexer);

Token* lexer_parse_string(Lexer* lexer);

Token* lexer_next_token(Lexer* lexer);
#endif
