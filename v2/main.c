#include <stdio.h>
#include <string.h>
#include <time.h>
#include "lexer.h"

#define MAX_CONTENT_LENGTH 10000

int main() {
    clock_t start_time = clock();

    FILE *file = fopen("first.ci", "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char content[MAX_CONTENT_LENGTH];
    size_t content_len = 0;

    content_len = fread(content, sizeof(char), MAX_CONTENT_LENGTH, file);

    fclose(file);

    Lexer lexer = lexer_new(content, content_len);

    Token token;
    do {
        token = lexer_next(&lexer);
        printf("Token: %s\n", token_type_name(token.type));
    } while (token.type != TOKEN_END);

    clock_t end_time = clock();

    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Execution time: %.6f seconds\n", execution_time);

    return 0;
}

