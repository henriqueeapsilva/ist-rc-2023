#include "utils.h"

int is_valid_uid(char *uid) {
    // Check if UID has 6 digits
    for (int i = 0; i < strlen(uid); i++)
    {
        if(!isdigit(uid[i]))
            return 0;
    }
    
    return (strlen(uid) == 6);
}

int is_valid_password(const char *password) {
    // Check if password has 8 alphanumerics (only digits and letters)
    for (int i = 0; i < 8; ++i) {
        if (!isalnum(password[i])) {
            return 0;
        }
    }
    return 1;
}
