#ifndef LIST_H
#define LIST_H
#include <stdlib.h>

typedef struct LIST_STRUCT
{
  void** items;
  size_t size;
  size_t item_size;
} List;

List* init_list(size_t item_size);

void list_push(List* list, void* item);

int list_indexof_str(List* list, char* item);
#endif
