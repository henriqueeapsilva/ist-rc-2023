#include "dir_functions.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

bool is_the_higher_bid(char *AID, int bid){
    char BIDS_dir_path[PATH_SIZE];
    int highest_bid;
    struct dirent *BID_file;

    sprintf(BIDS_dir_path, "%s/%s/BIDS" ,DIR_AUCTION, AID);
    DIR *BID_dir = opendir(BIDS_dir_path);

    while ((BID_file = readdir(BID_dir))!=NULL){
        sscanf(BID_file -> d_name, "%d", &highest_bid);
    };
    if (bid > highest_bid){
        return true;
    }
    return false;
}

bool user_is_logged(char *UID){
    char user_dir_name[PATH_SIZE];

    sprintf(user_dir_name, "%s/%s/%s_login.txt" ,DIR_USER, UID, UID);
    if (access(user_dir_name, F_OK) != -1) {
        return true;
    }
    return false;
}

bool is_auction(char *AID){
    char auction_dir_name[PATH_SIZE];
    sprintf(auction_dir_name, "%s/%s" ,DIR_AUCTION, AID);
    if (access(auction_dir_name, F_OK) != -1) {
        return true;
    }
    return false;
}

bool check_user_auction(char *UID, char *AID){
    char user_dir_auction[PATH_SIZE];

    sprintf(user_dir_auction, "%s/%s/%s/%s.txt" ,DIR_USER ,UID, "HOSTED", AID);

    if (access(user_dir_auction, F_OK) != -1) {
        return true;
    }
    return false;
}

bool is_auction_finished(char *AID){
    char auction_START_dir[PATH_SIZE];
    char auction_END_dir[PATH_SIZE];
    time_t auction_time;
    time_t auction_duration;
    sprintf(auction_END_dir, "%s/%s/END_%s.txt" ,DIR_AUCTION, AID, AID);
    if (access(auction_END_dir, F_OK) != -1) {
        return true;
    }
    sprintf(auction_START_dir, "%s/%s/START_%s.txt" ,DIR_AUCTION, AID, AID);
    FILE *file = fopen(auction_START_dir, "r");
    fscanf(file, "%*s %*s %*s %*s %ld %*s %ld", &auction_duration, &auction_time);
    if (auction_time+auction_duration > time(NULL)){
        close_auction(AID);
        return true;
    }
    return false;
}

bool is_directory(const char *path){
    struct stat sa;
    return stat(path, &sa) == 0 && S_ISDIR(sa.st_mode);
}

int isDirectoryEmpty(char *path) {
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE); // Error opening directory
    }

    struct dirent *entry;
    int empty = 0; // Assume directory is empty

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_DIR || (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)) {
            empty = 1; // Directory is not empty
            break;
        }
    }

    closedir(dir);

    return empty;
}

int isAuctionsEmpty(){
     char auctions_dir_name[PATH_SIZE];

    sprintf(auctions_dir_name, "%s" ,DIR_AUCTION);

    return isDirectoryEmpty(auctions_dir_name);
}

int isBiddedEmpty(char *UID){
    char bidded_dir_name[PATH_SIZE];

    sprintf(bidded_dir_name, "%s/%s/%s" ,DIR_USER, UID, "BIDDED");

    return isDirectoryEmpty(bidded_dir_name);
}

int isHostedEmpty(char *UID){
    char bidded_dir_name[PATH_SIZE];

    sprintf(bidded_dir_name, "%s/%s/%s" ,DIR_USER, UID, "HOSTED");

    return isDirectoryEmpty(bidded_dir_name);
}

int isBidsEmpty(char *AID){
    char bids_dir_name[PATH_SIZE];

    sprintf(bids_dir_name, "%s/%s/%s" ,DIR_AUCTION, AID, "BIDS");

    return isDirectoryEmpty(bids_dir_name);
}

