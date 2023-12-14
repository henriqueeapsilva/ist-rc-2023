#include "functions.h"

bool loggedIn = false; // 1 - logged In (true), 0 - Not logged In (false)
User client;


// ---- User command functions ---

int login(char *buffer){
    char UID[7];
    int response;
    char password[9];  // 8 characters for the password + 1 for null-terminator

    if(loggedIn){
        printf("Already logged in on another user, please logout first!\n");
        return 1;
    }


    // Parse UID and password from the buffer
    if (sscanf(buffer, "%*s %s %8s", UID, password) != 2) {
        printf("Invalid input format\n");
        return 1;
    }

    // Check UID and password formats
    if (!is_valid_uid(UID)) {
        printf("Invalid UID format\n");
        return 1;
    }

    if (!is_valid_password(password)) {
        printf("Invalid password format\n");
        return 1;
    }

    sprintf(buffer, "LIN %s %s\n", UID, password);
    
    response = send_udp_request(buffer);
    
    if(response == 0){ // updates the client (logged in)
        strcpy(client._UID, UID);
        strcpy(client._password, password);
        loggedIn = true;
    }

    return response;
}

int open_(char *buffer){

    int response, start_value, timeactive, fsize;
    char name[50], asset_fname[200];

    if (sscanf(buffer, "%*s %s %s %d %d", name, asset_fname, &start_value, &timeactive) != 4) {
        printf("Invalid input format\n");
        return 0;
    }

    FILE *file = fopen(asset_fname, "r");
    
    if (file == NULL) {
        printf("Invalid file name\n");
        return 0;
    }

    // get file size
    fseek(file, 0L, SEEK_END);
    fsize = ftell(file);
    fseek(file, 0L, SEEK_SET);

    sprintf(buffer, "OPA %s %s %s %d %d %s %d ", client._UID, client._password, name, start_value, timeactive, asset_fname, fsize);

    response=send_tcp_request(buffer, 1);
    fclose(file);
    return response;
}

int close_(char *buffer){
    int response, AID;
    if (sscanf(buffer, "%*s %d", &AID) != 1) {
        printf("Invalid input format\n");
        return 0;
    }
    sprintf(buffer, "CLS %s %s %d\n", client._UID, client._password, AID);
    response = send_tcp_request(buffer,0);
    return response;
}

int myauctions(char *buffer) {
    int response;

    if (loggedIn == 0) {
        printf("user not logged in\n\n");
        return 0;
    }

    sprintf(buffer, "LMA %s\n", client._UID);

    response = send_udp_request(buffer);

    return response;
}

int mybids(char *buffer) {
    int response;

    if (loggedIn == 0) {
        printf("user not logged in\n\n");
        return 0;
    }
    

    sprintf(buffer, "LMB %s\n", client._UID);

    response = send_udp_request(buffer);

    return response;
}

int list(char *buffer) {
    int response;

    sprintf(buffer, "LST\n");

    response = send_udp_request(buffer);

    return response;
}

int show_asset(char *buffer){
    int response;
    char AID[4];

    if (sscanf(buffer, "%*s %s", AID) != 1) {
        printf("Invalid input format\n");
        return 0;
    }
    sprintf(buffer, "SAS %s\n", AID);
    response = send_tcp_request(buffer,2);
    return response;
}

int bid(char *buffer){
    char AID[4];
    int value, response;

    if (loggedIn == 0) {
        printf("user not logged in\n\n");
        return 0;
    }

    if (sscanf(buffer, "%*s %s %d", AID, &value) != 2) {
        printf("Invalid input format\n");
        return 0;
    }
    sprintf(buffer, "BID %s %s %s %d\n", client._UID, client._password, AID, value);

    response = send_tcp_request(buffer,0);

    return response;
}

int show_record(char *buffer) {
    int response;
    char AID[4];

    // Parse AID from the buffer
    if (sscanf(buffer, "%*s %s", AID) != 1) {
        printf("Invalid input format\n");
        return 0;
    }

    // Create the protocol message
    sprintf(buffer, "SRC %s\n", AID);

    // Send UDP request
    response = send_udp_request(buffer);

    return response;
}


int logout(char *buffer){
    int response;

    if(loggedIn == 0){
        sprintf(buffer, "user not logged in\n");
        write(1,buffer,strlen(buffer));
        return 1;
    }

    sprintf(buffer, "LOU %s %s\n", client._UID, client._password);

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

    sprintf(buffer, "UNR %s %s\n", client._UID, client._password);

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
void executeCommand(const char *command, char* buffer) {
    for (int i = 0; commandTable[i].command != NULL; i++) {
        if (strcmp(commandTable[i].command, command) == 0) {
            commandTable[i].function(buffer);
            return;
        }
    }
    printf("Invalid command\n");
    return ;
}