#ifndef _FUNTIONS_H_
#define _FUNCTION_H_

#include "../utils/utils.h"
#include "protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <string.h>
#include <sys/sendfile.h>
#include <stdbool.h>

typedef struct user
{
    int _UID;
    char _password[9];
}User;

typedef int (*CommandFunction)();

// Command table entry
typedef struct {
    const char *command;
    CommandFunction function;
} CommandEntry;

void executeCommand(const char *command, char* buffer);

int login(char *buffer);

int open_(char *buffer);

int close_(char *buffer);

int myauctions(char *buffer);

int mybids(char *buffer);

int list(char *buffer);

int show_asset(char *buffer);

int bid(char *buffer);

int show_record(char *buffer);

int logout(char *buffer);

int unregister(char *buffer);

int exit_(char *buffer);



#endif
