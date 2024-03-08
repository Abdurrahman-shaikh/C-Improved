#ifndef TAC_TOKEN_H
#define TAC_TOKEN_H
typedef struct TOKEN_STRUCT
{
  char* value;
  enum
  {
    TOKEN_ID,
    TOKEN_EQUALS,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_ARROW_RIGHT,
    TOKEN_INT,
    TOKEN_STRING,
    TOKEN_STATEMENT,
    TOKEN_SEMI,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_DIV,
    TOKEN_MUL,
    TOKEN_EOF,
  } type;
} Token;

Token* init_token(char* value, int type);

const char* token_type_to_str(int type);

char* token_to_str(Token* token);
#endif
