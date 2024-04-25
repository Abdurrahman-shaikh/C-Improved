#ifndef STACK_FRAME_H
#define STACK_FRAME_H
#include "list.h"
typedef struct STACK_FRAME_STRUCT
{
  List* stack;
} Stack_frame;

Stack_frame* init_stack_frame();
#endif
