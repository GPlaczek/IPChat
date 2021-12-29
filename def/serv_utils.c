#include "../lib/serv_utils.h"
#include "../lib/types.h"
#include "../lib/protocol.h"

#include <string.h>
#include <stdio.h>
#include <sys/msg.h>

int join_channel(const struct query *q1, struct channel channel_array[16], int *nchannels){
    char exists = 0;
    for(int i = 0; i < *nchannels; i++){
        if(!strcmp(q1->text, channel_array[i].name)){
            strcpy(channel_array[i].users[channel_array[i].n_users].name, q1->name);
            channel_array[i].users[channel_array[i].n_users].pid = q1->num;
            channel_array[i].n_users++;
            exists = 1;
            return 0;
            break;
        }
    }
    if(!exists){
        channel_array[*nchannels].n_users = 0;
        strcpy(channel_array[*nchannels].users[channel_array[*nchannels].n_users].name, q1->name);
        strcpy(channel_array[*nchannels].name, q1->text);
        channel_array[*nchannels].users[channel_array[*nchannels].n_users].pid = q1->num;
        channel_array[*nchannels].n_users++;
        (*nchannels)++;
        return 1;
    }
    return -1;
}

int list_users(const struct user user_array[16], int n_users, int pid){
    int mid = msgget(pid, IPC_CREAT | 0644);
    struct query u1;
    u1.type = LIST_USERS;
    u1.num = n_users;
    msgsnd(mid, &u1, MESSAGE_SIZE, 0);
    if(!n_users) return 1;
    for(int i = 0; i < n_users; i++){
        u1.num= i+1;
        strcpy(u1.name, user_array[i].name);
        msgsnd(mid, &u1, MESSAGE_SIZE, 0);
    }
    return 0;
}

void send_to_channel(const struct channel *c, struct query *q1){
    q1->type = 2;
    for(int i = 0; i < c->n_users; i++){
        int rec_q = msgget(c->users[i].pid, IPC_CREAT | 0644);
        msgsnd(rec_q, q1, MESSAGE_SIZE, 0);
        printf("%d\n", c->users[i].pid);
    }
}
