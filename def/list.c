#include "../lib/types.h"
#include "../lib/list.h"
#include <stdlib.h>

void push(struct q_list *l, struct query item){
    struct list_item *to_push = malloc(sizeof(struct list_item));
    to_push->content = item;
    l->size++;
    if(l->front == NULL){
        to_push->next = NULL;
        to_push->prev = NULL;
        l->front = to_push;
        l->back = to_push;
    }else{
        to_push->next = NULL;
        to_push->prev = l->front;
        l->front->next = to_push;
        l->front = to_push;
    }
    if(l->size == MAX_SIZE + 1){
        l->size--;
        struct list_item *temp = l->back;
        l->back = temp->next;
        l->back->prev = NULL;
        free(temp);
    }
}

void init(struct q_list *l){
    l->back = NULL;
    l->front = NULL;
    l->size = 0;
}
