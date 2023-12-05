#ifndef _FUNTIONS_H_
#define _FUNCTION_H_

#define MAX_RESPONSE_SIZE 6010

extern char *ASIP;
extern char *ASport;



typedef struct user
{
    int UID;
    char* password;
}User;

typedef int (*CommandFunction)();

// Command table entry
typedef struct {
    const char *command;
    CommandFunction function;
} CommandEntry;

int executeCommand(const char *command, char* buffer);

int login(char *buffer);

int open_(char *buffer);

int close_(char *buffer);

int myauctions(char *buffer);

int mybids(char *buffer);

int list(char *buffer);

int show_asset(char *buffer);

int bid(char *buffer);

int show_record(char *buffer);

int logout(char *buffer);

int unsubscribe(char *buffer);

int exit_(char *buffer);

int send_udp_request(char *request);

int send_tcp_request(char *request);

#endif
