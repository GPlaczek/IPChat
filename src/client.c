#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#include "../lib/types.h"
#include "../lib/protocol.h"
#include "../lib/client_utils.h"

#define SERVER_NUM 827498
size_t mess_size = MESSAGE_SIZE;

struct channel_buffer buffers[16];

int main(){
    int s_mid = msgget(SERVER_NUM, IPC_CREAT | 0644);
    int l_mid = msgget(getpid(), IPC_CREAT | 0644);
    int nchannels = 0;
    struct query q1;
    printf("Podaj nazwę użytkownika: ");
    scanf("%s", q1.name);
    q1.type = ENTER_QUERY;
    q1.num = getpid();
    msgsnd(s_mid, &q1, MESSAGE_SIZE, 0);
    if(fork()){
        struct query q;
        while(1){
            msgrcv(l_mid, &q, MESSAGE_SIZE, 0, 0);
            if(q.type == 2){
                bold_text();
                printf("{private} [%s] %s:%s\n", q.time, q.name, q.text);
                reset_text(); // do naprawy
            }else if(q.type > 15 && q.type < 32){
                for(int i = 0; i < nchannels; i++){
                    if(buffers[i].num == q.type){
                        push(&buffers[i].buffer, q);
                        break;
                    }
                }
            }else if(q.type == ENTER_QUERY){
                system("clear");
                bold_text();
                printf("Witaj na czacie %s\n", q1.name);
                reset_text();
                instructions();
            }else if(q.type == KICK){
                msgctl(l_mid, IPC_RMID, NULL);
                kill(0, SIGKILL);
                return 0;
            }else if(q.type == CHANNEL){
                if(q.num){
                    init(&buffers[nchannels].buffer);
                    buffers[nchannels].num = q.num;
                    strcpy(buffers[nchannels].name, q.text);
                    nchannels++;
                    printf("Udało się uzyskać połączenie z kanałem\n");
                }else{
                    printf("Już należysz do kanału o podanej nazwie\n");
                }
            }else if(q.type == EXIT_CHANNEL){
                if(q.num){
                    printf("Zostałeś wyrejestrowany z kanału %s\n", q.text);
                }
                else{
                    printf("Kanał o podanej nazwie nie istnieje lub do niego nie należysz\n");
                }
            }else if(q.type == LIST_USERS){
                int n_users = q.num;
                for(int i = 0; i < n_users; i++){
                    msgrcv(l_mid, &q, MESSAGE_SIZE, LIST_USERS, 0);
                    printf("|%d: %s\n", q.num, q.name);
                }
            }else if(q.type == LIST_CHANNELS){
                int n_channels = q.num;
                for(int i = 0; i < n_channels; i++){
                    msgrcv(l_mid, &q, MESSAGE_SIZE, LIST_CHANNELS, 0);
                    printf("%s %s\n", q.text, q.name);
                    int n_users = q.num;
                    for(int j = 0; j < n_users; j++){
                        msgrcv(l_mid, &q, MESSAGE_SIZE, LIST_CHANNELS, 0);
                        printf("\t|%s\n", q.name);
                    }
                }
            }else if(q.type > 255 && q.type < 272){
                bold_text();
                for(int i = 0; i < nchannels; i++){
                    if(buffers[i].num == q.type - 240){
                        printf("%s:\n", buffers[i].name);
                        printf("---------\n");
                        show(&buffers[i].buffer);
                    }
                }reset_text();
            }else if (q.type == LOGIN_FAILED){
                printf("Odmowa dostępu. Użytkownik o podanej nazwie jest już zalogowany w systemie.\n");
                msgctl(l_mid, IPC_RMID, NULL);
                return 0;
            }else if(q.type == ERROR){printf("Coś poszło nie tak...\n");
            }else if(q.type == HELP){instructions();
            }else if(q.type == CLEAR){system("clear");}
        }
    }else{
        struct query q;
        q.num = q1.num;
        strcpy(q.name, q1.name);
        char *str = q.text;
        while(1){
            scanf("%ld", &q.type);
            if(q.type < 130){
                fflush(stdin);
                getline(&str, &mess_size, stdin);
                str[strcspn(str, "\r\n")] = 0;
            }
            if(q.type < 256){
                fflush(stdin);
                strcpy(q.time, gettime());
                msgsnd(s_mid, &q, MESSAGE_SIZE, 0);
            }else if(q.type < 272){
                msgsnd(l_mid, &q, MESSAGE_SIZE, 0);
            }
        }
    }
    return 0;
} 
