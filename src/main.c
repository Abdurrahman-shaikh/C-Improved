#include "include/tac.h"
#include <stdio.h>
#include <stdlib.h>

// Function declaration
//void tac_compile_file(const char* filename);
//char* itos(int num, const char* delimiter);
//void print(const char* str);

int main(int argc, char* argv[]) {
    // Check if filename argument is provided
    if (argc < 2) {
        // If filename is not provided, display an error message
        fprintf(stderr, "\033[1;31mPlease specify input file.\033[0m\n");
        return 1;
    }

    /* Attempt to open the input file
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        // If file is not found, execute the provided code stored in a string

        // Define the provided code as a string
        const char* provided_code = "\
            main = (argc: int, argv: Array<string>):int => {\n\
                x: number = 10 + 10 + 5 + 6;\n\
                y: string = itos(x, \" \");\n\
                print(y);\n\
                return 0;\n\
            }";

        // Print a message indicating that the file is not found
        fprintf(stderr, "\033[1;31mInput file not found. Using provided code.\033[0m\n");

        // Execute the provided code
        // (Note: You'll need to implement the functionality to execute the provided code)
        // For demonstration purposes, we're just printing the provided code here
        printf("Provided code:\n%s\n", provided_code);

        return 0;
    } */

    // If file is found, continue with the original logic to compile the file
    tac_compile_file(argv[1]);

    return 0;
}

