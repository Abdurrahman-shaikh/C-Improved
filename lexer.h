#ifndef LEXER_H_
#define LEXER_H_

#include <stddef.h>

typedef enum {
    TOKEN_END = 0,
    TOKEN_INVALID,
    TOKEN_PREPROC,
    TOKEN_SYMBOL,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
    TOKEN_OPEN_CURLY,
    TOKEN_CLOSE_CURLY,
    TOKEN_SEMICOLON,
    TOKEN_KEYWORD,
    TOKEN_COMMENT,
    TOKEN_STRING,
} Token_Type;

const char *token_type_name(Token_Type type);

typedef struct {
    Token_Type type;
    const char *text;
    size_t text_len;
} Token;

typedef struct {
    const char *content;
    size_t content_len;
    size_t cursor;
    size_t line;
    size_t bol;
    float x;
} Lexer;

Lexer lexer_new(const char *content, size_t content_len);
Token lexer_next(Lexer *l);

#endif // LEXER_H_

