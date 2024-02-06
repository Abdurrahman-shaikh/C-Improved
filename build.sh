#!/bin/bash

gcc -o lexer main.c lexer.c -Wall -Wextra -pedantic

if [ $? -eq 0 ]; then
	./lexer
else
	echo "Compilation failed."
fi
