#ifndef PARSER_H_
#define PARSER_H_

#include "lexer.h"

int parse_expression(Lexer *lexer);
int parse_term(Lexer *lexer);
int parse_factor(Lexer *lexer);

#endif /* PARSER_H_ */