char* getAuctionStates() {
    char auctions_dir_name[PATH_SIZE];

    sprintf(auctions_dir_name, "%s" ,DIR_AUCTION);

    DIR *dir = opendir(auctions_dir_name);

    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Allocate a buffer to store the results
    size_t bufferSize = MAX_BUFFER_SIZE; // Adjust the size as needed
    char *buffer = (char *)malloc(bufferSize);
    if (buffer == NULL) {
        closedir(dir);
        perror("malloc");
        exit(EXIT_FAILURE); // Error allocating memory
    }

    // Initialize the buffer
    buffer[0] = '\0';

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Skip "." and ".." entries
        }
        
        is_auction_finished(entry->d_name);

        // Construct the path of the potential END file
        char filePath[1024];
        snprintf(filePath, sizeof(filePath), "%s/%s/END_%s.txt", DIR_AUCTION, entry->d_name, entry->d_name);

        // Check if the END file exists
        FILE *endFile = fopen(filePath, "r");
        if (endFile != NULL) {
            // Append AID and state to the buffer
            strcat(buffer, entry->d_name); 
            strcat(buffer, " 0 "); 
            fclose(endFile);
        } else {
            // Auction is active (state 1)
            strcat(buffer, entry->d_name); 
            strcat(buffer, " 1 "); 
        }
    }

    closedir(dir);

    return buffer;
}

char *getBidsUser(char *UID){
    char bidded_dir_name[PATH_SIZE];
    char AID[4];

    sprintf(bidded_dir_name, "%s/%s/%s" ,DIR_USER, UID, "BIDDED");

    DIR *dir = opendir(bidded_dir_name);

    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Allocate a buffer to store the results
    size_t bufferSize = MAX_BUFFER_SIZE; // Adjust the size as needed
    char *buffer = (char *)malloc(bufferSize);
    if (buffer == NULL) {
        closedir(dir);
        perror("malloc");
        exit(EXIT_FAILURE); // Error allocating memory
    }

    // Initialize the buffer
    buffer[0] = '\0';

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Skip "." and ".." entries
        }

        strcpy(AID, entry->d_name);
        AID[3] = '\0';

        is_auction_finished(AID);

        // Construct the path of the potential END file
        char filePath[1024];
        snprintf(filePath, sizeof(filePath), "%s/%s/END_%s.txt", DIR_AUCTION, AID, AID);
           
        // Check if the END file exists
        FILE *endFile = fopen(filePath, "r");
        if (endFile != NULL) {
            // Append AID and state to the buffer
            strcat(buffer, AID); 
            strcat(buffer, " 0 "); 
            fclose(endFile);
        } else {
            // Auction is active (state 1)
            strcat(buffer, AID); 
            strcat(buffer, " 1 "); 
        }
    }

    closedir(dir);

    return buffer;
}




char *getNextAID() {
    char auc_dir_name[PATH_SIZE];
    sprintf(auc_dir_name, "%s", DIR_AUCTION);
    int curr_value, max_value = 0;

    DIR *dir = opendir(auc_dir_name);

    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    char *AID = (char *)malloc(4); // Assuming AID is a three-digit number
    if (AID == NULL) {
        closedir(dir);
        perror("malloc");
        free(AID);
        exit(EXIT_FAILURE); // Error allocating memory
    }


    if (isDirectoryEmpty(auc_dir_name) == 0) {
        sprintf(AID, "001");
    } else {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            sprintf(auc_dir_name, "%s/%s", DIR_AUCTION, entry->d_name);
            if(strlen(entry->d_name) == 3 && is_directory(auc_dir_name)){
                curr_value = atoi(entry->d_name);
                if(curr_value > max_value) max_value = curr_value;
            }
        }
    }

    closedir(dir);
    if(max_value < 999) {
        max_value++;
        snprintf(AID, 4,"%03d", max_value);
    } else {
        snprintf(AID, 4, "nok");
    }
    return AID;
}


