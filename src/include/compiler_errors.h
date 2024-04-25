#ifndef COMPILER_ERRORS_H
#define COMPILER_ERRORS_H
#include "AST.h"
void assert_call_matches_signature(AST* call_node, AST* func_node);
#endif
