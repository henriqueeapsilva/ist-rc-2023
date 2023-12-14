#include "protocol.h"

int UID = 0;
char AID[3];

// -------------  handler functions  -------------
char *handle_myauctions(char *response)
{

    int state;
    char AID[4];

    response += strlen("RMA OK ");

    // Initialize a buffer to store the display string
    char *display_buffer = (char *)malloc(MAX_BUFFER_SIZE);
    // Check if malloc was successful
    if (display_buffer == NULL)
    {
        exit(1);
    }

    display_buffer[0] = '\0'; // Ensure the buffer is initially empty

    sprintf(display_buffer + strlen(display_buffer), "MY AUCTIONS:\n");

    // Iterate through the pairs of AID and state
    while (sscanf(response, "%s %d", AID, &state) == 2)
    {

        // Read the AID and state
        sprintf(display_buffer + strlen(display_buffer), "AID: %3s state: %s\n", AID, (state == 1) ? "active" : "inactive");

        response += strlen(" %s %d");
    }
   
    free(display_buffer);

    return display_buffer;
}

char *handle_mybids(char *response)
{

    int state;
    char AID[4];

    response += strlen("RMB OK ");

    // Initialize a buffer to store the display string
    char *display_buffer = (char *)malloc(MAX_BUFFER_SIZE);
    // Check if malloc was successful
    if (display_buffer == NULL)
    {
        exit(1);
    }

    display_buffer[0] = '\0'; // Ensure the buffer is initially empty

    sprintf(display_buffer + strlen(display_buffer), "MY BIDS:\n");

    // Iterate through the pairs of AID and state
    while (sscanf(response, "%s %d", AID, &state) == 2)
    {

        // Read the AID and state
        sprintf(display_buffer + strlen(display_buffer), "AID: %3s state: %s\n", AID, (state == 1) ? "active" : "inactive");

        response += strlen(" %s %d");
    }
    free(display_buffer);

    return display_buffer;
}

char *handle_list(char *response)
{
    // Example RLS_OK response: "RLS OK 050 1 201 0 312 1"

    // Initialize variables to store auction ID and state

    int state;
    char AID[4];

    response += strlen("RLS OK ");

    // Initialize a buffer to store the display string
    char *display_buffer = (char *)malloc(MAX_BUFFER_SIZE);
    // Check if malloc was successful
    if (display_buffer == NULL)
    {
        exit(1);
    }

    display_buffer[0] = '\0'; // Ensure the buffer is initially empty

    sprintf(display_buffer + strlen(display_buffer), "LIST ACTIVE AUCTIONS:\n");

    // Iterate through the pairs of AID and state
    while (sscanf(response, "%s %d", AID, &state) == 2)
    {

        sprintf(display_buffer + strlen(display_buffer), "AID: %3s state: %s\n", AID, (state == 1) ? "active" : "inactive");

        response += strlen(" %s %d");
    }

    free(display_buffer);

    return display_buffer;
}

char *handle_show_record(char *response)
{
    // Example RRC_OK response:
    // "RRC OK 100 1 ArtAuction artwork.jpg 5000 2023-12-06 12:00:00 3600
    // B 101 6000 2023-12-06 12:05:00 300 E 2023-12-06 13:00:00 3600"

    // Initialize variables to store auction details
    char host_UID[MAX_NAME_D+1] = "";
    char auction_name[MAX_NAME_D+1] = "";
    char asset_fname[MAX_FILENAME+1] = "";
    char start_value[MAX_START_VALUE+1] = "";
    char start_date[10+1] = "";
    char start_time[8+1] = "";
    char timeactive[MAX_AUCTION_DURATION+1] = "";

    response += strlen("RRC OK ");
    
    // Initialize a buffer to store the display string
    char *display_buffer = (char *)malloc(MAX_BUFFER_SIZE);
    // Check if malloc was successful
    if (display_buffer == NULL)
    {
        exit(1);
    }

    display_buffer[0] = '\0'; // Ensure the buffer is initially empty
  
    // Read the auction details
    sscanf(response, "%s %s %s %s %s %s %s", host_UID, auction_name, asset_fname, start_value, start_date, start_time, timeactive);

    sprintf(display_buffer, "--- AUCTION: %s ---\n│Auction Host: %s|\n│Opened in: %s %s|\n│Duration: %s seconds|\n│Asset Name: %s|\n│Starting Bid: %s|\n", 
        auction_name, host_UID, start_date, start_time, timeactive, asset_fname, start_value);

    char *token = strtok(response, " ");

        while (token != NULL) {
            if (strcmp(token, "B") == 0) {
                strcat(display_buffer, "---\n");
                strcat(display_buffer, "│Bidder Name: "); 
                token = strtok(NULL, " ");
                strcat(display_buffer, token);
                strcat(display_buffer, "\n│Bid Value: ");
                token = strtok(NULL, " ");
                strcat(display_buffer, token);
                strcat(display_buffer, "\n│Date: ");
                token = strtok(NULL, " ");
                strcat(display_buffer, token);
                token = strtok(NULL, " ");
                strcat(display_buffer, token);
                strcat(display_buffer, "\n");
            }
            else if (strcmp(token, "E") == 0) {
                strcat(display_buffer, "---\n│Ended in: ");
                token = strtok(NULL, " ");
                strcat(display_buffer, token);
                token = strtok(NULL, " ");
                strcat(display_buffer, " ");
                strcat(display_buffer, token);
                strcat(display_buffer, "\n---\n");
            }
            token = strtok(NULL, " ");
        }

    free(display_buffer);

    return display_buffer;
}

