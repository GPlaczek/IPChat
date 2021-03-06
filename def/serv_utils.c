#include "../lib/serv_utils.h"
#include "../lib/types.h"
#include "../lib/protocol.h"

#include <string.h>
#include <stdio.h>
#include <sys/msg.h>

int join_channel(const struct query *q1, struct channel channel_array[16], int *nchannels){
    char exists = 0;
    int i;
    for(i = 0; i < *nchannels; i++){ // dodawanie użytkownika do istniejącego kanału
        if(!strcmp(q1->text, channel_array[i].name)){
            if(channel_array[i].n_users > 15) return 0;
            for (int j = 0; j < channel_array[i].n_users; j++){ // sprawdzanie czy dany użytkownik nie należy już do tego kanału
                if (channel_array[i].users[j].pid == q1->num){
                    return 0;
                    break;
                }
            }
            exists = 1;
            break;
        }else if(channel_array[i].n_users == 0){
            strcpy(channel_array[i].name, q1->text);
            exists = 1;
            break;
        }
    }
    if(exists){
        strcpy(channel_array[i].users[channel_array[i].n_users].name, q1->name);
        channel_array[i].users[channel_array[i].n_users].pid = q1->num;
        channel_array[i].n_users++;
        return i+16;
    }else{ // tworzenie kanału i dodawanie do niego użytkownika
        channel_array[*nchannels].n_users = 0;
        strcpy(channel_array[*nchannels].users[channel_array[*nchannels].n_users].name, q1->name);
        strcpy(channel_array[*nchannels].name, q1->text);
        channel_array[*nchannels].users[channel_array[*nchannels].n_users].pid = q1->num;
        channel_array[*nchannels].n_users++;
        (*nchannels)++;
        return (*nchannels)+15;
    }
    return 0;
}

int exit_channel(const struct query *q1, struct channel channel_array[16], int *nchannels){
    for(int i = 0; i < *nchannels; i++){
        if(!strcmp(q1->text, channel_array[i].name)){
            for (int j = 0; j < channel_array[i].n_users; j++){
                if (channel_array[i].users[j].pid == q1->num){
                    strcpy(channel_array[i].users[j].name, channel_array[i].users[channel_array[i].n_users-1].name);
                    channel_array[i].users[j].pid = channel_array[i].users[channel_array[i].n_users-1].pid;
                    channel_array[i].n_users--;
                    return 1;
                    break;
                }
            }
        }
    }
    return 0;
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

int list_channels(const struct channel channels[16], int n_channels, int pid){
    int mid = msgget(pid, IPC_CREAT | 0644);
    struct query u1;
    u1.type = LIST_CHANNELS;
    u1.num = n_channels;
    msgsnd(mid, &u1, MESSAGE_SIZE, 0);
    if(!n_channels) return 1;
    for(int i = 0; i < n_channels; i++){
        u1.num = channels[i].n_users;
        strcpy(u1.name, channels[i].name);
        sprintf(u1.text, "%d", i+16);
        msgsnd(mid, &u1, MESSAGE_SIZE, 0);
        for(int j = 0; j < channels[i].n_users; j++){
            strcpy(u1.name, channels[i].users[j].name);
            msgsnd(mid, &u1, MESSAGE_SIZE, 0);
        }
    }
    return 0;
}

int access_to_channel(const struct channel *c, int user_pid){
    for(int i = 0; i < c->n_users; i++){
        if (c->users[i].pid == user_pid){
            return 1;
        } 
    }return 0;
}

void send_to_channel(const struct channel *c, struct query *q1){
    for(int i = 0; i < c->n_users; i++){
        int rec_q = msgget(c->users[i].pid, IPC_CREAT | 0644);
        msgsnd(rec_q, q1, MESSAGE_SIZE, 0);
        printf("%d\n", c->users[i].pid);
    }
}

void exit_all_channels(const struct query *q1, struct channel channel_array[16], int *nchannels){
    for (int i = 0; i < *nchannels; i++){
        for (int j = 0; j < channel_array[i].n_users; j++){
            if (channel_array[i].users[j].pid == q1->num){
                strcpy(channel_array[i].users[j].name, channel_array[i].users[channel_array[i].n_users-1].name);
                channel_array[i].users[j].pid = channel_array[i].users[channel_array[i].n_users-1].pid;
                channel_array[i].n_users--;
                break;
            }
        }
    }
}
