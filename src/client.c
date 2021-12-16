#include "../lib/types.h"
#include "../lib/command.h"
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

size_t COMM_SIZE = 64;

int main(int argc, char *argv[]){
    char user_name[16];
    scanf("%15s", user_name);
    fflush(stdin);

    int server_num;
    int server_mid, local_mid;
    local_mid = msgget(getpid(), IPC_CREAT | 0644);
    while(1){
        enter_query quer;
        quer.num = getpid();
        quer.type = 12;
        printf("Podaj numer serwera: ");
        scanf("%d", &server_num);
        fflush(stdin);
        server_mid = msgget(server_num, 0);
        msgsnd(server_mid, &quer, 4, 0);
        char *command = malloc(COMM_SIZE);
        message mess;
        strcpy(mess.sender, user_name);
        while(1){
            printf("[%s]$ ", user_name);
            fflush(stdin);
            getline(&command, &COMM_SIZE, stdin);
            command[strcspn(command, "\r\n")] = 0;
            strcpy(mess.text, command);
            strcpy(mess.time, "12:34:45");
            msgsnd(local_mid, &mess, MESSAGE_SIZE, 0);
        }
    }
    return 0;
}
