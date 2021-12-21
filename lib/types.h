#ifndef TYPES
#define TYPES

#define MESSAGE_SIZE 96

struct query{
    long type;
    int num;
    char name[16];
    char time[16];
    char text[64];
};

struct user{
    int pid;
    char name[16];
};

#endif
