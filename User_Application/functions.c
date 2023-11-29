#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>


int login(char *buffer){
    return 0;
}

int open_(char *buffer){
    return 0;
}

int close_(char *buffer){
    return 0;
}

int myauctions(char *buffer){
    return 0;
}

int mybids(char *buffer){
    return 0;
}

int list(char *buffer){
    return 0;
}

int show_asset(char *buffer){
    return 0;
}

int bid(char *buffer){
    return 0;
}

int show_record(char *buffer){
    return 0;
}

int logout(char *buffer){
    return 0;
}

int unsubscribe(char *buffer){
    return 0;
}

int exit_(char *buffer){
    return 0;
}

// Command table
CommandEntry commandTable[] = {
    {"login", login},
    {"open", open_},
    {"close", close_},
    {"myauctions", myauctions},
    {"ma", myauctions},
    {"mybids", mybids},
    {"mb", mybids},
    {"list", list},
    {"l", list},
    {"show_asset", show_asset},
    {"sa", show_asset},
    {"bid", bid},
    {"b", bid},
    {"show_record", show_record},
    {"sr", show_record},
    {"logout", logout},
    {"unsubscribe", unsubscribe},
    {"exit", exit_},
    {NULL, NULL} // End of table marker
};

// Function to execute a command
int executeCommand(const char *command, char* buffer) {
    for (int i = 0; commandTable[i].command != NULL; i++) {
        if (strcmp(commandTable[i].command, command) == 0) {
            return commandTable[i].function(buffer);
        }
    }
    printf("Invalid command\n");
    return 1;
}