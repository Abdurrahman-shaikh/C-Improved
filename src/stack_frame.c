#include "include/stack_frame.h"


Stack_frame* init_stack_frame()
{
  Stack_frame* stack_frame = calloc(1, sizeof(struct STACK_FRAME_STRUCT));
  stack_frame->stack = init_list(sizeof(char*));

  return stack_frame;
}
