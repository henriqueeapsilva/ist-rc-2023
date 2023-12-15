#include "../utils/utils.h"

bool is_the_higher_bid(char *AID, int bid);

bool user_is_logged(char *UID);

bool is_auction(char *AID);

bool check_user_auction(char *UID, char *AID);

bool is_auction_finished(char *AID);

bool is_directory(const char *path);

bool user_is_registed(char *UID);

bool check_user_pass(char *UID, char *password);

void close_auction(char *AID);

void resgister_user(char *UID, char *password);

void log_client(char *UID);

void log_out_user(char *UID);

void unr_user(char *UID);

int isAuctionsEmpty();

char* getAuctionStates(); 

void make_bid(char *UID, char *AID, int bid);

void register_auction(int tcp_fd,char *UID, char *AID, char *name, char *asset_fname, int start_value, int timeactive, int fsize);

char *generate_random_number();