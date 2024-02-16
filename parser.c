#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

// Function prototypes
int parse_expression(Lexer *lexer);
int parse_term(Lexer *lexer);
int parse_factor(Lexer *lexer);

// Global variables
Lexer *current_lexer;
Token current_token;

// Error handling function
void error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
}

// Advance to the next token
void next_token() {
    current_token = lexer_next(current_lexer);
}

// Match the current token type with an expected type
bool match(Token_Type expected_type) {
    if (current_token.type == expected_type) {
        next_token();
        return true;
    }
    return false;
}

// <Expression> ::= <Term> | <Expression> '+' <Term>
int parse_expression(Lexer *lexer) {
    current_lexer = lexer;
    next_token();
    int result = parse_term(lexer);
    
    while (current_token.type == TOKEN_OPERATOR && current_token.text[0] == '+') {
        next_token();
        result += parse_term(lexer);
    }
    
    return result;
}

// <Term> ::= <Factor> | <Term> '*' <Factor>
int parse_term(Lexer *lexer) {
    int result = parse_factor(lexer);
    
    while (current_token.type == TOKEN_OPERATOR && current_token.text[0] == '*') {
        next_token();
        result *= parse_factor(lexer);
    }
    
    return result;
}

// <Factor> ::= '(' <Expression> ')' | <Number> | <Symbol>
int parse_factor(Lexer *lexer) {
    if (current_token.type == TOKEN_OPEN_PAREN) {
        next_token();
        int result = parse_expression(lexer);
        if (!match(TOKEN_CLOSE_PAREN)) {
            error("Expected ')' after expression");
        }
        return result;
    } else if (current_token.type == TOKEN_NUMBER) {
        int result = atoi(current_token.text);
        next_token();
        return result;
    } else if (current_token.type == TOKEN_SYMBOL) {
        // You can handle symbols here if needed
        next_token();
        return 0; // Placeholder for symbol handling
    } else {
        error("Unexpected token");
        return 0;
    }
}

