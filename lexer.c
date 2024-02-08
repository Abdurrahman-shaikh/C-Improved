#include <assert.h>
#include <stdio.h>
#include "lexer.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// Define a structure to represent literal tokens
typedef struct {
    Token_Type type;
    const char *text;
} Literal_Token;

// Initialize an array of literal tokens with their corresponding types
Literal_Token literal_tokens[] = {
    {.text = "(", .type = TOKEN_OPEN_PAREN},
    {.text = ")", .type = TOKEN_CLOSE_PAREN},
    {.text = "{", .type = TOKEN_OPEN_CURLY},
    {.text = "}", .type = TOKEN_CLOSE_CURLY},
    {.text = ";", .type = TOKEN_SEMICOLON},
};

// Define a macro to calculate the number of elements in the literal_tokens array
#define literal_tokens_count (sizeof(literal_tokens)/sizeof(literal_tokens[0]))


// Define an array of keywords
const char *keywords[] = {
    "azmi", "abstract", "assert", "boolean", "break",
    "byte", "case", "catch", "char", "class", "const",
    "continue", "default", "do", "double", "else",
    "enum", "extends", "final", "finally", "float",
    "for", "goto", "if", "implements", "import",
    "instanceof", "int", "interface", "long", "native",
    "new", "null", "package", "private", "protected",
    "public", "return", "short", "static", "strictfp",
    "super", "switch", "synchronized", "this", "throw",
    "throws", "transient", "try", "void", "volatile",
    "while", "auto", "break", "case", "char", "const",
    "continue", "default", "do", "double", "else",
    "enum", "extern", "float", "for", "goto","if",
    "inline", "int", "long", "register", "restrict",
    "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned",
    "void", "volatile", "while"
};


// Define a macro to calculate the number of elements in the keywords array
#define keywords_count (sizeof(keywords)/sizeof(keywords[0]))


// Function to return a string representation of a Token_Type
const char *token_type_name(Token_Type type) {
    switch (type) {
    case TOKEN_END:
        return "end of content";
    case TOKEN_INVALID:
        return "invalid token";
    case TOKEN_PREPROC:
        return "preprocessor directive";
    case TOKEN_SYMBOL:
        return "symbol";
    case TOKEN_OPEN_PAREN:
        return "open paren";
    case TOKEN_CLOSE_PAREN:
        return "close paren";
    case TOKEN_OPEN_CURLY:
        return "open curly";
    case TOKEN_CLOSE_CURLY:
        return "close curly";
    case TOKEN_SEMICOLON:
        return "semicolon";
    case TOKEN_KEYWORD:
        return "keyword";
    default:
        printf("invalid token");
    }
    return NULL;
}


// Function to create a new Lexer instance
Lexer lexer_new(const char *content, size_t content_len) {
    Lexer l = {0};
    l.content = content;
    l.content_len = content_len;
    return l;
}

// Function to check if a Lexer's content starts with a given prefix
bool lexer_starts_with(Lexer *l, const char *prefix) {
    size_t prefix_len = strlen(prefix);
    if (prefix_len == 0) {
        return true;
    }
    if (l->cursor + prefix_len - 1 >= l->content_len) {
        return false;
    }
    for (size_t i = 0; i < prefix_len; ++i) {
        if (prefix[i] != l->content[l->cursor + i]) {
            return false;
        }
    }
    return true;
}


// Function to advance the cursor of a Lexer by a given number of characters
void lexer_chop_char(Lexer *l, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        assert(l->cursor < l->content_len);
        char x = l->content[l->cursor];
        l->cursor += 1;
        if (x == '\n') {
            l->line += 1;
            l->bol = l->cursor;
            l->x = 0;
        }
    }
}

// Function to trim whitespace characters from the beginning of a Lexer's content
void trim_left(Lexer *l) {
    while (l->cursor < l->content_len && isspace(l->content[l->cursor])) {
        lexer_chop_char(l, 1);
    }
}

// Function to check if a character is a valid symbol start character
bool is_symbol_start(char x) {
    return isalpha(x) || x == '_';
}

// Function to check if a character is a valid symbol character
bool is_symbol(char x) {
    return isalnum(x) || x == '_';
}


// Function to get the next token from a Lexer
Token lexer_next(Lexer *l) {
    trim_left(l);

    Token token = {
        .text = &l->content[l->cursor],
    };

    if (l->cursor >= l->content_len) return token;

    if (l->content[l->cursor] == '"') {
        token.type = TOKEN_STRING;
        lexer_chop_char(l, 1);
        while (l->cursor < l->content_len && l->content[l->cursor] != '"' && l->content[l->cursor] != '\n') {
            lexer_chop_char(l, 1);
        }
        if (l->cursor < l->content_len) {
            lexer_chop_char(l, 1);
        }
        token.text_len = &l->content[l->cursor] - token.text;
        return token;
    }

    if (l->content[l->cursor] == '#') {
        token.type = TOKEN_PREPROC;
        while (l->cursor < l->content_len && l->content[l->cursor] != '\n') {
            lexer_chop_char(l, 1);
        }
        if (l->cursor < l->content_len) {
            lexer_chop_char(l, 1);
        }
        token.text_len = &l->content[l->cursor] - token.text;
        return token;
    }

    if (lexer_starts_with(l, "//")) {
        token.type = TOKEN_COMMENT;
        while (l->cursor < l->content_len && l->content[l->cursor] != '\n') {
            lexer_chop_char(l, 1);
        }
        if (l->cursor < l->content_len) {
            lexer_chop_char(l, 1);
        }
        token.text_len = &l->content[l->cursor] - token.text;
        return token;
    }
    
    for (size_t i = 0; i < literal_tokens_count; ++i) {
        if (lexer_starts_with(l, literal_tokens[i].text)) {
            size_t text_len = strlen(literal_tokens[i].text);
            token.type = literal_tokens[i].type;
            token.text_len = text_len;
            lexer_chop_char(l, text_len);
            return token;
        }
    }

    if (is_symbol_start(l->content[l->cursor])) {
        token.type = TOKEN_SYMBOL;
        while (l->cursor < l->content_len && is_symbol(l->content[l->cursor])) {
            lexer_chop_char(l, 1);
            token.text_len += 1;
        }

        for (size_t i = 0; i < keywords_count; ++i) {
            size_t keyword_len = strlen(keywords[i]);
            if (keyword_len == token.text_len && memcmp(keywords[i], token.text, keyword_len) == 0) {
                token.type = TOKEN_KEYWORD;
                break;
            }
        }

        return token;
    }

    lexer_chop_char(l, 1);
    token.type = TOKEN_INVALID;
    token.text_len = 1;
    printf("\033[1;31mInvalid token: \033[0m\033[1;41m%.*s\033[0m at position %zu\n", (int)token.text_len, token.text, l->cursor - token.text_len);
    return token;
}

