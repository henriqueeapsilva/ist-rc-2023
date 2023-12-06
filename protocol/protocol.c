#include "../User_Application/functions.h"
#include "messages.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <string.h>


// ---------------- UDP Functions ----------------

void analyse_udp_response(char *response, int *loggedIn) {
    // Extract the first three letters to determine the message type
    char msg_type[4];
    strncpy(msg_type, response, 3);
    msg_type[3] = '\0';

    // Initialize status and message variables
    char status[4];

    sscanf(response, "%*s %s", status);

    // Define status messages for each message type
    if (strcmp(msg_type, "RLI") == 0) {
        // RLI status messages
        if (strcmp(status, "OK") == 0) {strcpy(response, RLI_OK); *loggedIn = 0;}
        else if (strcmp(status, "NOK") == 0) strcpy(response, RLI_NOK);
        else if (strcmp(status, "REG") == 0) {strcpy(response, RLI_REG); *loggedIn = 0;}
        else strcpy(response, "Unknown Status");
        return;

    } else if (strcmp(msg_type, "RLO") == 0) {
        // RLO status messages
        if (strcmp(status, "OK") == 0) {strcpy(response, RLO_OK); *loggedIn = 1;}
        else if (strcmp(status, "NOK") == 0) strcpy(response, RLO_NOK);
        else if (strcmp(status, "UNR") == 0) strcpy(response, RLO_UNR);
        else strcpy(response, "Unknown Status");
        return;

    } else if (strcmp(msg_type, "RUR") == 0) {
        // RUR status messages
        if (strcmp(status, "OK") == 0) { strcpy(response, RUR_OK); *loggedIn = 1;}
        else if (strcmp(status, "NOK") == 0) strcpy(response, RUR_NOK);
        else if (strcmp(status, "UNR") == 0) strcpy(response, RUR_UNR);
        else strcpy(response, "Unknown Status");
        return;

    } else if (strcmp(msg_type, "RMA") == 0) {
        // RMA status messages
        if (strcmp(status, "OK") == 0) return; // receives the list of auctions and add to the response
        else if (strcmp(status, "NOK") == 0) strcpy(response, RMA_NOK);
        else if (strcmp(status, "NLG") == 0) strcpy(response, RMA_NLG);
        else strcpy(response, "Unknown Status");
        return;

    } else if (strcmp(msg_type, "RMB") == 0) {
        // RMB status messages
        if (strcmp(status, "OK") == 0) return; // receives the list of bids and add to the response
        else if (strcmp(status, "NOK") == 0) strcpy(response, RMB_NOK);
        else if (strcmp(status, "NLG") == 0) strcpy(response, RMA_NLG);
        else strcpy(response, "Unknown Status");
        return;

    } else if (strcmp(msg_type, "RLS") == 0) {
        // RLS status messages
        if (strcmp(status, "OK") == 0) return; // receives the list of auctions and add to the response
        else if (strcmp(status, "NOK") == 0) strcpy(response, RLS_NOK);
        else strcpy(response, "Unknown Status");
        return;

    } else if (strcmp(msg_type, "RRC") == 0) {
        // RRC status messages
        if (strcmp(status, "OK") == 0) return; // receives the stats and add to the response
        else if (strcmp(status, "NOK") == 0) strcpy(response, RRC_NOK);
        else strcpy(response, "Unknown Status");
        return;
    } else {
        strcpy(response, "Unknown Message Type");
        return;
    }

}


int send_udp_request(char *buffer){
    int fd, errcode, loggedIn = 1;
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

    analyse_udp_response(buffer, &loggedIn);

    write(1,buffer,strlen(buffer));

    freeaddrinfo(res);

    close(fd);

    return loggedIn;
}

// ----------------------------------------------

// ---------------- TCP Functions ----------------

void analyse_tcp_response(char *response, int *loggedIn) {
    // Extract the first three letters to determine the message type
    char msg_type[4];
    strncpy(msg_type, response, 3);
    msg_type[3] = '\0';

    // Initialize status and message variables
    char status[4];

    sscanf(response, "%*s %s", status);

    // Define status messages for each message type
    if (strcmp(msg_type, "ROA") == 0) {
        // ROA status messages
        if (strcmp(status, "OK") == 0) return; // does the open operations;
        else if (strcmp(status, "NOK") == 0) strcpy(response, ROA_NOK);
        else if (strcmp(status, "NLG") == 0) strcpy(response, ROA_NLG);
        else strcpy(response, "Unknown Status");
        return;

    } else if (strcmp(msg_type, "RCL") == 0) {
        // RCL status messages
        if (strcmp(status, "OK") == 0) strcpy(response, RCL_OK);
        else if (strcmp(status, "NOK") == 0) strcpy(response, RCL_NOK);
        else if (strcmp(status, "NLG") == 0) strcpy(response, RCL_NLG);
        else if (strcmp(status, "EAU") == 0) strcpy(response, RCL_EAU);
        else if (strcmp(status, "EOW") == 0) strcpy(response, RCL_EOW);
        else if (strcmp(status, "END") == 0) strcpy(response, RCL_END);
        else strcpy(response, "Unknown Status");
        return;

    } else if (strcmp(msg_type, "RSA") == 0) {
        // RSA status messages
        if (strcmp(status, "OK") == 0) return; //shows asset;
        else if (strcmp(status, "NOK") == 0) strcpy(response, RSA_NOK);
        else strcpy(response, "Unknown Status");
        return;

    } else if (strcmp(msg_type, "RBD") == 0) {
        // RBD status messages
        if (strcmp(status, "NOK") == 0) strcpy(response, RBD_NOK);
        else if (strcmp(status, "NLG") == 0) strcpy(response, RBD_NLG);
        else if (strcmp(status, "ACC") == 0) strcpy(response, RBD_ACC);
        else if (strcmp(status, "REF") == 0) strcpy(response, RBD_REF);
        else if (strcmp(status, "ILG") == 0) strcpy(response, RBD_ILG);
        else strcpy(response, "Unknown Status");
        return;

    } else {
        strcpy(response, "Unknown Message Type");
        return;
    }
}



int send_tcp_request(char *request){
    return 0;
}

// ----------------------------------------------