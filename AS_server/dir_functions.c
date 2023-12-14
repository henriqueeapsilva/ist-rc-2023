#include "dir_functions.h"

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