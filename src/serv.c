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
            // PODŁĄCZENIE UŻYTKOWNIKÓW DO SERWERA
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
            // PRZESYŁANIE WIADOMOŚCI PRYWATNYCH
            int rec_q = msgget(user_array[q1.type - 1].pid, IPC_CREAT | 0644);
            q1.type = 2;
            msgsnd(rec_q, &q1, MESSAGE_SIZE, 0);
        }else if(q1.type < 32){
            // PRZESYŁANIE WIADOMOŚCI DO WSZYSTKICH UŻYTKOWNIKÓW POŁĄCZONYCH Z KANAŁEM
            int chnl = CHANNEL_NUM(q1.type);
            q1.num = q1.type;
            send_to_channel(&channel_array[chnl], &q1);
        }else if(q1.type == CHANNEL){
            // PODŁĄCZANIE UŻYTKOWNIKÓW DO KANAŁU
            int mid = msgget(q1.num, IPC_CREAT | 0644);
            q1.num = join_channel(&q1, channel_array, &nchannels);
            msgsnd(mid, &q1, MESSAGE_SIZE, 0);
        }else if(q1.type == LIST_USERS){
            // WYSYŁANIE UŻYTKOWNIKOM LISTY UŻYTKOWNIKÓW PODŁĄCZONYCH DO SERWERA
            list_users(user_array, nusers, q1.num);
        }else if(q1.type == LIST_CHANNELS){
            list_channels(channel_array, nchannels, q1.num);
        }else if(q1.type == EXIT){
            // ROZŁĄCZANIE UŻYTKOWNIKÓW
            int mid = msgget(q1.num, IPC_CREAT | 0644);
            q1.type = KICK;
            int to_delete;
            for (int i = 0; i < nusers; i++){
                if (!strcmp(q1.name, user_array[i].name)) {to_delete = i; break;}
            }
            nusers--;
            user_array[to_delete].pid = user_array[nusers].pid;
            strcpy(user_array[to_delete].name, user_array[nusers].name);
            msgsnd(mid, &q1, MESSAGE_SIZE, 0);
            fflush(stdout);
        }
    }
    return 0;
}
