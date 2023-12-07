#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

int send_udp_request(char *request);

int send_tcp_request(char *request);

void analyse_udp_response(char *response, int *loggedIn);

void analyse_tcp_response(char *response, int *loggedIn);

#endif