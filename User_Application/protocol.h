#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "../utils/utils.h"
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
#include <sys/sendfile.h>
#include <stdbool.h>
#include <fcntl.h>


char *handle_myauctions(char *buffer);

char *handle_mybids(char *buffer);

char *handle_list(char *buffer);

char *handle_show_record(char *buffer);


int send_udp_request(char *request);

int send_tcp_request(char *request, int isFile);

void analyse_udp_response(char *response, int *loggedIn);

void analyse_tcp_response(char *response);

#endif