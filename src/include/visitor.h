#ifndef VISITOR_H
#define VISITOR_H
#include "AST.h"
#include "list.h"
#include "stack_frame.h"

typedef struct VISITOR_STRUCT
{
  AST* object;
} Visitor;

Visitor* init_visitor();

AST* visitor_visit(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame);

AST* visitor_visit_compound(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame);
AST* visitor_visit_assignment(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame);
AST* visitor_visit_variable(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame);
AST* visitor_visit_function(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame);
AST* visitor_visit_call(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame);
AST* visitor_visit_int(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame);
AST* visitor_visit_string(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame);
AST* visitor_visit_binop(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame);
AST* visitor_visit_statement_return(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame);
AST* visitor_visit_access(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame);
#endif
