#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

int send_udp_request(char *request);

int send_tcp_request(char *request);

void analyse_udp_response(char *buffer);

void analyse_tcp_response(char *buffer);

#endif