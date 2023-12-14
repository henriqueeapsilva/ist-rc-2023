#include "../utils/utils.h"

bool is_directory(const char *path);

bool user_is_registed(char *UID);

bool check_user_pass(char *UID, char *password);

void resgister_user(char *UID, char *password);

void log_client(char *UID);