#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_


void handle_myauctions(char *buffer);

void handle_mybids(char *buffer);

void handle_list(char *buffer);

void handle_show_record(char *buffer);

void handle_show_asset(int file, int filesize, char *filename);


int send_udp_request(char *request);

int send_tcp_request(char *request, int isFile);

void analyse_udp_response(char *response, int *loggedIn);

void analyse_tcp_response(char *response);

#endif