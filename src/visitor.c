#include "include/visitor.h"
#include "include/compiler_errors.h"
#include "include/utils.h"
#include <stdio.h>
#include <string.h>

static AST* var_lookup(List* list, const char* name)
{
  for (int i = 0; i < (int) list->size; i++)
  {
    AST* child_ast = (AST*) list->items[i];

    if ((child_ast->type != AST_VARIABLE && child_ast->type != AST_FUNCTION) || !child_ast->name)
      continue;

    if (strcmp(child_ast->name, name) == 0)
      return child_ast;
  }

  return 0;
}

Visitor* init_visitor()
{
  Visitor* visitor = calloc(1, sizeof(struct VISITOR_STRUCT));
  visitor->object = init_ast(AST_COMPOUND);

  return visitor;
}

AST* visitor_visit(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame)
{
  switch (node->type)
  {
    case AST_COMPOUND: return visitor_visit_compound(visitor, node, list, stack_frame); break;
    case AST_ASSIGNMENT: return visitor_visit_assignment(visitor, node, list, stack_frame); break;
    case AST_VARIABLE: return visitor_visit_variable(visitor, node, list, stack_frame); break;
    case AST_CALL: return visitor_visit_call(visitor, node, list, stack_frame); break;
    case AST_INT: return visitor_visit_int(visitor, node, list, stack_frame); break;
    case AST_ACCESS: return visitor_visit_access(visitor, node, list, stack_frame); break;
    case AST_FUNCTION: return visitor_visit_function(visitor, node, list, stack_frame); break;
    case AST_STRING: return visitor_visit_string(visitor, node, list, stack_frame); break;
    case AST_BINOP: return visitor_visit_binop(visitor, node, list, stack_frame); break;
    case AST_STATEMENT_RETURN: return visitor_visit_statement_return(visitor, node, list, stack_frame); break;
    default: { printf("[Visitor]: Don't know how to handle AST of type `%d`\n", node->type); exit(1); } break;
  }

  return node;
}

AST* visitor_visit_compound(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame)
{
  AST* compound = init_ast(AST_COMPOUND);
  compound->stack_frame = stack_frame;

  for (unsigned int i = 0; i < node->children->size; i++)
  {
    AST* x = visitor_visit(visitor, (AST*) node->children->items[i], list, stack_frame);
    list_push(compound->children, x);
  }

  return compound;
}

AST* visitor_visit_assignment(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame)
{
  AST* new_var = init_ast(AST_ASSIGNMENT);
  new_var->name = node->name;
  new_var->dtype = node->dtype;

  if (node->value)
    new_var->value = visitor_visit(visitor, node->value, list, stack_frame);
  
  //if (list->size == 0)
  //  list_push(list, new_var);

  //if (new_var->value)
  //if (new_var->value->type == AST_FUNCTION)
  //  list_push(visitor->object->children, new_var->value);
  

  new_var->stack_index = stack_frame->stack->size;
  new_var->stack_frame = stack_frame;
  list_push(stack_frame->stack, new_var->name);

  return new_var;
}

AST* visitor_visit_variable(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame)
{
  list_push(stack_frame->stack, 0);

  int index = 0;

  for (unsigned int i = 0; i < list->size; i++)
  {
    AST* child = (AST*) list->items[i];
    
    if (!child->name)
      continue;

    if (strcmp(child->name, node->name) == 0)
    {
      index = i + 1;
      break;
    }
  }

  node->stack_index = index ? (index + 1) : list_indexof_str(stack_frame->stack, node->name);
  node->stack_frame = stack_frame;

  return node;
}

AST* visitor_visit_function(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame)
{
  AST* func = init_ast(AST_FUNCTION);
  func->name = node->name;
  func->dtype = node->dtype;
  func->children = init_list(sizeof(struct AST_STRUCT*));

  Stack_frame* new_stack_frame = init_stack_frame();
  list_push(new_stack_frame->stack, 0); 
  list_push(new_stack_frame->stack, 0); 

  for (unsigned int i = 0; i < node->children->size; i++)
    list_push(func->children, (AST*) visitor_visit(visitor, (AST*) node->children->items[i], list, new_stack_frame));
  
  for (unsigned int i = 0; i < func->children->size; i++)
    list_push(list, func->children->items[i]);

  func->value = visitor_visit(visitor, node->value, list, new_stack_frame);
  func->stack_frame = new_stack_frame;

  return func;
}

AST* visitor_visit_call(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame)
{
  AST* var = var_lookup(visitor->object->children, node->name);

  List* new_args = init_list(sizeof(struct AST_STRUCT*));

  for (unsigned int i = 0; i < node->value->children->size; i++)
    list_push(new_args, visitor_visit(visitor, (AST*)node->value->children->items[i], list, stack_frame));

  if (var)
  {
    /* Not in use at the moment
     *
     * if (var->fptr)
    {
      AST* ret = var->fptr(visitor, node, new_args);
      ret->stack_frame = stack_frame;
      return ret;
    }*/

    assert_call_matches_signature(node, var); 
  }

  node->stack_frame = stack_frame;

  return node;
}

AST* visitor_visit_int(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame)
{
  node->stack_index = -((int)stack_frame->stack->size);
  node->stack_frame = stack_frame;
  list_push(stack_frame->stack, mkstr("0"));
  return node;
}

AST* visitor_visit_string(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame)
{
  List* chunks = str_to_hex_chunks(node->string_value);
  
  list_push(stack_frame->stack, 0);
  node->stack_index = -(stack_frame->stack->size + chunks->size);

  return node;
}

AST* visitor_visit_binop(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame)
{
  AST* new_binop = init_ast(AST_BINOP);
  new_binop->left = visitor_visit(visitor, node->left, list, stack_frame);
  new_binop->op = node->op;
  new_binop->right = visitor_visit(visitor, node->right, list, stack_frame);
  return new_binop;
}

AST* visitor_visit_statement_return(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame)
{
  AST* ast = init_ast(AST_STATEMENT_RETURN);
  ast->value = visitor_visit(visitor, node->value, list, stack_frame);
  return ast;
}

AST* visitor_visit_access(Visitor* visitor, AST* node, List* list, Stack_frame* stack_frame)
{
  list_push(stack_frame->stack, 0);
  node->stack_index = (list_indexof_str(stack_frame->stack, node->name));

  return node;
}
