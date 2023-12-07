#include "../User_Application/functions.h"
#include "protocol.h"
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

int UID = 0;
int AID = 0;

// ---------------- UDP Functions ----------------

void analyse_udp_response(char *response, int *loggedIn) {
    // Extract the first three letters to determine the message type
    char msg_type[4];
    strncpy(msg_type, response, 3);
    msg_type[3] = '\0';

    // Initialize status and message variables
    char status[4];

    sscanf(response, "%*s %s", status);
    
    // Use the ResponseMessages structure to get the appropriate status message
    if (strcmp(msg_type, "RLI") == 0) {
        if (strcmp(status, "OK") == 0) {
            strcpy(response, Responses.RLI_OK());
            *loggedIn = 0;
        } else if (strcmp(status, "NOK") == 0) {
            strcpy(response, Responses.RLI_NOK());
        } else if (strcmp(status, "REG") == 0) {
            strcpy(response, Responses.RLI_REG());
            *loggedIn = 0;
        } else {
            strcpy(response, "Unknown Status");
        }
        return;
    } else if (strcmp(msg_type, "RLO") == 0) {
        if (strcmp(status, "OK") == 0) {
            strcpy(response, Responses.RLO_OK());
            *loggedIn = 1;
        } else if (strcmp(status, "NOK") == 0) {
            strcpy(response, Responses.RLO_NOK());
        } else if (strcmp(status, "UNR") == 0) {
            strcpy(response, Responses.RLO_UNR());
        } else {
            strcpy(response, "Unknown Status");
        }
        return;
    } else if (strcmp(msg_type, "RUR") == 0) {
        if (strcmp(status, "OK") == 0) {
            strcpy(response, Responses.RUR_OK());
            *loggedIn = 1;
        } else if (strcmp(status, "NOK") == 0) {
            strcpy(response, Responses.RUR_NOK());
        } else if (strcmp(status, "UNR") == 0) {
            strcpy(response, Responses.RUR_UNR());
        } else {
            strcpy(response, "Unknown Status");
        }
        return;
    } else if (strcmp(msg_type, "RMA") == 0) {
        if (strcmp(status, "OK") == 0) {
            // Handle the case where the response should be added to the list of auctions
            // For now, just return
            return;
        } else if (strcmp(status, "NOK") == 0) {
            strcpy(response, Responses.RMA_NOK());
        } else if (strcmp(status, "NLG") == 0) {
            strcpy(response, Responses.RMA_NLG());
        } else {
            strcpy(response, "Unknown Status");
        }
        return;
    } else if (strcmp(msg_type, "RMB") == 0) {
        if (strcmp(status, "OK") == 0) {
            // Handle the case where the response should be added to the list of bids
            // For now, just return
            return;
        } else if (strcmp(status, "NOK") == 0) {
            strcpy(response, Responses.RMB_NOK());
        } else if (strcmp(status, "NLG") == 0) {
            strcpy(response, Responses.RMA_NLG());
        } else {
            strcpy(response, "Unknown Status");
        }
        return;
    } else if (strcmp(msg_type, "RLS") == 0) {
        if (strcmp(status, "OK") == 0) {
            // Handle the case where the response should be added to the list of auctions
            // For now, just return
            return;
        } else if (strcmp(status, "NOK") == 0) {
            strcpy(response, Responses.RLS_NOK());
        } else {
            strcpy(response, "Unknown Status");
        }
        return;
    } else if (strcmp(msg_type, "RRC") == 0) {
        if (strcmp(status, "OK") == 0) {
            // Handle the case where the response should be added to the user's stats
            // For now, just return
            return;
        } else if (strcmp(status, "NOK") == 0) {
            strcpy(response, Responses.RRC_NOK());
        } else {
            strcpy(response, "Unknown Status");
        }
        return;
    } else {
        strcpy(response, "Unknown Message Type");
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

    // Use the ResponseMessages structure to get the appropriate status message
    if (strcmp(msg_type, "ROA") == 0) {
        if (strcmp(status, "NOK") == 0) {
            printf("%s\n", Responses.ROA_NOK());
        } else if (strcmp(status, "NLG") == 0) {
            printf("%s\n", Responses.ROA_NLG());
        }
        return;
    } else if (strcmp(msg_type, "RCL") == 0) {
        if (strcmp(status, "OK") == 0) {
            printf("%s\n", Responses.RCL_OK());
        } else if (strcmp(status, "NOK") == 0) {
            printf("%s\n", Responses.RCL_NOK(UID));
        } else if (strcmp(status, "NLG") == 0) {
            printf("%s\n", Responses.RCL_NLG());
        } else if (strcmp(status, "EAU") == 0) {
            printf("%s\n", Responses.RCL_EAU(AID));
        } else if (strcmp(status, "EOW") == 0) {
            printf("%s\n", Responses.RCL_EOW(UID));
        } else if (strcmp(status, "END") == 0) {
            printf("%s\n", Responses.RCL_END(AID,UID));
        }
        return;
    } else if (strcmp(msg_type, "SAS") == 0) {
        // Handle SAS messages (not specified in the original responses, adjust accordingly)
        return;
    } else if (strcmp(msg_type, "RSA") == 0) {
        if (strcmp(status, "NOK") == 0) {
            printf("%s\n", Responses.RSA_NOK());
        } else {
            // Handle RSA messages (not specified in the original responses, adjust accordingly)
        }
        return;
    } else if (strcmp(msg_type, "BID") == 0) {
        // Handle BID messages (not specified in the original responses, adjust accordingly)
        return;
    } else if (strcmp(msg_type, "RBD") == 0) {
        if (strcmp(status, "NOK") == 0) {
            printf("%s\n", Responses.RBD_NOK(AID));
        } else if (strcmp(status, "NLG") == 0) {
            printf("%s\n", Responses.RBD_NLG());
        } else if (strcmp(status, "ACC") == 0) {
            printf("%s\n", Responses.RBD_ACC(AID));
        } else if (strcmp(status, "REF") == 0) {
            printf("%s\n", Responses.RBD_REF());
        } else if (strcmp(status, "ILG") == 0) {
            printf("%s\n", Responses.RBD_ILG());
        }
        return;
    } else {
        printf("Unknown Message Type\n");
    }
}



int send_tcp_request(char *request){ // check the UID and AID if necessary and save it, then put it to 0 if not necessary
    return 0;
}

// ----------------------------------------------

