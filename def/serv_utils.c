#include "../lib/serv_utils.h"
#include "../lib/types.h"
#include <string.h>
#include <stdio.h>

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
