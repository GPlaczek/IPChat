#include "../lib/types.h"
#include "../lib/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void push(struct q_list *l, struct query item){
    struct list_item *to_push = malloc(sizeof(struct list_item));
    strcpy(to_push->content.time, item.time);
    strcpy(to_push->content.name, item.name);
    strcpy(to_push->content.text, item.text);
    to_push->content.num = item.num;
    to_push->content.type = item.type;
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

void show(const struct q_list *l){
    if(l->back == NULL){
        printf("Pozdrowienia dla sąsiada\n");
        return;
    }
    for(struct list_item *temp = l->back; temp != NULL; temp = temp->next){
        printf("[%s] %s: %s\n", temp->content.time, temp->content.name, temp->content.text);
    }
}
