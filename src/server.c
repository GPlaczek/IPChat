#include "../lib/types.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

user connected_users[64];
channel channels[16];
size_t COMM_SIZE = 64;

int main(int argc, char *argv[]){
    int key;
    sscanf(argv[1], "%d", &key);
    int local_ipc = msgget(key, IPC_CREAT | 0644);
    if(fork()){
        enter_query quer;
        while(1){
            msgrcv(local_ipc, &quer, 4, 0, 0);
            if(fork()){
                int mid = msgget(quer.num, IPC_CREAT | 0644);
                message buf;
                while(1){
                    msgrcv(mid, &buf, MESSAGE_SIZE, 0, 0);
                    printf("\n%s - %s: %s\n", buf.time, buf.sender, buf.text);
                    printf("[serwer %d]$ ", key);
                }
                exit(0);
            }
        }
        exit(0);
    }else{
        char *command = malloc(COMM_SIZE);
        while(1){
            printf("[serwer %d]$ ", key);
            getline(&command, &COMM_SIZE, stdin);
            command[strcspn(command, "\r\n")] = 0;
            if(!strcmp(command, "exit")){
                kill(0, SIGKILL);
                exit(0);
            }
        }
    }
    return 0;
}
