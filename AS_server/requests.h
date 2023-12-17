#ifndef _REQUESTS_
#define _REQUESTS_


#include "../utils/utils.h"
#include "dir_functions.h"
#include "messages.h"

typedef int (*CommandFunction)();


// Command table entry
typedef struct {
    const char *command;
    CommandFunction function;
} CommandEntry;

void execute_request(int fd, struct sockaddr_in addr,  const char *command, char* buffer);

int login_handler(int fd, struct sockaddr_in addr,  char *buffer);

int logout_handler(int fd, struct sockaddr_in addr,  char *buffer);

int unregister_handler(int fd, struct sockaddr_in addr,  char *buffer);

int open_handler(int fd, struct sockaddr_in addr,  char *buffer);

int close_handler(int fd, struct sockaddr_in addr,  char *buffer);

int myauctions_handler(int fd, struct sockaddr_in addr,  char *buffer);

int mybids_handler(int fd, struct sockaddr_in addr,  char *buffer);

int list_handler(int fd, struct sockaddr_in addr,  char *buffer);

int show_asset_handler(int fd, struct sockaddr_in addr,  char *buffer);

int bid_handler(int fd, struct sockaddr_in addr,  char *buffer);

int show_record_handler(int fd, struct sockaddr_in addr,  char *buffer);


#endif