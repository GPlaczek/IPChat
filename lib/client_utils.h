#ifndef CLIENT_UTILS
#define CLIENT_UTILS
#include "list.h"

struct channel_buffer{
    struct q_list buffer;
    int num;
    char name[16];
};
char *gettime();

#endif
