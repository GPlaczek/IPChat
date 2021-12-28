#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "../lib/types.h"
#include "../lib/protocol.h"
#include "../lib/serv_utils.h"

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
            printf("%d\n", join_channel(&q1, channel_array, &nchannels));
        }else if(q1.type == LIST_USERS){
            list_users(user_array, nusers, q1.num);
        }else if(q1.type == EXIT){
            int mid = msgget(q1.num, IPC_CREAT | 0644);
            q1.type = KICK;
            int to_delete;
            for (int i = 0; i < nusers; i++){
                if (!strcmp(q1.name, user_array[i].name)) {to_delete = i; break;}
            }
            nusers--;
            user_array[to_delete].pid = user_array[nusers].pid;
            strcpy(user_array[to_delete].name, user_array[nusers].name);
            nusers--;
            msgsnd(mid, &q1, MESSAGE_SIZE, 0);
        }
    }
    return 0;
}
