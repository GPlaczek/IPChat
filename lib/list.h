#ifndef LIST
#define LIST

#include "types.h"

#define MAX_SIZE 10

struct q_list{
    struct list_item *front;
    struct list_item *back;
    unsigned int size;
};

struct list_item{
    struct query content;
    struct list_item *next;
    struct list_item *prev;
};

void push(struct q_list*, struct query item);
void init(struct q_list*);

#endif