char *getAuctionsUser(char *UID){
    char bidded_dir_name[PATH_SIZE];
    char AID[4];

    sprintf(bidded_dir_name, "%s/%s/%s" ,DIR_USER, UID, "HOSTED");

    DIR *dir = opendir(bidded_dir_name);

    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Allocate a buffer to store the results
    size_t bufferSize = MAX_BUFFER_SIZE; // Adjust the size as needed
    char *buffer = (char *)malloc(bufferSize);
    if (buffer == NULL) {
        closedir(dir);
        perror("malloc");
        exit(EXIT_FAILURE); // Error allocating memory
    }

    // Initialize the buffer
    buffer[0] = '\0';

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Skip "." and ".." entries
        }

        strcpy(AID, entry->d_name);
        AID[3] = '\0';

        is_auction_finished(AID);

        // Construct the path of the potential END file
        char filePath[1024];
        snprintf(filePath, sizeof(filePath), "%s/%s/END_%s.txt", DIR_AUCTION, AID, AID);


        // Check if the END file exists
        FILE *endFile = fopen(filePath, "r");
        if (endFile != NULL) {
            // Append AID and state to the buffer
            strcat(buffer, AID); 
            strcat(buffer, " 0 "); 
            fclose(endFile);
        } else {
            // Auction is active (state 1)
            strcat(buffer, AID); 
            strcat(buffer, " 1 "); 
        }
    }
    closedir(dir);

    return buffer;
}

bool user_is_registed(char *UID){
    char user_dir_name[PATH_SIZE];

    sprintf(user_dir_name, "%s/%s/%s_pass.txt" ,DIR_USER ,UID, UID);
    FILE *f_user = fopen(user_dir_name, "r");
    if(f_user == NULL) return false;
    fclose(f_user);
    return true;
}

bool check_user_pass(char *UID, char *password){
     char user_dir_name[PATH_SIZE];
     char dir_password[9];

    sprintf(user_dir_name, "%s/%s/%s_pass.txt" ,DIR_USER ,UID, UID);
    FILE *f_user = fopen(user_dir_name, "r");

    fscanf(f_user, "%s", dir_password);
    dir_password[8] = '\0';

    fclose(f_user);

    if(strcmp(password, dir_password) == 0){
        return true;
    }
    return false;
}

void resgister_user(char *UID, char *password){
    char user_dir_name[PATH_SIZE];
    char user_dir_pass[PATH_SIZE];
    char user_dir_login[PATH_SIZE];
    int ret;


    sprintf(user_dir_name, "%s/%s" ,DIR_USER ,UID);

    sprintf(user_dir_login, "%s/%s/%s_login.txt" ,DIR_USER, UID, UID);

    sprintf(user_dir_pass, "%s/%s/%s_pass.txt" ,DIR_USER, UID, UID);
    
    if(is_directory(user_dir_name)){
        // create login file
        FILE *f_login = fopen(user_dir_login, "w");
        fclose(f_login);

        // create password file
        FILE *f_pass = fopen(user_dir_pass, "w");
        fprintf(f_pass, "%s",password);
        fclose(f_pass);
    } else {
        // create user folder
        ret = mkdir(user_dir_name, 0700);
        if(ret == -1) exit(EXIT_FAILURE);

        sprintf(user_dir_name, "%s/%s/%s" ,DIR_USER ,UID, "HOSTED");

        // create HOSTED folder
        ret = mkdir(user_dir_name, 0700);
        if(ret == -1) exit(EXIT_FAILURE);

        sprintf(user_dir_name, "%s/%s/%s" ,DIR_USER ,UID, "BIDDED");

        // create BIDDED folder
        ret = mkdir(user_dir_name, 0700);
        if(ret == -1) exit(EXIT_FAILURE);

        // create login file
        FILE *f_login = fopen(user_dir_login, "w");
        fclose(f_login);

        // create password file
        FILE *f_pass = fopen(user_dir_pass, "w");
        fprintf(f_pass, "%s",password);
        fclose(f_pass);

    } 
}

void log_client(char *UID){
    char user_dir_name[PATH_SIZE];

    sprintf(user_dir_name, "%s/%s/%s_login.txt" ,DIR_USER, UID, UID);
    FILE *f_user = fopen(user_dir_name, "w");

    fclose(f_user);

}

void log_out_user(char *UID){
    char user_dir_login[PATH_SIZE];

    sprintf(user_dir_login, "%s/%s/%s_login.txt" ,DIR_USER, UID, UID);
    unlink(user_dir_login);
}

void unr_user(char *UID){
    char user_dir_login[PATH_SIZE];
    char user_dir_pass[PATH_SIZE];

    sprintf(user_dir_login, "%s/%s/%s_login.txt" ,DIR_USER, UID, UID);
    sprintf(user_dir_pass, "%s/%s/%s_pass.txt" ,DIR_USER, UID, UID); 

    unlink(user_dir_login);
    unlink(user_dir_pass);      
}

