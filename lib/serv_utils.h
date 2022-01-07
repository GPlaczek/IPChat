#ifndef SERV_UTILS
#define SERV_UTILS
#include "../lib/types.h"

int join_channel(const struct query*, struct channel[16], int*);
int exit_channel(const struct query*, struct channel[16], int*);
int list_users(const struct user usr_arr[16], int n_users, int pid);
int list_channels(const struct channel[16], int n_channels, int pid);
int access_to_channel(const struct channel*, int user_pid);
void send_to_channel(const struct channel*, struct query*);
void exit_all_channels(const struct query*, struct channel[16], int*);

#endif
