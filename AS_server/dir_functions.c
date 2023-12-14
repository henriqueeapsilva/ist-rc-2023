#include "dir_functions.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    fclose(f_user);

    if(strcmp(password, dir_password)){
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