void close_auction(char *AID){
    time_t current_time;
    time_t auction_time;
    char end_datetime[20];
    char auction_START_dir[PATH_SIZE];
    char auction_END_dir[PATH_SIZE];


    sprintf(auction_START_dir, "%s/%s/START_%s.txt" ,DIR_AUCTION, AID, AID);
    FILE *START_file = fopen(auction_START_dir, "r");
    fscanf(START_file, "%*s %*s %*s %*s %*s %*s %ld", &auction_time);
    fclose(START_file);

    // calculate new auction time and end date
    time(&current_time);
    auction_time = current_time - auction_time;
    struct tm *localTimeStruct = localtime(&current_time);
    strftime(end_datetime, 20, "%Y-%m-%d %H:%M:%S", localTimeStruct);

    // create END file
    sprintf(auction_END_dir, "%s/%s/END_%s.txt" ,DIR_AUCTION, AID, AID);
    FILE *END_file = fopen(auction_END_dir, "w");
    fprintf(END_file, "%s %ld", end_datetime, auction_time);
    fclose(END_file);
}

void make_bid(char *UID, char *AID, int bid){
    char user_bid_dir[PATH_SIZE];
    char auction_bid_dir[PATH_SIZE];
    time_t bid_time;
    char bid_date[PATH_SIZE];

    sprintf(user_bid_dir, "%s/%s/%s/%s.txt" ,DIR_USER, UID, "BIDDED", AID);
    sprintf(auction_bid_dir, "%s/%s/BIDS/%06d.txt" ,DIR_AUCTION, AID, bid);

    FILE *user_bid_file = fopen(user_bid_dir, "w");
    FILE *auction_bid_file = fopen(auction_bid_dir, "w");
    time(&bid_time);
    struct tm *localTimeStruct = localtime(&bid_time);
    strftime(bid_date, 20, "%Y-%m-%d %H:%M:%S", localTimeStruct);
    fprintf(auction_bid_file, "%s %06d %s %ld", UID, bid, bid_date, bid_time);

    fclose(user_bid_file);
    fclose(auction_bid_file);
}


void register_auction(int tcp_fd,char *UID, char *AID, char *name, char *asset_fname, int start_value, int timeactive, int fsize){
    char buffer[fsize];
    char user_HOSTED_file[PATH_SIZE];
    char auction_dir_name[PATH_SIZE];
    char auction_START_dir[PATH_SIZE];
    char auction_ASSET_dir[PATH_SIZE];
    char auction_BIDS_dir[PATH_SIZE];
    char auction_BIDS_file[PATH_SIZE];
    char start_datetime[PATH_SIZE];
    time_t start_fulltime;
    time_t bid_time;
    char bid_date[PATH_SIZE];

    // create auction folder
    sprintf(auction_dir_name, "%s/%s" ,DIR_AUCTION, AID);
    mkdir(auction_dir_name, 0700);

    // create START file
    sprintf(auction_START_dir, "%s/%s/START_%s.txt" ,DIR_AUCTION, AID, AID);
    FILE *START_file = fopen(auction_START_dir, "w");
    time(&start_fulltime);
    struct tm *localTimeStruct = localtime(&start_fulltime);
    strftime(start_datetime, 20, "%Y-%m-%d %H:%M:%S", localTimeStruct);
    fprintf(START_file, "%s %s %s %d %d %s %ld", UID, name, asset_fname, start_value, timeactive, start_datetime,start_fulltime);
    fclose(START_file);

    // create ASSET file
    sprintf(auction_dir_name, "%s/%s/%s" ,DIR_AUCTION, AID, "ASSET");
    mkdir(auction_dir_name, 0700);  

    sprintf(auction_ASSET_dir, "%s/%s/ASSET/%s" ,DIR_AUCTION, AID, asset_fname);
    FILE *ASSET_file = fopen(auction_ASSET_dir, "w");
    int fs = 0;
    int bytes_received;
    while (fs < fsize)
    {
        bytes_received = read(tcp_fd, buffer, fsize);
        if (bytes_received == -1) /*error*/
            exit(1);
        fwrite(buffer, 1, bytes_received, ASSET_file);
        fs += bytes_received;
    }
    fclose(ASSET_file);

    // create BIDS folder
    sprintf(auction_BIDS_dir, "%s/%s/BIDS" ,DIR_AUCTION, AID);
    mkdir(auction_BIDS_dir, 0700);

    // create BIDS file
    sprintf(auction_BIDS_file, "%s/%s/BIDS/%06d.txt" ,DIR_AUCTION, AID, start_value);
    FILE *BIDS_file = fopen(auction_BIDS_file, "w");
    time(&bid_time);
    localTimeStruct = localtime(&bid_time);
    strftime(bid_date, 20, "%Y-%m-%d %H:%M:%S", localTimeStruct);
    fprintf(BIDS_file, "%s %06d %s %ld", UID, start_value, bid_date, bid_time);

    fclose(BIDS_file);

    // Create HOSTED file
    sprintf(user_HOSTED_file, "%s/%s/%s/%s.txt" ,DIR_USER ,UID, "HOSTED", AID);
    FILE *HOSTED_file = fopen(user_HOSTED_file, "w");
    fclose(HOSTED_file);
}


