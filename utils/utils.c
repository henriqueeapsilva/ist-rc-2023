#include "utils.h"

int is_valid_uid(char *uid) {
    // Check if UID has 6 digits
    for (int i = 0; i < strlen(uid); i++)
    {
        if(!isdigit(uid[i])){
            return 0;
        }
    }
    
    if (strlen(uid) == 6) return 0;
    return 1;
}

int is_valid_aid(char *aid) {
    // Check if AID has 3 digits
    for (int i = 0; i < strlen(aid); i++)
    {
        if(!isdigit(aid[i]))
            return 0;
    }
    
    return (strlen(aid) == 3);
}

int is_valid_password(const char *password) {
    // Check if password has 8 alphanumerics (only digits and letters)
    for (int i = 0; i < 8; ++i) {
        if (!isalnum(password[i])) {
            return 1;
        }
    }
    return 0;
}
