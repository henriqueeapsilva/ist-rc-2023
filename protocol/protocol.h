#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_


char *handle_myauctions(char *buffer);

char *handle_mybids(char *buffer);

char *handle_list(char *buffer);

char *handle_show_record(char *buffer);


int send_udp_request(char *request);

int send_tcp_request(char *request, int isFile);

void analyse_udp_response(char *response, int *loggedIn);

void analyse_tcp_response(char *response);

#endif