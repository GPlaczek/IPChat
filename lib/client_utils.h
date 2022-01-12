#ifndef CLIENT_UTILS
#define CLIENT_UTILS
#include "list.h"
#include "protocol.h"

struct channel_buffer{
    struct q_list buffer;
    int num;
    char name[16];
};
char *gettime();
void instructions();
void bold_text();
void reset_text();

#endif
