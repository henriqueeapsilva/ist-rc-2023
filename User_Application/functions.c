#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>



// ------- check functions -------

int is_valid_uid(const int uid) {
    // Check if UID has 6 digits
    return (uid >= 100000 && uid <= 999999);
}

int is_valid_password(const char *password) {
    // Check if password has 8 alphanumerics (only digits and letters)
    for (int i = 0; i < 8; ++i) {
        if (!isalnum(password[i])) {
            return 0;
        }
    }
    return 1;
}

// -------------------------------

// ---- UDP and TCP functions ----
int send_udp_request(char *request){
    int fd, errcode;
    ssize_t n;
    socketlen_t addrlen;
    struct addrinfo hints, *res;
    struct sockaddr_in addr;
    char buffer[MAX_RESPONSE_SIZE];

    fd=socket(AF_INET,SOCK_DGRAM,0);
    if(fd==-1)return 1;

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;/
    hints.ai_socktype=SOCK_DGRAM;
    
    errcode=getaddrinfo(ASIP,ASport,&hints,&res);
    if(errcode!=0) return 1;
    
    n=sendto(fd,request,sizeof(request),0,res->ai_addr,res->ai_addrlen);
    if(n==-1) return 1;


    addrlen=sizeof(addr);
    n=recvfrom(fd,buffer,MAX_RESPONSE_SIZE,0,(struct sockaddr*)&addr,&addrlen);
    if(n==-1)return 1;

    write(1,buffer,n);
    close(fd);

    freeaddrinfo(res);

    return 0;
}

int send_tcp_request(char *request){
    return 0;
}

// -------------------------------

// ---- User command functions ---

int login(char *buffer){
    int UID, response;
    char password[9];  // 8 characters for the password + 1 for null-terminator
    char *message;

    // Parse UID and password from the buffer
    if (sscanf(buffer, "%*s %d %8s", &UID, password) != 2) {
        printf("Invalid input format\n");
        return 0;
    }

    // Check UID and password formats
    if (!is_valid_uid(UID)) {
        printf("Invalid UID format\n");
        return 0;
    }

    if (!is_valid_password(password)) {
        printf("Invalid password format\n");
        return 0;
    }

    // Create the message in the specified format
    sprintf(message, "LIN %d %s\n", UID, password);

    int response = send_udp_request(message);

    return response;
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

// -------------------------------



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