#include "../lib/client_utils.h"
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

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