// ----------------------------------------------

// ---------------- UDP Functions ----------------

void analyse_udp_response(char *response, int *loggedIn)
{
    // Extract the first three letters to determine the message type
    char msg_type[4];
    strncpy(msg_type, response, 3);
    msg_type[3] = '\0';

    // Initialize status and message variables
    char status[4];

    sscanf(response, "%*s %s", status);

    // Use the ResponseMessages structure to get the appropriate status message
    if (strcmp(msg_type, "RLI") == 0)
    {
        if (strcmp(status, "OK") == 0)
        {
            strcpy(response, Responses.RLI_OK());
            *loggedIn = 0;
        }
        else if (strcmp(status, "NOK") == 0)
        {
            strcpy(response, Responses.RLI_NOK());
        }
        else if (strcmp(status, "REG") == 0)
        {
            strcpy(response, Responses.RLI_REG());
            *loggedIn = 0;
        }
        else
        {
            strcpy(response, "ERR");
        }
        return;
    }
    else if (strcmp(msg_type, "RLO") == 0)
    {
        if (strcmp(status, "OK") == 0)
        {
            strcpy(response, Responses.RLO_OK());
            *loggedIn = 1;
        }
        else if (strcmp(status, "NOK") == 0)
        {
            strcpy(response, Responses.RLO_NOK());
        }
        else if (strcmp(status, "UNR") == 0)
        {
            strcpy(response, Responses.RLO_UNR());
        }
        else
        {
            strcpy(response, "ERR");
        }
        return;
    }
    else if (strcmp(msg_type, "RUR") == 0)
    {
        if (strcmp(status, "OK") == 0)
        {
            strcpy(response, Responses.RUR_OK());
            *loggedIn = 1;
        }
        else if (strcmp(status, "NOK") == 0)
        {
            strcpy(response, Responses.RUR_NOK());
        }
        else if (strcmp(status, "UNR") == 0)
        {
            strcpy(response, Responses.RUR_UNR());
        }
        else
        {
            strcpy(response, "ERR");
        }
        return;
    }
    else if (strcmp(msg_type, "RMA") == 0)
    {
        if (strcmp(status, "OK") == 0)
        {
            strcpy(response, handle_myauctions(response));
            return;
        }
        else if (strcmp(status, "NOK") == 0)
        {
            strcpy(response, Responses.RMA_NOK());
        }
        else if (strcmp(status, "NLG") == 0)
        {
            strcpy(response, Responses.RMA_NLG());
        }
        else
        {
            strcpy(response, "ERR");
        }
        return;
    }
    else if (strcmp(msg_type, "RMB") == 0)
    {
        if (strcmp(status, "OK") == 0)
        {
            strcpy(response, handle_mybids(response));
            return;
        }
        else if (strcmp(status, "NOK") == 0)
        {
            strcpy(response, Responses.RMB_NOK());
        }
        else if (strcmp(status, "NLG") == 0)
        {
            strcpy(response, Responses.RMA_NLG());
        }
        else
        {
            strcpy(response, "ERR");
        }
        return;
    }
    else if (strcmp(msg_type, "RLS") == 0)
    {
        if (strcmp(status, "OK") == 0)
        {
            strcpy(response, handle_list(response));
            return;
        }
        else if (strcmp(status, "NOK") == 0)
        {
            strcpy(response, Responses.RLS_NOK());
        }
        else
        {
            strcpy(response, "ERR");
        }
        return;
    }
    else if (strcmp(msg_type, "RRC") == 0)
    {
        if (strcmp(status, "OK") == 0)
        {
            strcpy(response, handle_show_record(response));
            return;
        }
        else if (strcmp(status, "NOK") == 0)
        {
            strcpy(response, Responses.RRC_NOK());
        }
        else
        {
            strcpy(response, "ERR");
        }
        return;
    }
    else
    {
        strcpy(response, "ERR");
    }
}

int send_udp_request(char *buffer)
{
    int fd, errcode, loggedIn = 1;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints, *res;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
        exit(1);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    errcode = getaddrinfo(asip, asport, &hints, &res);
    if (errcode != 0)
        exit(1);

    n = sendto(fd, buffer, strlen(buffer), 0, res->ai_addr, res->ai_addrlen);
    if (n == -1)
        exit(1);

    addrlen = sizeof(addr);

    n = recvfrom(fd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&addr, &addrlen);

    if (n == -1)
        exit(1);
    buffer[n] = '\0';

    analyse_udp_response(buffer, &loggedIn);

    write(1, buffer, strlen(buffer));

    freeaddrinfo(res);

    close(fd);

    return loggedIn;
}

// ----------------------------------------------

// ---------------- TCP Functions ----------------

