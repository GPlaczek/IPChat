#ifndef COMMAND
#define COMMAND

typedef struct command command;

struct command{
    char *command;
    char *args[];
};

command parse_command(char *str, int len);
#endif
