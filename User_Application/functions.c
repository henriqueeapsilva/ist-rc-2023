#include "functions.h"
#include "../protocol/protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <string.h>

bool loggedIn = false; // 1 - logged In (true), 0 - Not logged In (false)
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
    
    if(response == 0){ // updates the client (logged in)
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

    if(loggedIn == 0){
        sprintf(buffer, "user not logged in\n");
        write(1,buffer,strlen(buffer));
        return 1;
    }

    sprintf(buffer, "LOU %d %s\n", client._UID, client._password);

    response = send_udp_request(buffer);

    if(response == 1){ // updates the status of the client
        loggedIn = false;
        return 0;
    }

    return response;

}

int unregister(char *buffer){
    int response;

    if(loggedIn == 0){
        sprintf(buffer, "user not logged in\n");
        write(1,buffer,strlen(buffer));
        return 1;
    }

    sprintf(buffer, "UNR %d %s\n", client._UID, client._password);

    response = send_udp_request(buffer);
     if(response == 1){ //updates the status of the client
        loggedIn = false;
        return 0;
    }

    return response;
}

int exit_(char *buffer){
    if(loggedIn == 1){ // checks if the client is still logged in
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