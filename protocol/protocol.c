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

// -------------  handler functions  -------------
void handle_myauctions(char *response) {
    // Example RMA_OK response: "RMA OK 1 1 2 0 3 1"
    
    // Initialize variables to store auction ID and state
    int AID, state;
    char status[4];

    sscanf(response, "%*s %s", status);

    // Initialize a buffer to store the display string
    char display_buffer[MAX_RESPONSE_SIZE];
    display_buffer[0] = '\0';  // Ensure the buffer is initially empty

    // Iterate through the remaining pairs of AID and state
    while (sscanf(response, "%03d %d", &AID, &state) == 2) {
        // Read the AID
        sprintf(display_buffer + strlen(display_buffer), "%d  %d\n", AID, state);

        // Move to the next pair in the response
        response = strchr(response, ' ');  
        if (response == NULL) {
            break;  // Exit the loop if there are no more pairs
        }
        response++;  // Move past the space
    }

    // Update the response variable with the generated string
    strcpy(response, display_buffer);
}


void handle_mybids(char *response) {
    // Example RMB_OK response: "RMB OK 1 1 2 0 3 1"
    
    // Initialize variables to store auction ID and state
    int AID, state;
    char status[4];

    sscanf(response, "%*s %s", status);

    // Initialize a buffer to store the display string
    char display_buffer[MAX_RESPONSE_SIZE];
    display_buffer[0] = '\0';  // Ensure the buffer is initially empty

    // Iterate through the pairs of AID and state
    while (sscanf(response, "%d %d", &AID, &state) == 2) {
        // Read the AID and state
        sprintf(display_buffer + strlen(display_buffer), "%d  %d\n", AID, state);

        // Move to the next pair in the response
        response = strchr(response, ' ');  
        if (response == NULL) {
            break;  // Exit the loop if there are no more pairs
        }
        response++;  // Move past the space
    }

    // Update the response variable with the generated string
    strcpy(response, display_buffer);
}


void handle_list(char *response) {
    // Example RLS_OK response: "RLS OK 1 1 2 0 3 1"
    
    // Initialize variables to store auction ID and state
    int AID, state;
    char status[4];

    sscanf(response, "%*s %s", status);

    // Initialize a buffer to store the display string
    char display_buffer[MAX_RESPONSE_SIZE];
    display_buffer[0] = '\0';  // Ensure the buffer is initially empty

    // Iterate through the pairs of AID and state
    while (sscanf(response, "%d %d", &AID, &state) == 2) {
        // Read the AID and state
        sprintf(display_buffer + strlen(display_buffer), "%d  %d\n", AID, state);

        // Move to the next pair in the response
        response = strchr(response, ' ');  
        if (response == NULL) {
            break;  // Exit the loop if there are no more pairs
        }
        response++;  // Move past the space
    }

    // Update the response variable with the generated string
    strcpy(response, display_buffer);
}

void handle_show_record(char *response) {
    // Example RRC_OK response:
    // "RRC OK 100 1 ArtAuction artwork.jpg 5000 2023-12-06 12:00:00 3600
    // B 101 6000 2023-12-06 12:05:00 300 E 2023-12-06 13:00:00 3600"
    
    // Initialize variables to store auction details
    int host_UID, bidder_UID, start_value, bid_value, timeactive, bid_sec_time, end_sec_time;
    char auction_name[256], asset_fname[256], start_date_time[20], bid_date_time[20], end_date_time[20];
    char status[4];

    sscanf(response, "%*s %s", status);

    // Read the auction details
    sscanf(response, "%*s %*s %d %d %s %s %d %s %d", &host_UID, &bidder_UID, auction_name, asset_fname, &start_value, start_date_time, &timeactive);

    // Initialize a buffer to store the display string
    char display_buffer[MAX_RESPONSE_SIZE];
    display_buffer[0] = '\0';  // Ensure the buffer is initially empty

    // Add auction details to the display string
    sprintf(display_buffer, "Auction Details:\nHost UID: %d\nAuction Name: %s\nAsset Filename: %s\nStart Value: %d\nStart Date-Time: %s\nTime Active: %d seconds\n\n", host_UID, auction_name, asset_fname, start_value, start_date_time, timeactive);

    // Move to the bids section of the response
    response = strchr(response, 'B');
    if (response == NULL) {
        // No bids present, print the auction details and return
        strcpy(response, display_buffer);
        return;
    }

    // Iterate through the bids
    while (sscanf(response, "B %d %d %s %d", &bidder_UID, &bid_value, bid_date_time, &bid_sec_time) == 4) {
        // Add bid details to the display string
        sprintf(display_buffer + strlen(display_buffer), "Bidder UID: %d\nBid Value: %d\nBid Date-Time: %s\nBid Sec Time: %d seconds\n\n", bidder_UID, bid_value, bid_date_time, bid_sec_time);

        // Move to the next bid in the response
        response = strchr(response, 'B');
        if (response == NULL) {
            break;  // Exit the loop if there are no more bids
        }
    }

    // Check if the auction is closed
    if (sscanf(response, "E %s %d", end_date_time, &end_sec_time) == 2) {
        // Add closing details to the display string
        sprintf(display_buffer + strlen(display_buffer), "Auction Closed:\nEnd Date-Time: %s\nEnd Sec Time: %d seconds\n", end_date_time, end_sec_time);
    }

    strcpy(response, display_buffer);
}

