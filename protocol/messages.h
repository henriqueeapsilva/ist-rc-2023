
#ifndef RESPONSE_MESSAGES_H
#define RESPONSE_MESSAGES_H

// ---------------- UDP Messages ----------------

// RLI Status Messages
#define RLI_OK "successful login\n"
#define RLI_NOK "incorrect login attempt\n"
#define RLI_REG "new user registered\n"

// RLO Status Messages
#define RLO_OK "successful logout\n"
#define RLO_NOK "user not logged in\n"
#define RLO_UNR "unknow user\n"

// RUR Status Messages
#define RUR_OK "successful unregister\n"
#define RUR_NOK " incorrect unregister attempt\n"
#define RUR_UNR "unknown user\n"

// RMA Status Messages
// RMS_OK sends the list with the auctions the user is involved in
#define RMA_NOK "user is not involved in any auctions\n"
#define RMA_NLG "user not logged in\n"

// RMB Status Messages
// RMB_OK sends the list with the bids the user is involved in
#define RMB_NOK "user has not placed any bids\n"
#define RMB_NLG "user not logged in\n"

// RLS Status Messages
// RLS_OK send a list with all the auctions
#define RLS_NOK " no auctions were yet created\n"

// RRC Status Messages
// RRC_OK sends all the stats from the user
#define RRC_NOK " auction AID does not exist\n"

// ----------------------------------------------

// ---------------- TCP Messages ----------------

// ROA Status Messages
#define ROA_NOK "auction could not be started\n"
#define ROA_NLG "user not logged in\n"

// RCL Status Messages
#define RCL_OK "auction successfully closed\n"
#define RCL_NOK "user UID does not exis\n"
#define RCL_NLG "user not logged in\n"
#define RCL_EAU " auction does not exist\n"
#define RCL_EOW "e auction is not owned by user\n"
#define RCL_END "auction owned by user has already finished.\n" 
// find a way to put the AID and UID before auction and user

// RSA Status Messages
#define RSA_NOK "no file to be sent\n"

// RBD Status Messages
#define RBD_NOK "auction is not active\n"
#define RBD_NLG "user not logged in\n"
#define RBD_ACC "bid accepted\n"
#define RBD_REF "bid was refused - larger bid has already been placed previously\n"
#define RBD_ILG "cannot bid on own auction\n"

// ----------------------------------------------

#endif 
