#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <string.h>

bool loggedIn = false; // 0 - logged In, 1 - Not logged In
User client;


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
void analyse_response_udp(char *buffer){


}




int send_udp_request(char *buffer){
    int fd, errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints, *res;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_DGRAM, 0); 
    if (fd == -1) exit(1);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; 
    hints.ai_socktype = SOCK_DGRAM; 
    

    errcode=getaddrinfo(ASIP,ASport,&hints,&res);
    if(errcode!=0) exit(1);

    n=sendto(fd,buffer,strlen(buffer),0,res->ai_addr,res->ai_addrlen);
    if(n==-1) exit(1);

    addrlen = sizeof(addr);

    n=recvfrom(fd,buffer,MAX_RESPONSE_SIZE,0,(struct sockaddr*)&addr,&addrlen);

    if(n==-1) exit(1);
    buffer[n] = '\0';

    write(1,buffer,strlen(buffer));

    // analyse udp response
    // updates loggedIn variable
    
    freeaddrinfo(res);

    close(fd);

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

    sprintf(buffer, "LIN %d %s\n", UID, password);
    
    response = send_udp_request(buffer);
    
    printf("1\n");

    if(response == 0){
        client._UID = UID;
        strcpy(client._password, password);
        loggedIn = true;
    }
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
    int response;

    if(!loggedIn){
        sprintf(buffer, "No client logged in.\n");
        write(1,buffer,strlen(buffer));
        return 1;
    }

    sprintf(buffer, "LOU %d %s\n", client._UID, client._password);

    response = send_udp_request(buffer);

    if(response == 0){
        loggedIn = false;
    }

    return response;

}

int unregister(char *buffer){
    int response;

    if(!loggedIn){
        sprintf(buffer, "No client logged in.\n");
        write(1,buffer,strlen(buffer));
        return 1;
    }

    sprintf(buffer, "UNR %d %s\n", client._UID, client._password);

    response = send_udp_request(buffer);

    return response;
}

int exit_(char *buffer){
    if(loggedIn){
        sprintf(buffer, "Please, logout first.\n");
        write(1,buffer,strlen(buffer));
        return 1;
    } 

    exit(EXIT_SUCCESS);
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
    {"unregister", unregister},
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