void handle_show_asset(char *buffer){

}

// ----------------------------------------------


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
            strcpy(response, "ERR");
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
            strcpy(response, "ERR");
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
            strcpy(response, "ERR");
        }
        return;
    } else if (strcmp(msg_type, "RMA") == 0) {
        if (strcmp(status, "OK") == 0) {
            handle_myauctions(response);
            return;
        } else if (strcmp(status, "NOK") == 0) {
            strcpy(response, Responses.RMA_NOK());
        } else if (strcmp(status, "NLG") == 0) {
            strcpy(response, Responses.RMA_NLG());
        } else {
            strcpy(response, "ERR");
        }
        return;
    } else if (strcmp(msg_type, "RMB") == 0) {
        if (strcmp(status, "OK") == 0) {
            handle_mybids(response);
            return;
        } else if (strcmp(status, "NOK") == 0) {
            strcpy(response, Responses.RMB_NOK());
        } else if (strcmp(status, "NLG") == 0) {
            strcpy(response, Responses.RMA_NLG());
        } else {
            strcpy(response, "ERR");
        }
        return;
    } else if (strcmp(msg_type, "RLS") == 0) {
        if (strcmp(status, "OK") == 0) {
            handle_list(response);
            return;
        } else if (strcmp(status, "NOK") == 0) {
            strcpy(response, Responses.RLS_NOK());
        } else {
            strcpy(response, "ERR");
        }
        return;
    } else if (strcmp(msg_type, "RRC") == 0) {
        if (strcmp(status, "OK") == 0) {
            handle_show_record(response);
            return;
        } else if (strcmp(status, "NOK") == 0) {
            strcpy(response, Responses.RRC_NOK());
        } else {
            strcpy(response, "ERR");
        }
        return;
    } else {
        strcpy(response, "ERR");
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
            strcpy(response, Responses.ROA_NOK());
        } else if (strcmp(status, "NLG") == 0) {
            strcpy(response, Responses.ROA_NLG());
        } else {
            strcpy(response, "ERR");
        }
        return;
    } else if (strcmp(msg_type, "RCL") == 0) {
        if (strcmp(status, "OK") == 0) {
            strcpy(response, Responses.RCL_OK());
        } else if (strcmp(status, "NOK") == 0) {
            strcpy(response, Responses.RCL_NOK(UID));
        } else if (strcmp(status, "NLG") == 0) {
            strcpy(response, Responses.RCL_NLG());
        } else if (strcmp(status, "EAU") == 0) {
            strcpy(response, Responses.RCL_EAU(AID));
        } else if (strcmp(status, "EOW") == 0) {
            strcpy(response, Responses.RCL_EOW(UID));
        } else if (strcmp(status, "END") == 0) {
            strcpy(response, Responses.RCL_END(AID,UID));
        }
        return;
    } else if (strcmp(msg_type, "RSA") == 0) {
        if (strcmp(status, "NOK") == 0) {
            strcpy(response, Responses.RSA_NOK());
        } 
        return;
    } else if (strcmp(msg_type, "RBD") == 0) {
        if (strcmp(status, "NOK") == 0) {
            strcpy(response, Responses.RBD_NOK(AID));
        } else if (strcmp(status, "NLG") == 0) {
            strcpy(response, Responses.RBD_NLG());
        } else if (strcmp(status, "ACC") == 0) {
            strcpy(response, Responses.RBD_ACC(AID));
        } else if (strcmp(status, "REF") == 0) {
            strcpy(response, Responses.RBD_REF());
        } else if (strcmp(status, "ILG") == 0) {
            strcpy(response, Responses.RBD_ILG());
        }
        return;
    } else {
        strcpy(response, "ERR");
    }
}

int send_tcp_request(char *buffer){
    struct addrinfo hints,*res;
    int fd,n,errcode;
    fd=socket(AF_INET,SOCK_STREAM,0); //TCP socket
    if (fd==-1) exit(1); //error
    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET; //IPv4
    hints.ai_socktype=SOCK_STREAM; //TCP socket
    errcode=getaddrinfo(ASIP,ASport,&hints,&res);
    if(errcode!=0)/*error*/exit(1);
    n=connect(fd,res->ai_addr,res->ai_addrlen);
    if(n==-1)/*error*/exit(1);
    n=write(fd,buffer,strlen(buffer));
    if(n==-1)/*error*/exit(1);
    n=read(fd,buffer,128);
    if(n==-1)/*error*/exit(1);
    printf("Response: %s\n",buffer);
    freeaddrinfo(res);
    close(fd);
    return 0;
}

// ----------------------------------------------