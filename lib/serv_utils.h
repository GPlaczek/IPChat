#ifndef SERV_UTILS
#define SERV_UTILS
#include "../lib/types.h"

int join_channel(const struct query*, struct channel[16], int*);
int list_users(const struct user usr_arr[16], int n_users, int mid);

#endif
