#ifndef TYPES
#define TYPES

#define MESSAGE_SIZE 100

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

struct channel{
    char name[16];
    int n_users;
    struct user users[16];
};

#endif
