#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "../lib/types.h"
#include "../lib/protocol.h"
#include "../lib/client_utils.h"

#define SERVER_NUM 827498
size_t mess_size = MESSAGE_SIZE;

int main(){
    int s_mid = msgget(SERVER_NUM, IPC_CREAT | 0644);
    int l_mid = msgget(getpid(), IPC_CREAT | 0644);
    struct query q1;
    scanf("%s", q1.name);
    q1.type = ENTER_QUERY;
    q1.num = getpid();
    msgsnd(s_mid, &q1, MESSAGE_SIZE, 0);
    if(fork()){
        struct query q;
        while(1){
            msgrcv(l_mid, &q, MESSAGE_SIZE, 0, 0);
            if(q.type == 2){
                printf("[%s] %s:%s\n", q.time, q.name, q.text);
            }else if(q.type == KICK){
                msgctl(l_mid, IPC_RMID, NULL);
                return 0;
            }else if(q.type == LIST_USERS){
                int n_users = q.num;
                printf("%d\n", n_users);
                for(int i = 0; i < n_users; i++){
                    msgrcv(l_mid, &q, MESSAGE_SIZE, LIST_USERS, 0);
                    printf("|%d: %s\n", q.num, q.name);
                }
            }
        }
    }else{
        struct query q;
        q.num = q1.num;
        strcpy(q.name, q1.name);
        char *str = q.text;
        while(1){
            printf("Numer, wiadomosc\n");
            scanf("%ld", &q.type);
            if(q.type < 130){
                // scanf("%s", q.text);
                fflush(stdin);
                getline(&str, &mess_size, stdin);
                str[strcspn(str, "\r\n")] = 0;
            }
            fflush(stdin);
            strcpy(q.time, gettime());
            msgsnd(s_mid, &q, MESSAGE_SIZE, 0);
        }
    }
    return 0;
}
    
