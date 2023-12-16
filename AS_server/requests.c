#include "requests.h"

void send_reply(int fd, struct sockaddr_in addr,const char *reply){
     if(sendto(fd, reply, strlen(reply), 0, (struct sockaddr*)&addr,sizeof(addr)) == -1) exit(1);
}


int login_handler(int fd, struct sockaddr_in addr,  char *buffer){
    char UID[7];
    char password[9];  
    

    if (sscanf(buffer, "%*s %s %s", UID, password) != 2) {
        send_reply(fd, addr, Messages.LIN_ERR());
    }
    else if (is_valid_uid(UID) == 1 || is_valid_password(password) == 1) {
        send_reply(fd, addr, Messages.LIN_ERR());
    }
    else if(!user_is_registed(UID)){
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
    char UID[7];
    char password[9];  
    

    if (sscanf(buffer, "%*s %s %s", UID, password) != 2) {
        send_reply(fd, addr, Messages.LOU_ERR());
    } else if (is_valid_uid(UID) == 1 || is_valid_password(password) == 1) {
        send_reply(fd, addr, Messages.LOU_ERR());
    } else if(!user_is_registed(UID)){
        send_reply(fd, addr, Messages.LOU_UNR());
    } else if(!user_is_logged(UID)){
        send_reply(fd, addr, Messages.LOU_NOK());
    } else {
        log_out_user(UID);
        send_reply(fd, addr, Messages.LOU_OK());
    }

    return 0;
}

int unregister_handler(int fd, struct sockaddr_in addr,  char *buffer){
    char UID[7];
    char password[9];  
    

    if (sscanf(buffer, "%*s %s %s", UID, password) != 2) {
        send_reply(fd, addr, Messages.UNR_ERR());
    } else if (is_valid_uid(UID) == 1 || is_valid_password(password) == 1) {
        send_reply(fd, addr, Messages.UNR_ERR());
    } else if(!user_is_logged(UID)){
        send_reply(fd, addr, Messages.UNR_NOK());
    } else if(!user_is_registed(UID)){
        send_reply(fd, addr, Messages.UNR_UNR());
    } else {
        unr_user(UID);
        send_reply(fd, addr, Messages.UNR_OK());
    }


    return 0;
}


int open_handler(int fd, struct sockaddr_in addr,  char *buffer){
    char UID[7];
    char password[9];
    int start_value, timeactive, asset_fsize;
    char name[50], asset_fname[200];
    if (sscanf(buffer, "%*s %s %s %s %d %d %s %d\n", UID, password, name, &start_value, &timeactive, asset_fname, &asset_fsize) != 7) {
        send_reply(fd, addr, Messages.OPA_ERR());
        return 1;
    }
    else if (is_valid_uid(UID) == 1 || is_valid_password(password) == 1) {
        send_reply(fd, addr, Messages.OPA_ERR());
        return 1;
    }

    else if(!user_is_logged(UID)){
        send_reply(fd, addr, Messages.OPA_NLG());
        return 1;
    }
    else{
        char *AID = generate_random_number();
        register_auction(fd, UID, AID, name, asset_fname, start_value, timeactive, asset_fsize);
        send_reply(fd, addr, Messages.OPA_OK(AID));
        return 1;
    }
    return 0;
}

int close_handler(int fd, struct sockaddr_in addr,  char *buffer){
    char UID[7];
    char password[9];
    char AID[3];

    if (sscanf(buffer, "%*s %s %s %s", UID, password, AID) != 3) {
        send_reply(fd, addr, Messages.CLS_ERR());
    }
    else if (is_valid_uid(UID) == 1 || is_valid_password(password) == 1 || is_valid_aid(AID) == 0) {
        send_reply(fd, addr, Messages.CLS_ERR());
    }
    else if(!user_is_logged(UID)){
        send_reply(fd, addr, Messages.CLS_NLG());
    }
    else if(!is_auction(AID)){
        send_reply(fd, addr, Messages.CLS_EAU());
    }
    else if(!check_user_auction(UID, AID)){
        send_reply(fd, addr, Messages.CLS_EOW());
    }
    else if(is_auction_finished(AID)){
        send_reply(fd, addr, Messages.CLS_END());
    }
    else {
        close_auction(AID);
        send_reply(fd, addr, Messages.CLS_OK());
    }
    return 0;
}


int myauctions_handler(int fd, struct sockaddr_in addr,  char *buffer){
    char UID[7];

    if (sscanf(buffer, "%*s %s", UID) != 1) {
        send_reply(fd, addr, Messages.LMA_ERR());
        return 1;
    } else if (is_valid_uid(UID) == 1) {
        send_reply(fd, addr, Messages.LMA_ERR());
    } else if (!user_is_logged(UID)){
        send_reply(fd, addr, Messages.LMB_NLG());
    } else if(isHostedEmpty(UID) == 1){
        send_reply(fd, addr, Messages.LMB_NOK());
    } else {
        strcpy(buffer, getAuctionsUser(UID));
        send_reply(fd, addr, Messages.LMB_OK(buffer));    
    }
    return 0;
}


int mybids_handler(int fd, struct sockaddr_in addr,  char *buffer){
    char UID[7];

    if (sscanf(buffer, "%*s %s", UID) != 1) {
        send_reply(fd, addr, Messages.LMB_ERR());
        return 1;
    } else if (is_valid_uid(UID) == 1) {
        send_reply(fd, addr, Messages.LMB_ERR());
    } else if (!user_is_logged(UID)){
        send_reply(fd, addr, Messages.LMB_NLG());
    } else if(isBiddedEmpty(UID) == 1){
        send_reply(fd, addr, Messages.LMB_NOK());
    } else {
        strcpy(buffer, getBidsUser(UID));
        send_reply(fd, addr, Messages.LST_OK(buffer));    
    }
    return 0;
    }


int list_handler(int fd, struct sockaddr_in addr,  char *buffer){
    if(isAuctionsEmpty() == 1){
        send_reply(fd, addr, Messages.LST_NOK());
    } else {
        strcpy(buffer, getAuctionStates());
        send_reply(fd, addr, Messages.LST_OK(buffer));    
    }
    return 0;
}


int show_asset_handler(int fd, struct sockaddr_in addr,  char *buffer){
    char AID[3];
    if (sscanf(buffer, "%*s %s", AID) != 1) {
        send_reply(fd, addr, Messages.SAS_ERR());
    }
    else if (is_valid_aid(AID) == 0) {
        send_reply(fd, addr, Messages.SAS_ERR());
    }
    else if (!is_auction(AID)) {
        send_reply(fd, addr, Messages.SAS_NOK());
    }
    else {
        send_asset(fd, AID);
    }
    return 0;
    }   

int bid_handler(int fd, struct sockaddr_in addr,  char *buffer){
    char UID[7];
    char password[9];
    char AID[3];
    int value;

    if (sscanf(buffer, "%*s %s %s %s %d", UID, password, AID, &value) != 4) {
        send_reply(fd, addr, Messages.BID_ERR());
    }
    else if (is_valid_uid(UID) == 0 || is_valid_password(password) == 0 || is_valid_aid(AID) == 0) {
        send_reply(fd, addr, Messages.BID_ERR());
    }
    else if(!is_auction(AID)){
        send_reply(fd, addr, Messages.BID_NOK());
    }
    else if(is_auction_finished(AID)){
        send_reply(fd, addr, Messages.BID_NOK());
    }
    else if(!user_is_logged(UID)){
        send_reply(fd, addr, Messages.BID_NLG());
    }
    else if(check_user_auction(UID, AID)){
        send_reply(fd, addr, Messages.BID_ILG());
    }
    else if(is_the_higher_bid(AID, value)){
        send_reply(fd, addr, Messages.BID_REF());
    }
    else{
        make_bid(UID, AID, value);
    }
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
    printf("command: %s\nbuffer: %s\n", command, buffer);
    for (int i = 0; commandTable[i].command != NULL; i++) {
        if (strcmp(commandTable[i].command, command) == 0) {
            commandTable[i].function(fd,addr,buffer);
            return;
        }
    }
    printf("Invalid command\n");
    return ;
}