char *do_show_record(char *AID){
    char auc_dir_name[100];
    char bidded_dir_name[200];
    char end_file_name[PATH_SIZE];
    char start_file_name[PATH_SIZE];
    char bid_file_path[PATH_SIZE];
    char auction_info[PATH_SIZE];
    char bid_info[PATH_SIZE];
    char end_info[PATH_SIZE];

    sprintf(auc_dir_name, "%s/%s" ,DIR_AUCTION , AID);
    sprintf(bidded_dir_name, "%s/%s", auc_dir_name, "BIDS");
    sprintf(start_file_name, "%s/START_%s.txt", auc_dir_name, AID);
    sprintf(end_file_name, "%s/END_%s.txt", auc_dir_name, AID);


    // start file information
    char host_uid[7];
    char auc_name[50];
    char asset_fname[50];
    char start_value[7];
    char start_datetime[50];
    char start_date[50];
    char timeactive[30];

    FILE *start_file = fopen(start_file_name, "r");
    fscanf(start_file,  "%s %s %s %s %s %s %s", host_uid, auc_name, asset_fname, start_value, timeactive, start_date ,start_datetime);
    sprintf(auction_info, "%s %s %s %s %s %s %s", host_uid, auc_name, asset_fname, start_value, start_date ,start_datetime, timeactive);

    char *buffer = (char *)malloc(strlen(auction_info) + 1);
    if (buffer == NULL) {
        perror("malloc");
        free(buffer);
        exit(EXIT_FAILURE); 
    }

    strcat(buffer, auction_info);


    // bids information
    char bid_value[7];
    char bid_datetime[50]; 
    char bid_sec_time[50];
    char _start_value[7];
    int i = 0;
    

    DIR *dir = opendir(bidded_dir_name);

    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    if(isBidsEmpty(AID) == 1){

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue; 
            }
            if(entry == NULL ) break;

            sscanf(entry->d_name, "%6s.txt", _start_value);
            
            if(atoi(_start_value) == atoi(start_value)) continue;
       
            sprintf(bid_file_path, "%s/%s", bidded_dir_name, entry->d_name);
            FILE *bid_file = fopen(bid_file_path, "r");
            fscanf(bid_file,  "%s %s %s %s", host_uid, bid_value, bid_datetime, bid_sec_time);
            sprintf(bid_info, " B %s %s %s %s", host_uid, bid_value, bid_datetime, bid_sec_time);
            i++;
            buffer = realloc(buffer, strlen(auction_info) + strlen(bid_info)*i + 2);

            strcat(buffer, bid_info);
        }
    }
    //end file information
    char end_date_time[50];
    char end_sec_time[50];


    FILE *end_file = fopen(end_file_name, "r");
    if(end_file == NULL) return buffer;

    fscanf(end_file,  "%s %s", end_date_time, end_sec_time);
    sprintf(end_info, " E %s %s", end_date_time, end_sec_time);

    buffer = realloc(buffer, strlen(auction_info) + strlen(end_info) + 2);
    strcat(buffer, end_info);

    return buffer;
}   
