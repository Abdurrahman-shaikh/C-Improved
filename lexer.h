#ifndef LEXER_H_
#define LEXER_H_

#include <stddef.h>

/**
 * Enumeration representing different types of tokens.
 */
typedef enum {  
  TOKEN_END = 0,          /**< End of content token. */
  TOKEN_INVALID,          /**< Invalid token. */
  TOKEN_PREPROC,          /**< Preprocessor directive token. */
  TOKEN_SYMBOL,           /**< Symbol token. */
  TOKEN_OPEN_PAREN,       /**< Open parenthesis token. */
  TOKEN_CLOSE_PAREN,      /**< Close parenthesis token. */
  TOKEN_OPEN_CURLY,       /**< Open curly brace token. */
  TOKEN_CLOSE_CURLY,      /**< Close curly brace token. */
  TOKEN_SEMICOLON,        /**< Semicolon token. */
  TOKEN_KEYWORD,          /**< Keyword token. */
  TOKEN_COMMENT,          /**< Comment token. */
  TOKEN_STRING,            /**< String token. */
  TOKEN_NUMBER,
  TOKEN_OPERATOR,
  TOKEN_SPECIAL_CHAR
} Token_Type;

typedef enum {
  TOKENIZER_ERROR_INVALID_CHARACTER,
  TOKENIZER_SUCCESS
} TonkeError;

typedef struct  {
  const char *file_path;
  size_t row;
}Loc;

/**
 * Returns the string representation of a token type.
 *
 * @param type The token type.
 * @return The string representation of the token type.
 */
const char *token_type_name(Token_Type type);

/**
 * Structure representing a token.
 */
typedef struct {
    Token_Type type;    /**< The type of the token. */
    const char *text;   /**< The text of the token. */
    size_t text_len;    /**< The length of the token text. */
} Token;

/**
 * Structure representing a lexer.
 */
typedef struct {
    const char *content;    /**< The content to tokenize. */
    size_t content_len;     /**< The length of the content. */
    size_t cursor;          /**< The current position in the content. */
    size_t line;            /**< The current line number. */
    size_t bol;             /**< The beginning of the current line. */
    float x;                /**< Additional position information. */
} Lexer;

/**
 * Creates a new lexer instance.
 *
 * @param content The content to tokenize.
 * @param content_len The length of the content.
 * @return The newly created lexer.
 */
Lexer lexer_new(const char *content, size_t content_len);

/**
 * Retrieves the next token from the lexer.
 *
 * @param l The lexer.
 * @return The next token.
 */
Token lexer_next(Lexer *l);

#endif // LEXER_H_

