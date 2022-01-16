#include "../lib/client_utils.h"
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

char *gettime(){
    char *buf = malloc(16);
    struct tm *info;
    struct timeval tv;
    time_t t;
    gettimeofday(&tv, NULL);
    t = tv.tv_sec;
    info = localtime(&t);
    strftime(buf, 16, "%H:%M:%S", info);
    return buf;
}

void instructions(){
    printf("\n---------------------------------------------------------------------------\n");
    printf("INSTRUKCJA OBSŁUGI\n");
    printf("Komunikacja na czacie odbywa się poprzez wpisywanie następujących komend: \n");
    printf("%d <nazwa kanału> -> opuszczanie kanału\n", (int)EXIT_CHANNEL);
    printf("%d <nazwa kanału> -> utworzenie/dołączenie do kanału\n", (int)CHANNEL);
    printf("%d -> wyświetlanie listy zalogowanych użytkowników\n", (int)LIST_USERS);
    printf("%d -> wyświetlanie listy kanałów wraz z zalogowanymi użytkownikami\n", (int)LIST_CHANNELS);
    printf("<id użytkownika> <wiadomość> -> wysłanie wiadomości prywatnej\n");
    printf("<id kanału> <wiadomość> -> wysłanie wiadomości publicznej w obrębie kanału\n");
    printf("%d -> czyszczenie ekranu\n", (int)CLEAR);
    printf("%d -> pomoc\n", (int)HELP);
    printf("%d -> wyjście z czatu\n", (int)EXIT);
    printf("%d + <id kanału> -> wyświetl bufor wiadomości danego kanału\n", 240);
    printf("---------------------------------------------------------------------------\n\n");
}

void bold_text(){printf("\033[32;1m");}

void reset_text(){
    printf("\x1B[0m");
    fflush(stdout);
}