void analyse_tcp_response(char *response)
{
    // Extract the first three letters to determine the message type
    char msg_type[4];
    strncpy(msg_type, response, 3);
    msg_type[3] = '\0';

    // Initialize status and message variables
    char status[4];

    sscanf(response, "%*s %s", status);

    // Use the ResponseMessages structure to get the appropriate status message
    if (strcmp(msg_type, "ROA") == 0)
    {
        if (strcmp(status, "OK") == 0)
        {
            sscanf(response, "%*s %*s %s", AID);
            strcpy(response, Responses.ROA_OK(AID));
        }
        else if (strcmp(status, "NOK") == 0)
        {
            strcpy(response, Responses.ROA_NOK());
        }
        else if (strcmp(status, "NLG") == 0)
        {
            strcpy(response, Responses.ROA_NLG());
        }
        else
        {
            strcpy(response, "ERR");
        }
        return;
    }
    else if (strcmp(msg_type, "RCL") == 0)
    {
        if (strcmp(status, "OK") == 0)
        {
            strcpy(response, Responses.RCL_OK());
        }
        else if (strcmp(status, "NOK") == 0)
        {
            strcpy(response, Responses.RCL_NOK(UID));
        }
        else if (strcmp(status, "NLG") == 0)
        {
            strcpy(response, Responses.RCL_NLG());
        }
        else if (strcmp(status, "EAU") == 0)
        {
            strcpy(response, Responses.RCL_EAU(AID));
        }
        else if (strcmp(status, "EOW") == 0)
        {
            strcpy(response, Responses.RCL_EOW(UID));
        }
        else if (strcmp(status, "END") == 0)
        {
            strcpy(response, Responses.RCL_END(AID, UID));
        }
        return;
    }
    else if (strcmp(msg_type, "RSA") == 0)
    {
        if (strcmp(status, "NOK") == 0)
        {
            strcpy(response, Responses.RSA_NOK());
        }
        else if (strcmp(status, "OK") == 0)
        {
            int FSIZE;
            char FILENAME[256];
            sscanf(response, "%*s %*s %s %d", FILENAME, &FSIZE);
            strcpy(response, Responses.RSA_OK(FSIZE, FILENAME));
        }
        return;
    }
    else if (strcmp(msg_type, "RBD") == 0)
    {
        if (strcmp(status, "OK") == 0)
        {
            strcpy(response, Responses.RBD_OK(AID));
        }
        else if (strcmp(status, "NOK") == 0)
        {
            strcpy(response, Responses.RBD_NOK(AID));
        }
        else if (strcmp(status, "NLG") == 0)
        {
            strcpy(response, Responses.RBD_NLG());
        }
        else if (strcmp(status, "ACC") == 0)
        {
            strcpy(response, Responses.RBD_ACC(AID));
        }
        else if (strcmp(status, "REF") == 0)
        {
            strcpy(response, Responses.RBD_REF());
        }
        else if (strcmp(status, "ILG") == 0)
        {
            strcpy(response, Responses.RBD_ILG());
        }
        return;
    }
    else
    {
        strcpy(response, "ERR");
    }
}

int send_tcp_request(char *buffer, int havefile)
{
    struct addrinfo hints, *res;
    int fd, n, errcode;

    fd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
    if (fd == -1)
        exit(1); // error

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    errcode = getaddrinfo(asip, asport, &hints, &res);
    if (errcode != 0) /*error*/
        exit(1);

    n = connect(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1) /*error*/
        exit(1);

    n = write(fd, buffer, strlen(buffer));
    if (n == -1) /*error*/
        exit(1);

    if (havefile == 1)
    {
        char asset_fname[256];
        int fsize;

        sscanf(buffer, "%*s %*d %*s %*s %*d %*d %s %d", asset_fname, &fsize);
        
        int fd1 = open(asset_fname, O_RDONLY);
        
        n = sendfile(fd, fd1, NULL, fsize);
        
        if (n == -1) /*error*/
            exit(1);
        
        n = write(fd, "\n", 1);
        
        if (n == -1) /*error*/
            exit(1);
        
        close(fd1);
    }
    memset(buffer, 0, MAX_BUFFER_SIZE);
    
    int i = 0;
    int a = 0;
    
    while ((n = read(fd, &buffer[i], 1)) > 0)
    {
        if (buffer[i] == ' ')
            a++;
        if (a == 4)
            break;
        i++;
    }
    if (havefile == 2)
    {
        int filesize;
        char filename[256];
        
        sscanf(buffer, "%*s %*s %s %d", filename, &filesize);
        
        FILE *asset;
        
        char teste[filesize];
        
        asset = fopen(filename, "wb");
        
        int n = 0;
        int fs = 0;
        
        while (fs < filesize)
        {
            n = read(fd, teste, filesize);
            if (n == -1) /*error*/
                exit(1);
            fwrite(teste, 1, n, asset);
            fs += n;
        }
        fclose(asset);
    }

    analyse_tcp_response(buffer);

    write(1, buffer, strlen(buffer));

    freeaddrinfo(res);

    close(fd);
    return 0;
}
// ----------------------------------------------