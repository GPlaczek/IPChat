#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

#include "../lib/types.h"
#include "../lib/protocol.h"

#define SERVER_NUM 827498 

struct user user_array[16];
struct channel channel_array[16];

int main(){
    int mid = msgget(SERVER_NUM, IPC_CREAT | 0644);
    int nusers = 0;
    int nchannels = 0;
    struct query q1;
    while(1){
        msgrcv(mid, &q1, MESSAGE_SIZE, 0, 0);
        printf("%ld\n", q1.type);
        if(q1.type == ENTER_QUERY){
            char enters = 1;
            for(int i = 0; i < nusers; i++){
                if(!strcmp(q1.name, user_array[i].name)){ enters = 0; break; }
            }
            if(enters){
                user_array[nusers].pid = q1.num;
                strcpy(user_array[nusers].name, q1.name);
                nusers++;
            }else{
                int mid = msgget(q1.num, IPC_CREAT | 0644);
                q1.type = KICK;
                msgsnd(mid, &q1, MESSAGE_SIZE, 0);
            }
        }else if(q1.type < 16){
            int rec_q = msgget(user_array[q1.type - 1].pid, IPC_CREAT | 0644);
            q1.type = 2;
            msgsnd(rec_q, &q1, MESSAGE_SIZE, 0);
        }else if(q1.type == CHANNEL){
            char exists = 0;
            for(int i = 0; i < nchannels; i++){
                if(!strcmp(q1.text, channel_array[i].name)){
                    strcpy(channel_array[i].users[channel_array[i].n_users].name, q1.name);
                    channel_array[i].users[channel_array[i].n_users].pid = q1.num;
                    channel_array[i].n_users++;
                    exists = 1;
                    printf("Joined channel\n");
                    break;
                }
            }
            if(!exists){
                channel_array[nchannels].n_users = 0;
                strcpy(channel_array[nchannels].users[channel_array[nchannels].n_users].name, q1.name);
                strcpy(channel_array[nchannels].name, q1.text);
                channel_array[nchannels].users[channel_array[nchannels].n_users].pid = q1.num;
                channel_array[nchannels].n_users++;
                nchannels++;
                printf("Created channel\n");
            }
        }
    }
    return 0;
}
