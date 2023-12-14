#include "requests.h"

void send_reply(int fd, struct sockaddr_in addr,const char *reply){
     if(sendto(fd, reply, strlen(reply), 0, (struct sockaddr*)&addr,sizeof(addr)) == -1) exit(1);
}


int login_handler(int fd, struct sockaddr_in addr,  char *buffer){
    char UID[7];
    char password[9];  
    

    if (sscanf(buffer, "%*s %s %s", UID, password) != 2) {
        send_reply(fd, addr, Messages.LIN_ERR());
        return 1;
    }
    if (is_valid_uid(UID) == 0 || is_valid_password(password) == 0) {
        send_reply(fd, addr, Messages.LIN_NOK());
        return 1;
    }
    if(!user_is_registed(UID)){
        resgister_user(UID, password);
        send_reply(fd, addr, Messages.LIN_REG());
    } else {
        if(!check_user_pass(UID, password)){
            send_reply(fd, addr, Messages.LIN_NOK());
        } else {
            log_client(UID);
            send_reply(fd, addr, Messages.LIN_OK());
        }
    }

    return 0;
}

int logout_handler(int fd, struct sockaddr_in addr,  char *buffer){
    return 0;
}

int unregister_handler(int fd, struct sockaddr_in addr,  char *buffer){
    return 0;
}

int exit_handler(int fd, struct sockaddr_in addr,  char *buffer){
    return 0;
}

int open_handler(int fd, struct sockaddr_in addr,  char *buffer){
    return 0;
}


int close_handler(int fd, struct sockaddr_in addr,  char *buffer){
    return 0;
}


int myauctions_handler(int fd, struct sockaddr_in addr,  char *buffer){
    return 0;
}


int mybids_handler(int fd, struct sockaddr_in addr,  char *buffer){
    return 0;
    }


int list_handler(int fd, struct sockaddr_in addr,  char *buffer){
    return 0;
    }


int show_asset_handler(int fd, struct sockaddr_in addr,  char *buffer){
    return 0;
    }


int bid_handler(int fd, struct sockaddr_in addr,  char *buffer){
    return 0;
    }


int show_record_handler(int fd, struct sockaddr_in addr,  char *buffer){
    return 0;
    }




// Command table
CommandEntry commandTable[] = {
    {"LIN", login_handler},
    {"LOU", logout_handler},
    {"UNR", unregister_handler},
    {"LMA", myauctions_handler},
    {"LMB", mybids_handler},
    {"LST", list_handler},
    {"SRC", show_record_handler},
    {"OPA", open_handler},
    {"CLS", close_handler},
    {"SAS", show_asset_handler},
    {"BID", bid_handler},
    {NULL, NULL} // End of table marker
};

void execute_request(int fd, struct sockaddr_in addr, const char *command, char* buffer) {
    for (int i = 0; commandTable[i].command != NULL; i++) {
        if (strcmp(commandTable[i].command, command) == 0) {
            commandTable[i].function(fd,addr,buffer);
            return;
        }
    }
    printf("Invalid command\n");
    return ;
}
