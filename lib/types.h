#ifndef TYPES
#define TYPES
#define MESSAGE_SIZE 297

typedef struct message message;
typedef struct channel channel;
typedef struct user user;
typedef struct enter_query enter_query;

struct message{
    long type;
    char sender[32];
    char text[256];
    char time[9];
};

struct channel{
    char name[16];
    char id;
    message buffer[16];
};

struct user{
    char name[16];
    char id;
    message buffer[16];
};

struct enter_query{
    long type;
    int num;
};
#endif
