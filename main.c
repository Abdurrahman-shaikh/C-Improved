#include <stdio.h>
#include <string.h>
#include <time.h>
#include "lexer.h"
#include "parser.h"

#define MAX_CONTENT_LENGTH 10000

int main() {
    clock_t start_time = clock();

    // Code for reading from file and lexing content
    FILE *file = fopen("first.ci", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    char content[MAX_CONTENT_LENGTH];
    size_t content_len = fread(content, sizeof(char), MAX_CONTENT_LENGTH, file);
    fclose(file);
    Lexer lexer = lexer_new(content, content_len);
    
    // Lexing and printing tokens
    Token token;
    do {
        token = lexer_next(&lexer);
        printf("Token: %s\n", token_type_name(token.type));
    } while (token.type != TOKEN_END);

    // Parsing the expression from the file
    lexer = lexer_new(content, content_len);
    int result = parse_expression(&lexer);
    printf("Result: %d\n", result); // Output: Result of the parsed expression

    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.6f seconds\n", execution_time);

    return 0;
}
