#ifndef _UTILS_
#define _UTILS_

#define MAX_NAME_D 6
#define MAX_FILENAME 24
#define MAX_START_VALUE 6
#define MAX_AUCTION_DURATION 5
#define MAX_BUFFER_SIZE 6010
#define DIR_USER "../ASDIR/USERS" 
#define DIR_AUCTION "../ASDIR/AUCTIONS"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <stddef.h>
#include <dirent.h>
#include <sys/stat.h>

extern char *asip;
extern char *asport;

int is_valid_uid(char *uid);
int is_valid_password(const char *password);
int is_valid_aid(char *aid);


#endif