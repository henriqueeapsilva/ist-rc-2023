#include "dir_functions.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
bool is_the_higher_bid(char *AID, int bid){
    char BIDS_dir_path[50];
    int highest_bid;

    sprintf(BIDS_dir_path, "%s/%s/BIDS" ,DIR_AUCTION, AID);
    DIR *BID_dir = opendir(BIDS_dir_path);

    struct dirent *BID_file = readdir(BID_dir);
    sscanf(BID_file -> d_name, "%d.txt", &highest_bid);
    if (bid > highest_bid){
        return true;
    }
    return false;
}

bool user_is_logged(char *UID){
    char user_dir_name[50];

    sprintf(user_dir_name, "%s/%s/%s_login.txt" ,DIR_USER, UID, UID);
    if (access(user_dir_name, F_OK) != -1) {
        return true;
    }
    return false;
}

bool is_auction(char *AID){
    char auction_dir_name[50];
    sprintf(auction_dir_name, "%s/%s" ,DIR_AUCTION, AID);
    if (access(auction_dir_name, F_OK) != -1) {
        return true;
    }
    return false;
}

bool check_user_auction(char *UID, char *AID){
    char user_dir_auction[50];

    sprintf(user_dir_auction, "%s/%s/%s/%s" ,DIR_USER ,UID, "HOSTED", AID);

    if (access(user_dir_auction, F_OK) != -1) {
        return true;
    }
    return false;
}

bool is_auction_finished(char *AID){
    char auction_START_dir[50];
    char auction_END_dir[50];
    time_t auction_time; 
    sprintf(auction_END_dir, "%s/%s/END_%s.txt" ,DIR_AUCTION, AID, AID);
    if (access(auction_END_dir, F_OK) != -1) {
        return true;
    }
    sprintf(auction_START_dir, "%s/%s/START_%s.txt" ,DIR_AUCTION, AID, AID);
    FILE *file = fopen(auction_START_dir, "r");
    fscanf(file, "%*s %*s %*s %*s %ld %*s %*s", &auction_time);
    if (auction_time < time(NULL)){
        close_auction(AID);
        return true;
    }
    return false;
}

bool is_directory(const char *path){
    struct stat sa;
    return stat(path, &sa) == 0 && S_ISDIR(sa.st_mode);
}

int isAuctionsEmpty() {
    char auctions_dir_name[50];

    sprintf(auctions_dir_name, "%s" ,DIR_AUCTION);

    DIR *dir = opendir(auctions_dir_name);

    if (dir == NULL) {
        printf("opendir");
        exit(EXIT_FAILURE); // Error opening directory
    }

    struct dirent *entry = readdir(dir);

    closedir(dir);

    return entry == NULL; // Return 0 if entry is not NULL (directory is not empty), 1 otherwise
}

