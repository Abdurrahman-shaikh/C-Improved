#ifndef BUILTINS_H
#define BUILTINS_H
#include "visitor.h"

AST* fptr_print(Visitor* visitor, AST* node, List* list);
void builtins_init(List* list);
#endif