char* getAuctionStates() {
    char auctions_dir_name[50];

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


bool user_is_registed(char *UID){
    char user_dir_name[50];

    sprintf(user_dir_name, "%s/%s/%s_pass.txt" ,DIR_USER ,UID, UID);
    FILE *f_user = fopen(user_dir_name, "r");
    if(f_user == NULL) return false;
    fclose(f_user);
    return true;
}

bool check_user_pass(char *UID, char *password){
     char user_dir_name[50];
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
    char user_dir_name[50];
    char user_dir_pass[50];
    char user_dir_login[50];
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
    char user_dir_name[50];

    sprintf(user_dir_name, "%s/%s/%s_login.txt" ,DIR_USER, UID, UID);
    FILE *f_user = fopen(user_dir_name, "w");

    fclose(f_user);

}

void log_out_user(char *UID){
    char user_dir_login[50];

    sprintf(user_dir_login, "%s/%s/%s_login.txt" ,DIR_USER, UID, UID);
    unlink(user_dir_login);
}

void unr_user(char *UID){
    char user_dir_login[50];
    char user_dir_pass[50];

    sprintf(user_dir_login, "%s/%s/%s_login.txt" ,DIR_USER, UID, UID);
    sprintf(user_dir_pass, "%s/%s/%s_pass.txt" ,DIR_USER, UID, UID); 

    unlink(user_dir_login);
    unlink(user_dir_pass);      
}

void close_auction(char *AID){
    time_t current_time;
    time_t auction_time;
    char end_datetime[20];
    char auction_START_dir[50];
    char auction_END_dir[50];

    // create START file
    sprintf(auction_START_dir, "%s/%s/START_%s.txt" ,DIR_AUCTION, AID, AID);
    FILE *START_file = fopen(auction_START_dir, "r");
    fscanf(START_file, "%*s %*s %*s %*s %ld %*s %*s", &auction_time);
    fclose(START_file);

    // calculate new auction time and end date
    time(&current_time);
    auction_time = current_time - auction_time;
    struct tm *localTimeStruct = localtime(&current_time);
    strftime(end_datetime, 20, "%Y-%m-%dT%H:%M:%S", localTimeStruct);

    // create END file
    sprintf(auction_END_dir, "%s/%s/END_%s.txt" ,DIR_AUCTION, AID, AID);
    FILE *END_file = fopen(auction_END_dir, "w");
    fprintf(END_file, "%s %ld", end_datetime, auction_time);
}

void make_bid(char *UID, char *AID, int bid){
    char user_bid_dir[50];
    char auction_bid_dir[50];

    sprintf(user_bid_dir, "%s/%s/%s/%s.txt" ,DIR_USER, UID, "BIDDED", AID);
    sprintf(auction_bid_dir, "%s/%s/BIDS/%d.txt" ,DIR_AUCTION, AID, bid);

    FILE *user_bid_file = fopen(user_bid_dir, "w");
    FILE *auction_bid_file = fopen(auction_bid_dir, "w");

    fclose(user_bid_file);
    fclose(auction_bid_file);
}

void register_auction(int tcp_fd,char *UID, char *AID, char *name, char *asset_fname, int start_value, int timeactive, int fsize){
    char buffer[fsize];
    char user_HOSTED_file[50];
    char auction_dir_name[50];
    char auction_START_dir[50];
    char auction_ASSET_dir[50];
    char auction_BIDS_dir[50];
    char auction_BIDS_file[50];
    char start_datetime[20];
    time_t start_fulltime;

    // create auction folder
    sprintf(auction_dir_name, "%s/%s" ,DIR_AUCTION, AID);
    mkdir(auction_dir_name, 0700);

    // create START file
    sprintf(auction_START_dir, "%s/%s/START_%s.txt" ,DIR_AUCTION, AID, AID);
    FILE *START_file = fopen(auction_START_dir, "w");
    time(&start_fulltime);
    struct tm *localTimeStruct = localtime(&start_fulltime);
    strftime(start_datetime, 20, "%Y-%m-%dT%H:%M:%S", localTimeStruct);
    fprintf(START_file, "%s %s %s %d %d %s %ld", UID, name, asset_fname, start_value, timeactive, start_datetime,start_fulltime);
    fclose(START_file);

    // create ASSET file
    sprintf(auction_dir_name, "%s/%s/%S" ,DIR_AUCTION, AID, "ASSET");
    mkdir(auction_dir_name, 0700);  

    sprintf(auction_ASSET_dir, "%s/%s/ASSET/%s.txt" ,DIR_AUCTION, AID, asset_fname);
    FILE *ASSET_file = fopen(auction_ASSET_dir, "w");
    int fs = 0;
    int bytes_received;
    while (fs < fsize)
    {
        bytes_received = recv(tcp_fd, buffer, fsize, 0);
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
    sprintf(auction_BIDS_file, "%s/%s/BIDS/%d.txt" ,DIR_AUCTION, AID, start_value);
    FILE *BIDS_file = fopen(auction_BIDS_file, "w");
    fclose(BIDS_file);

    // Create HOSTED file
    sprintf(user_HOSTED_file, "%s/%s/%s/%s.txt" ,DIR_USER ,UID, "HOSTED", AID);
    FILE *HOSTED_file = fopen(auction_BIDS_file, "w");
    fclose(HOSTED_file);
}

char *generate_random_number() {
    // Seed para a função rand() baseada no tempo atual
    srand((unsigned int)time(NULL));

    // Gerar um número aleatório de três dígitos
    int random_number = rand() % 1000;

    // Alocar espaço para a string (4 caracteres para os dígitos + 1 para o caractere nulo '\0')
    char* result = (char*)malloc(4 * sizeof(char));

    // Verificar se a alocação de memória foi bem-sucedida
    if (result == NULL) {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    // Converter o número inteiro para uma string
    sprintf(result, "%03d", random_number);

    return result;
}